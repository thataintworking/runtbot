// wheel.cpp
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

// TB6612FNG Truth Table
//
//      INA     INB     Effect
//      ----    ----    --------------------
//      LOW     LOW     Brake
//      LOW     HIGH    Forward
//      HIGH    LOW     Reverse
//      HIGH    HIGH    Coast (do not use)

#include "wheel.h"

const boolean _debug = true;

const unsigned long ADJ_DELAY = 200L;

Wheel::Wheel(String label, int pwmPin, int inaPin, int inbPin, int initoff, boolean debug) :
  _lastTickTime(0L),
  _nextAdjTime(0L),
  _tbix(0),
  _pwmPin(pwmPin),
  _inaPin(inaPin),
  _inbPin(inbPin),
  _initoff(initoff),
  _speed(0),
  _pwm(0),
  _label(label),
  _debug(debug)
{
  if (_debug) {
    Serial.print(label);
    Serial.print(" Wheel: ");
    Serial.print("Initializing pwmPin=");
    Serial.print(pwmPin);
    Serial.print(", inaPin=");
    Serial.print(inaPin);
    Serial.print(", inbPin=");
    Serial.print(inbPin);
    Serial.print(", initoff=");
    Serial.println(initoff);
  }
  for (int i = 0; i < TBSZ; i++) _tickBuf[i] = 0L;
  pinMode(_pwmPin, OUTPUT);
  pinMode(_inaPin, OUTPUT);
  pinMode(_inbPin, OUTPUT);
  setSpeed(0);
}


Wheel::~Wheel() {
  analogWrite(_pwmPin, 0);
  digitalWrite(_inaPin, LOW);
  digitalWrite(_inbPin, LOW);
}


void Wheel::tick() {
  unsigned long mics = micros();
  _tickBuf[_tbix] = mics - _lastTickTime;
  _lastTickTime = mics;
  if (++_tbix >= TBSZ) _tbix = 0;
}


unsigned int Wheel::avgTickTime() {
  unsigned long total = 0L;
  noInterrupts();
  for (int i = 0; i < TBSZ; i++) total += _tickBuf[i];
  interrupts();
  return (unsigned int)(total / TBSZ);
}


void Wheel::setSpeed(int s) {
  if ((s > 0 && _speed < 0) || (s < 0 && _speed > 0)) {
    if (_debug) {
      Serial.print(_label);
      Serial.print(" Wheel: ");
      Serial.println("Stopping motor to prepare for direction change.");
    }
    setPWM(0);
    delay(ADJ_DELAY);
  }

  if (s > MAX_FWD_SPEED) _speed = MAX_FWD_SPEED;
  else if (s < MAX_REV_SPEED) _speed = MAX_REV_SPEED;
  else _speed = s;

  if (_debug) {
    Serial.print(_label);
    Serial.print(" Wheel: ");
    Serial.print("Setting direction to ");
    Serial.print(_speed < 0 ? "REVERSE" : (_speed > 0 ? "FORWARD" : "STOP"));
    Serial.print(" (ina ");
    Serial.print(_speed >= 0 ? "LOW" : "HIGH");
    Serial.print(", inb ");
    Serial.print(_speed <= 0 ? "LOW" : "HIGH");
    Serial.print(")");
  }
  digitalWrite(_inaPin, _speed >= 0 ? LOW : HIGH);
  digitalWrite(_inbPin, _speed <= 0 ? LOW : HIGH);

  if (_debug) {
    Serial.print(" and speed to ");
    Serial.println(_speed);
  }
  setPWM(10 * abs(_speed) + _initoff);
}


void Wheel::setPWM(int pwm) {
  if (pwm < 0) _pwm = 0;
  else if (pwm > 255) _pwm = 255;
  else _pwm = pwm;
  if (_debug) {
    Serial.print(_label);
    Serial.print(" Wheel: ");
    Serial.print("Setting PWM to ");
    Serial.println(_pwm);
  }
  analogWrite(_pwmPin, _pwm);
}


