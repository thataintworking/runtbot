// MotorController
// Author: Ron Smith
// Created: 2018-02-14
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Wire.h>
#include "pitches.h"
#include "Wheel.h"

const int TEST_SPEED = 5;

const int leftMotorDir      = 7;
const int leftMotorPWM      = 6;
const int leftMotorEncoder  = 3;
const int rightMotorDir     = 9;
const int rightMotorPWM     = 8;
const int rightMotorEncoder = 2;
const int testButton        = 53;
const int piezoPin          = 45;

const unsigned long debounceDelay = 300UL;  // milliseconds

unsigned long debounceTime = 0UL;
unsigned long nextCheckTime = 0UL;

int speed = 0;
int speedCount = 0;

boolean motorsOn = false;

Wheel* leftWheel;
Wheel* rightWheel;

const boolean DEBUG = true;

void setup() {
  Serial.begin(9600);

  leftWheel = new Wheel("Left", leftMotorPWM, leftMotorDir, DEBUG);
  rightWheel = new Wheel("Right", rightMotorPWM, rightMotorDir, DEBUG);

  Serial.println("Initializing I2C interface");
  Wire.begin(); // as master
  
  Serial.println("Initializing pin modes");
  pinMode(leftMotorEncoder, INPUT);
  pinMode(rightMotorEncoder, INPUT);
  pinMode(testButton, INPUT_PULLUP);
  
  Serial.println("Initializing interrupts");
  attachInterrupt(digitalPinToInterrupt(leftMotorEncoder), leftEncoderTick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightMotorEncoder), rightEncoderTick, CHANGE);

  Serial.println("Playing start tone");
  playTada();
}


void loop() {
  unsigned long m = millis();
  
  if (!digitalRead(testButton) && (m > debounceDelay)) {
    debounceTime = m + debounceDelay;
    if (motorsOn) stopMotors();
    else {
      nextCheckTime = m + 1000UL;
      Serial.println("Speed,Left,Right");
      speedCount = 0;
      speed = 1;
      startMotors(speed);
    }
  }

  if (motorsOn && m >= nextCheckTime) {
    nextCheckTime = m + 1000UL;
    Serial.print(speed);
    Serial.print(",");
    Serial.print(leftWheel->avgTickTime());
    Serial.print(",");
    Serial.println(rightWheel->avgTickTime());
    if (++speedCount > 30) {
      speedCount = 0;
      if (++speed > Wheel::MAX_FWD_SPEED) {
        stopMotors();
        speed = 0;
      } else {
        leftWheel->setSpeed(speed);
        rightWheel->setSpeed(speed);
      }
    }
  }

//  leftWheel->loop(m);
//  rightWheel->loop(m);
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
  tone(piezoPin, NOTE_C5, 150);
  delay(150);
  tone(piezoPin, NOTE_E5, 150);
  delay(150);
  tone(piezoPin, NOTE_F5, 150);
  delay(200);
  tone(piezoPin, NOTE_G5, 300);
  delay(400);
  tone(piezoPin, NOTE_E5, 150);
  delay(150);
  tone(piezoPin, NOTE_G5, 500);
  delay(500);
  noTone(piezoPin);
}


void playTada() {
  tone(piezoPin, NOTE_C5, 200);
  delay(200);
  tone(piezoPin, NOTE_G5, 500);
  delay(500);
  noTone(piezoPin);
}


void playDaTa() {
  tone(piezoPin, NOTE_G5, 200);
  delay(200);
  tone(piezoPin, NOTE_C5, 500);
  delay(500);
  noTone(piezoPin);
}

