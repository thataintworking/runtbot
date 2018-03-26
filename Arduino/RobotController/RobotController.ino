// RobotController
// Author: Ron Smith
// Created: 2018-02-14
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Wire.h>
#include "config.h"
#include "piezo.h"
#include "wheel.h"
#include "minimu9.h"

unsigned long debounceTime = 0UL;
unsigned long nextSensorTime = 0UL;
unsigned long stopTime = 0UL;
unsigned long reportTime = 0UL;

boolean motorsOn = false;

int speed = 10;

char sbuf[100];

Wheel* leftWheel;
Wheel* rightWheel;

MinIMU9 imu;

void setup() {
  // set timer 1 (pins 9 & 10) divisor to 1 for PWM frequency of 31372.55 Hz
  TCCR1B = TCCR1B & B11111000 | B00000001;

  // don't mess with timer 0, it is used by delay(), millis(), etc.
  // don't mess with timer 2, it is used by tone()

  Serial.begin(9600);

  Wire.begin(); // as master

//  if (!imu.setup())
//    Serial.println("Failed to setup IMU!");

  leftWheel = new Wheel("Left", LEFT_PWM, LEFT_DIR1, LEFT_DIR2, 0, WHEEL_DEBUG);
  rightWheel = new Wheel("Right", RIGHT_PWM, RIGHT_DIR1, RIGHT_DIR2, 0, WHEEL_DEBUG);

  pinMode(LEFT_ENC, INPUT);
  pinMode(RIGHT_ENC, INPUT);
  pinMode(A_BTN, INPUT_PULLUP);
  pinMode(PLUS_BTN, INPUT_PULLUP);
  pinMode(MINUS_BTN, INPUT_PULLUP);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENC), leftEncoderTick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightEncoderTick, CHANGE);

  playTaDa(PIEZO);
}


void loop() {
  unsigned long m = millis();

  if (m > debounceTime) {
    if (!digitalRead(A_BTN)) {
      debounceTime = m + DEBOUNCE_DELAY;
      if (motorsOn) {
      	stopMotors();
      } else {
        startMotors(speed);
        stopTime = millis() + 5000UL;  // 5 seconds
      }
    } else if (!digitalRead(PLUS_BTN)) {
      debounceTime = m + DEBOUNCE_DELAY;
      if (speed < Wheel::MAX_FWD_SPEED) {
        Serial.print("Speed increased to ");
        Serial.println(++speed);
        playPlus(PIEZO);
      } else {
        playBonk(PIEZO);
      }
    } else if (!digitalRead(MINUS_BTN)) {
      debounceTime = m + DEBOUNCE_DELAY;
      if (speed > 1) {
        Serial.print("Speed decreased to ");
        Serial.println(--speed);
        playMinus(PIEZO);
      } else {
        playBonk(PIEZO);
      }
    }
  }

//  if (imu.ok() && m >= nextSensorTime) {
//    nextSensorTime = m + SENSOR_REPORT_FREQ;
//    Readings r = imu.readAll();
//    snprintf(sbuf, sizeof(sbuf), "A: %6d %6d %6d   G: %6d %6d %6d   M: %6d %6d %6d",
//      r.a.x, r.a.y, r.a.z,
//      r.g.x, r.g.y, r.g.z,
//      r.m.x, r.m.y, r.m.z);
//    Serial.println(sbuf);
//  }

//  if (motorsOn) {
//    if (m >= stopTime) {
//      stopMotors();
//    } else {
//      leftWheel->adjust(m);
//      rightWheel->adjust(m);
//      if (m > reportTime) {
//        reportTime = m + 500UL;
//        Serial.print("Tick Time: L=");
//        Serial.print(leftWheel->avgTickTime());
//        Serial.print("  R=");
//        Serial.println(rightWheel->avgTickTime());
//      }
//    }
//  }
}

void startMotors(int s) {
  playCharge(PIEZO);
  Serial.println("Motors on");
  leftWheel->setSpeed(s);
  rightWheel->setSpeed(s);
  motorsOn = true;
}

void stopMotors() {
  Serial.println("Motors off");
  leftWheel->setSpeed(0);
  rightWheel->setSpeed(0);
  motorsOn = false;
  playDaTa(PIEZO);
}

// Interrupt handler for left wheel encoder that counts the ticks
void leftEncoderTick() {
  leftWheel->tick();
}


// Interrupt handler for right wheel encoder that counts the ticks
void rightEncoderTick() {
  rightWheel->tick();
}

