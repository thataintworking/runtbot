// Wheel.h
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include "Wheel.h"

Wheel::Wheel(String name, int pwmPin, int dirPin) : 
  _name(name), 
  _pwmPin(pwmPin), 
  _dirPin(dirPin), 
  _nextSecond(0L) 
  _ticks(0), 
  _speed(0),
  _tps(0),
  _pwm(0)
{ 
  pinMode(_pwmPin, OUTPUT);
  pinMode(_dirPin, OUTPUT);
  analogWrite(_pwmPin, 0);
  digitalWrite(_dirPin, FORWARD);     
}


Wheel::~Wheel() { 
  analogWrite(_pwmPin, 0);
  digitalWrite(_dirPin, FORWARD);     
}


Wheel::loop() {
  unsigned long m = millis();
  
  if _nextSecond == 0L {
    _nextSecond == m;
  } else if (m >= _nextSecond) {
    _tps = _ticks;
    _ticks = 0;
    _nextSecond = m;
  }

  int absSpeed = abs(_speed);
  
  if (absSpeed > 0) {
    if (absSpeed < _tps)
      
    } else if (absSpeed > _tps) {
      
    }
  }
}

