// Wheel.h
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

class Wheel {

  public:

    Wheel(String name, int pwmPin, int dirPin);
    ~Wheel();

    void tick() { _ticks++; }   // call this method from the interrupt handler for the encoder
    void loop();                // call this method every main loop iteration

    const String& name() { return _name; }
    unsigned int tps() { return _tps; }

    void set_speed(speed);      // positive forward, negative reverse. 0-255 (but low speeds may not generate 
    
    const int FORWARD = LOW;
    const int REVERSE = HIGH;
    
  private:

    String _name;                   // the name of this wheel (left, right, etc)
    int _pwmPin;                    // arduino pin# connected to the DRV8835 Enable pin to control speed with PWM
    int _dirPin;                    // arduino pin# connected to the DRV8835 Phase pin to control forward/reverse
    volatile unsigned int _ticks;   // counter for the number of encoder ticks
    unsigned long _nextSecond;      // next millis() value when TPS will be updated
    unsigned int _tps;              // ticks-per-second measured for the previous second
    int _speed;                     // requested TPS, positive for forward, negative for reverse
    int _pwm;                       // the current PWM value

    void set_pwm(pwm);
}

