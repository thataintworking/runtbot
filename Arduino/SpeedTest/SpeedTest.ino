// MotorController
// Author: Ron Smith
// Created: 2018-02-14
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Wire.h>
#include "pitches.h"
#include "Wheel.h"
#include "MinIMU9.h"

const boolean WHEEL_DEBUG = true;

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

boolean motorsOn = false;

int speed = 0;

char sbuf[100];

Wheel* leftWheel;
Wheel* rightWheel;

MinIMU9 imu;

void setup() {
  Serial.begin(9600);

  Wire.begin(); // as master

  if (!imu.setup())
    Serial.println("Failed to setup IMU!");

  leftWheel = new Wheel("Left", LEFT_MOTOR_PWM, LEFT_MOTOR_DIR, WHEEL_DEBUG);
  rightWheel = new Wheel("Right", RIGHT_MOTOR_PWM, RIGHT_MOTOR_DIR, WHEEL_DEBUG);

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

  if (m > DEBOUNCE_DELAY) {
    if (!digitalRead(TEST_BTN)) {
      debounceTime = m + DEBOUNCE_DELAY;
      if (motorsOn) stopMotors();
      else {
        speed = 1;
        startMotors(speed);
      }
    } else if (motorsOn) {
      if (!digitalRead(PLUS_BTN)) {
        debounceTime = m + DEBOUNCE_DELAY;
        if (speed < Wheel::MAX_FWD_SPEED) {
          playPlus();
          speed++;
          leftWheel->setSpeed(speed);
          rightWheel->setSpeed(speed);
        } else {
          playBonk();
        }
      } else if (!digitalRead(MINUS_BTN)) {
        debounceTime = m + DEBOUNCE_DELAY;
        if (speed > 1) {
          playMinus();
          speed--;
          leftWheel->setSpeed(speed);
          rightWheel->setSpeed(speed);
        } else {
          playBonk();
        }
      }
    }
  }

  if (imu.ok() && m >= nextSensorTime) {
    nextSensorTime = m + SENSOR_REPORT_FREQ;
    Readings r = imu.readAll();
    snprintf(sbuf, sizeof(sbuf), "A: %6d %6d %6d   G: %6d %6d %6d   M: %6d %6d %6d",
      r.a.x, r.a.y, r.a.z,
      r.g.x, r.g.y, r.g.z,
      r.m.x, r.m.y, r.m.z);
    Serial.println(sbuf);
  }

  leftWheel->loop(m);
  rightWheel->loop(m);
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

