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
const int plusButton        = A6;
const int minusButton       = A7;
const int piezoPin          = 45;

const unsigned long debounceDelay = 300L;  // milliseconds

unsigned long debounceTime = 0L;

boolean motorsOn = false;

int speed = 0;

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
  pinMode(plusButton, INPUT_PULLUP);
  pinMode(minusButton, INPUT_PULLUP);
  
  Serial.println("Initializing interrupts");
  attachInterrupt(digitalPinToInterrupt(leftMotorEncoder), leftEncoderTick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightMotorEncoder), rightEncoderTick, CHANGE);

  Serial.println("Playing start tone");
  playTaDa();
}


void loop() {
  unsigned long m = millis();

  if (m > debounceDelay) {
    if (!digitalRead(testButton)) {
      debounceTime = m + debounceDelay;
      if (motorsOn) stopMotors();
      else {
        speed = 1;
        startMotors(speed);
      }
    } else if (motorsOn) {
      if (!digitalRead(plusButton)) {
        debounceTime = m + debounceDelay;
        if (speed < Wheel::MAX_FWD_SPEED) {
          playPlus();
          speed++;
          leftWheel->setSpeed(speed);
          rightWheel->setSpeed(speed);
        } else {
          playBonk();
        }
      } else if (!digitalRead(minusButton)) {
        debounceTime = m + debounceDelay;
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


void playTaDa() {
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

void playPlus() {
  tone(piezoPin, NOTE_G5, 500);
  delay(500);
  noTone(piezoPin);
}

void playMinus() {
  tone(piezoPin, NOTE_C5, 500);
  delay(500);
  noTone(piezoPin);
}

void playBonk() {
  tone(piezoPin, NOTE_C3, 500);
  delay(500);
  noTone(piezoPin);
}

