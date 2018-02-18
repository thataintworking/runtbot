// Wheel.h
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include "Wheel.h"

Wheel::Wheel(String name, int pwmPin, int dirPin) : 
  _name(name), 
  _pwmPin(pwmPin), 
  _dirPin(dirPin), 
  _ticks(0), 
  _tps(0), 
  _nextSecond(0L) 
  { }


Wheel::~Wheel() { }


Wheel::loop() {
  
}

