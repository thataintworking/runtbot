// MotorController
// Author: Ron Smith
// Created: 2018-02-14
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Wire.h>
#include "pitches.h"
#include "Wheel.h"
#include "MinIMU9.h"

const boolean WHEEL_DEBUG = false;

const int LEFT_MOTOR_DIR  = 7;
const int LEFT_MOTOR_PWM  = 6;
const int LEFT_MOTOR_ENC  = 3;
const int RIGHT_MOTOR_DIR = 9;
const int RIGHT_MOTOR_PWM = 8;
const int RIGHT_MOTOR_ENC = 2;
const int TEST_BTN        = 53;
const int PLUS_BTN        = A6;
const int MINUS_BTN       = A7;
const int PIEZO           = 45;

const unsigned long DEBOUNCE_DELAY = 300UL;       // milliseconds
const unsigned long SENSOR_REPORT_FREQ = 1000UL;  // milliseconds

unsigned long debounceTime = 0UL;
unsigned long nextSensorTime = 0UL;
unsigned long stopTime = 0UL;
unsigned long reportTime = 0UL;

boolean motorsOn = false;

int speed = 1;

char sbuf[100];

Wheel* leftWheel;
Wheel* rightWheel;

MinIMU9 imu;

void setup() {
  Serial.begin(9600);

  Wire.begin(); // as master

  if (!imu.setup())
    Serial.println("Failed to setup IMU!");

  leftWheel = new Wheel("Left", LEFT_MOTOR_PWM, LEFT_MOTOR_DIR, 0, WHEEL_DEBUG);
  rightWheel = new Wheel("Right", RIGHT_MOTOR_PWM, RIGHT_MOTOR_DIR, 0, WHEEL_DEBUG);

  pinMode(LEFT_MOTOR_ENC, INPUT);
  pinMode(RIGHT_MOTOR_ENC, INPUT);
  pinMode(TEST_BTN, INPUT_PULLUP);
  pinMode(PLUS_BTN, INPUT_PULLUP);
  pinMode(MINUS_BTN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_ENC), leftEncoderTick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_ENC), rightEncoderTick, CHANGE);

  playTaDa();
}


void loop() {
  unsigned long m = millis();

  if (m > debounceTime) {
    if (!digitalRead(TEST_BTN)) {
      debounceTime = m + DEBOUNCE_DELAY;
      if (motorsOn) stopMotors();
      else {
        startMotors(speed);
        stopTime = millis() + 5000UL;  // 5 seconds
      }
    } else if (!digitalRead(PLUS_BTN)) {
      debounceTime = m + DEBOUNCE_DELAY;
      if (speed < Wheel::MAX_FWD_SPEED) {
        Serial.print("Speed increased to ");
        Serial.println(++speed);
        playPlus();
      } else {
        playBonk();
      }
    } else if (!digitalRead(MINUS_BTN)) {
      debounceTime = m + DEBOUNCE_DELAY;
      if (speed > 1) {
        Serial.print("Speed decreased to ");
        Serial.println(--speed);
        playMinus();
      } else {
        playBonk();
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

  if (motorsOn) {
    if (m >= stopTime) {
      stopMotors();
    } else {
      leftWheel->loop(m);
      rightWheel->loop(m);
      if (m > reportTime) {
        reportTime = m + 500UL;
        Serial.print("Tick Time: L=");
        Serial.print(leftWheel->avgTickTime());
        Serial.print("  R=");
        Serial.println(rightWheel->avgTickTime());
      }
    }
  }
}

void startMotors(int s) {
  playCharge();
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
  playDaTa();
}

// Interrupt handler for left wheel encoder that counts the ticks
void leftEncoderTick() {
  leftWheel->tick();
}


// Interrupt handler for right wheel encoder that counts the ticks
void rightEncoderTick() {
  rightWheel->tick();
}


void playCharge() {
  tone(PIEZO, NOTE_C5, 150);
  delay(150);
  tone(PIEZO, NOTE_E5, 150);
  delay(150);
  tone(PIEZO, NOTE_F5, 150);
  delay(200);
  tone(PIEZO, NOTE_G5, 300);
  delay(400);
  tone(PIEZO, NOTE_E5, 150);
  delay(150);
  tone(PIEZO, NOTE_G5, 500);
  delay(500);
  noTone(PIEZO);
}


void playTaDa() {
  tone(PIEZO, NOTE_C5, 200);
  delay(200);
  tone(PIEZO, NOTE_G5, 500);
  delay(500);
  noTone(PIEZO);
}


void playDaTa() {
  tone(PIEZO, NOTE_G5, 200);
  delay(200);
  tone(PIEZO, NOTE_C5, 500);
  delay(500);
  noTone(PIEZO);
}

void playPlus() {
  tone(PIEZO, NOTE_G5, 500);
  delay(500);
  noTone(PIEZO);
}

void playMinus() {
  tone(PIEZO, NOTE_C5, 500);
  delay(500);
  noTone(PIEZO);
}

void playBonk() {
  tone(PIEZO, NOTE_C3, 500);
  delay(500);
  noTone(PIEZO);
}

