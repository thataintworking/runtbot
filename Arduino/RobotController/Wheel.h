// Wheel.h
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

class Wheel {

  private:

    String _name;                   // The name of this wheel (left, right, etc)
    int _pwmPin;                    // Arduino pin# connected to the DRV8835 Enable pin to control speed with PWM
    int _dirPin;                    // Arduino pin# connected to the DRV8835 Phase pin to control forward/reverse
    volatile unsigned int _ticks;   // counter for the number of encoder ticks
    unsigned int _tps;              // ticks-per-second measured for the previous second
    unsigned long _nextSecond;      // next millis() value when TPS will be updated
    int _speed;                     // requested TPS, positive for forward, negative for reverse
  
  public:

    Wheel(String name, int pwmPin, int dirPin);
    ~Wheel();

    void tick() { _ticks++; }   // call this method from the interrupt handler for the encoder
    void loop();                // call this method every main loop iteration

    const String& name() { return _name; }
    unsigned int tps() { return _tps; }
    
}

