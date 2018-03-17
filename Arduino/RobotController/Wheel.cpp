// Wheel.h
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include "Wheel.h"

const boolean _debug = true;

const unsigned long ADJ_DELAY = 200L;

Wheel::Wheel(String label, int pwmPin, int dirPin1, int dirPin2, int initoff, boolean debug) :
  _lastTickTime(0L),
  _nextAdjTime(0L),
  _tbix(0),
  _pwmPin(pwmPin), 
  _dirPin1(dirPin1),
  _dirPin1(dirPin2),
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
    Serial.print(", dirPin1=");
    Serial.print(dirPin1);
    Serial.print(", dirPin2=");
    Serial.print(dirPin2);
    Serial.print(", initoff=");
    Serial.println(initoff);
  }
  for (int i = 0; i < TBSZ; i++) _tickBuf[i] = 0L;
  pinMode(_pwmPin, OUTPUT);
  pinMode(_dirPin1, OUTPUT);
  pinMode(_dirPin2, OUTPUT);
  analogWrite(_pwmPin, 0);
  digitalWrite(_dirPin1, LOW);
  digitalWrite(_dirPin2, LOW);
}


Wheel::~Wheel() { 
  analogWrite(_pwmPin, 0);
  digitalWrite(_dirPin1, LOW);
  digitalWrite(_dirPin2, LOW);
}


void Wheel::adjust(unsigned long m) {
  if (_speed && m > _nextAdjTime) {
    unsigned int targetTickTime = TTT[abs(_speed)];
    unsigned int tickTime = avgTickTime();
    if (tickTime > 0) {
      if (tickTime < targetTickTime) {
        _pwm--;
        if (_debug) {
          Serial.print(_label);
          Serial.print(" Wheel: ");
          Serial.print("tickTime (");
          Serial.print(tickTime);
          Serial.print(") is less than target (");
          Serial.print(targetTickTime);
          Serial.print(") decreasing PWM to ");
          Serial.println(_pwm);
        }
        setPWM(_pwm);
      } else if (tickTime > targetTickTime) {
        _pwm++;
        if (_debug) {
          Serial.print(_label);
          Serial.print(" Wheel: ");
          Serial.print("tickTime (");
          Serial.print(tickTime);
          Serial.print(") is greater than target (");
          Serial.print(targetTickTime);
          Serial.print(") increasing PWM to ");
          Serial.println(_pwm);
        }
        setPWM(_pwm);
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
    Serial.print("Setting directrion to ");
    Serial.print(_speed < 0 ? "REVERSE" : "FORWARD");
  }
  digitalWrite(_dirPin, _speed < 0 ? REVERSE : FORWARD);
  
  if (_debug) {
    Serial.print(" and speed to ");
    Serial.println(_speed);
  }
  setPWM(INIT_PWM[abs(_speed)] + _initoff);
}


void Wheel::setPWM(int pwm) {
  if (pwm < 0) _pwm = 0;
  else if (pwm > 255) _pwm = 255;
  else _pwm = pwm;
  analogWrite(_pwmPin, _pwm);
}

