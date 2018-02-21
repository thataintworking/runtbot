// Wheel.h
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include "Wheel.h"

const boolean _debug = true;

const int FORWARD = LOW;
const int REVERSE = HIGH;

const unsigned long ADJ_DELAY = 100L;

const unsigned int TTT[] = {
  0, 119159, 80159, 44088, 30524, 23040, 18495, 15711, 14102, 11875, 10348, 9067, 
  8316, 7671, 7182, 6458, 5965, 5433, 5174, 4988, 4625, 4475, 4301, 3973, 3954, 3974
};

Wheel::Wheel(String label, int pwmPin, int dirPin, boolean debug) : 
  _lastTickTime(0L),
  _nextAdjTime(0L),
  _tbix(0),
  _pwmPin(pwmPin), 
  _dirPin(dirPin), 
  _speed(0),
  _pwm(0),
  _label(label),
  _debug(debug)
{ 
  if (_debug) Serial.print(label);
  if (_debug) Serial.print(" Wheel: ");
  if (_debug) Serial.print("Initializing pwmPin=");
  if (_debug) Serial.print(pwmPin);
  if (_debug) Serial.print(", dirPin=");
  if (_debug) Serial.println(dirPin);
  for (int i = 0; i < TBSZ; i++) _tickBuf[i] = 0L;
  pinMode(_pwmPin, OUTPUT);
  pinMode(_dirPin, OUTPUT);
  analogWrite(_pwmPin, 0);
  digitalWrite(_dirPin, FORWARD);     
}


Wheel::~Wheel() { 
  analogWrite(_pwmPin, 0);
  digitalWrite(_dirPin, FORWARD);     
}


void Wheel::loop(unsigned long m) {
  if (_speed && m > _nextAdjTime) {
    unsigned int targetTickTime = TTT[abs(_speed)];
    unsigned int tickTime = avgTickTime();
    if (tickTime > 0) {
      if (tickTime < targetTickTime) {
        if (_debug) Serial.print(_label);
        if (_debug) Serial.print(" Wheel: ");
        if (_debug) Serial.print("tickTime (");
        if (_debug) Serial.print(tickTime);
        if (_debug) Serial.print(") is less than target (");
        if (_debug) Serial.print(targetTickTime);
        if (_debug) Serial.println(") decreasing PWM");
        setPWM(_pwm - 1);
      } else if (tickTime > targetTickTime) {
        if (_debug) Serial.print(_label);
        if (_debug) Serial.print(" Wheel: ");
        if (_debug) Serial.print("tickTime (");
        if (_debug) Serial.print(tickTime);
        if (_debug) Serial.print(") is greater than target (");
        if (_debug) Serial.print(targetTickTime);
        if (_debug) Serial.println(") increasing PWM");
        setPWM(_pwm + 1);
      }
    }
    _nextAdjTime = m + ADJ_DELAY; 
  }
}


void Wheel::tick() {
  unsigned long mics = micros();
  _tickBuf[_tbix] = mics - _lastTickTime;
  _lastTickTime = mics;
  if (++_tbix >= TBSZ) _tbix = 0;
}


unsigned int Wheel::avgTickTime() {
  unsigned long total = 0L;
  for (int i = 0; i < TBSZ; i++) total += _tickBuf[i];
  return (unsigned int)(total / TBSZ);
}


void Wheel::setSpeed(int s) {
  if ((s > 0 && _speed < 0) || (s < 0 && _speed > 0)) {
    if (_debug) Serial.print(_label);
    if (_debug) Serial.print(" Wheel: ");
    if (_debug) Serial.println("Stopping motor to prepare for direction change.");
    setPWM(0);
    delay(ADJ_DELAY);
  }
  if (s > 25) _speed = 25;
  else if (s < -25) _speed = -25;
  else _speed = s;
  if (_debug) Serial.print(_label);
  if (_debug) Serial.print(" Wheel: ");
  if (_debug) Serial.print("Setting directrion to ");
  if (_debug) Serial.print(_speed < 0 ? "REVERSE" : "FORWARD");
  digitalWrite(_dirPin, _speed < 0 ? REVERSE : FORWARD); 
  if (_debug) Serial.print(" and speed to ");
  if (_debug) Serial.println(_speed);
  setPWM(abs(_speed) * 10);
}


void Wheel::setPWM(int pwm) {
  if (pwm < 0) _pwm = 0;
  else if (pwm > 255) _pwm = 255;
  else _pwm = pwm;
  if (_debug) Serial.print(_label);
  if (_debug) Serial.print(" Wheel: ");
  if (_debug) Serial.print("analogWrite(");
  if (_debug) Serial.print(_pwmPin);
  if (_debug) Serial.print(", ");
  if (_debug) Serial.print(_pwm);
  if (_debug) Serial.println(")");
  analogWrite(_pwmPin, _pwm);
}

