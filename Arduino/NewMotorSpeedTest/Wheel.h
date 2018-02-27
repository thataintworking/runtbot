// Wheel.h
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

#ifndef WHEEL_H_
#define WHEEL_H_

#include <Arduino.h>

class Wheel {

  public:

    Wheel(String label, int pwmPin, int dirPin, boolean debug);

    Wheel::Wheel(String label, int pwmPin, int dirPin) : Wheel(label, pwmPin, dirPin, false) {}

    ~Wheel();

    void tick();                                // call this method from the interrupt handler for the encoder

    void loop(unsigned long m);                 // call this method every main loop iteration passing the current millis

    void setSpeed(int s);                       // desired speed 0-20, positive forward, negative reverse

    unsigned int avgTickTime();                 // returns the average tick interval using the values in the tick buffer
    
    const String& label() { return _label; }

    static const int MAX_FWD_SPEED = 20;
    static const int MAX_REV_SPEED = -20;

  private:

    static const int TBSZ = 5;                  // tick buffer size

    volatile unsigned long _tickBuf[TBSZ];      // tick buffer -- array containing the last several tick intervals
    
    unsigned long _lastTickTime;                // the last time the tick() method was called. Used to calculate the tick interval
    unsigned long _nextAdjTime;                 // the millis() value when PWM can be adjusted again after an adjustment is made

    int _tbix;                                  // tick buffer index -- the element where the next tick interval will be stored
    int _pwmPin;                                // arduino pin# connected to the DRV8835 Enable pin to control speed with PWM
    int _dirPin;                                // arduino pin# connected to the DRV8835 Phase pin to control forward/reverse
    int _speed;                                 // requested speed 0-20, positive for forward, negative for reverse
    int _pwm;                                   // the current PWM value

    String _label;                              // the name of this wheel (left, right, etc)

    boolean _debug;

    void setPWM(int pwm);
};

#endif
