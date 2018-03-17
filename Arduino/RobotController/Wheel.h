// Wheel.h
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

#ifndef WHEEL_H_
#define WHEEL_H_

#include <Arduino.h>

class Wheel {

  public:

    Wheel(String label, int pwmPin, int dirPin1, int dirPin2, int initoff, boolean debug);

    Wheel(String label, int pwmPin, int dirPin1, dirPin2) : Wheel(label, pwmPin, dirPin1, dirPin2, 0, false) {}

    ~Wheel();

    void tick();                                // call this method from the interrupt handler for the encoder

    void adjust(int a);                         // used by the WheelMonitor to make minor adjustments to the PWM

    void setSpeed(int s);                       // desired speed 0-20, positive forward, negative reverse

    unsigned int avgTickTime();                 // returns the average tick interval using the values in the tick buffer

    void setLabel(const String& label) { _label = label; }
    const String& getLabel() { return _label; }

    void setDebug(boolean debug) { _debug = debug; }
    boolean getDebug() { return _debug; }

    void setInitOffset(int initoff) { _initoff = initoff; }
    int getInitOffset() { return _initoff; }

    static const int MAX_FWD_SPEED = 20;
    static const int MAX_REV_SPEED = -20;

  private:

    static const int TBSZ = 5;                  // tick buffer size

    volatile unsigned long _tickBuf[TBSZ];      // tick buffer -- array containing the last several tick intervals
    
    unsigned long _lastTickTime;                // the last time the tick() method was called. Used to calculate the tick interval
    unsigned long _nextAdjTime;                 // the millis() value when PWM can be adjusted again after an adjustment is made

    int _tbix;                                  // tick buffer index -- the element where the next tick interval will be stored
    int _pwmPin;                                // arduino pin# connected to the TB6612FNG Motor Controller PWM pin to control speed
    int _dirPin1;                               // arduino pin# connected to the TB6612FNG Motor Controller DIR1 pin
    int _dirPin2;                               // arduino pin# connected to the TB6612FNG Motor Controller DIR2 pin
    int _speed;                                 // requested speed 0-25, positive for forward, negative for reverse
    int _pwm;                                   // the current PWM value
    int _initoff;                               // manual PWM adjustment to help account for differences in the motors

    String _label;                              // the name of this wheel (left, right, etc)

    boolean _debug;

    void setPWM(int pwm);
};

#endif
