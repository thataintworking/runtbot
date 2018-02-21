// MotorController
// Author: Ron Smith
// Created: 2018-02-14
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Wire.h>
#include "pitches.h"
#include "Wheel.h"

const int leftMotorDir      = 7;
const int leftMotorPWM      = 6;
const int leftMotorEncoder  = 3;
const int rightMotorDir     = 9;
const int rightMotorPWM     = 8;
const int rightMotorEncoder = 2;
const int testButton        = 53;
const int piezoPin          = 45;

const unsigned long debounceDelay = 300L;  // milliseconds

unsigned long debounceTime = 0L;

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
    else startMotors();
  }

  leftWheel->loop(m);
  rightWheel->loop(m);
}

void startMotors() {
  playCharge();
  Serial.println("Motors on");
  leftWheel->setSpeed(20);
  rightWheel->setSpeed(20);
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

