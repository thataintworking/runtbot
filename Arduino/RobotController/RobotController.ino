// MotorController
// Author: Ron Smith
// Created: 2018-02-14
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Wire.h>
#include "pitches.h"

const int leftMotorDir      = 10;
const int leftMotorPWM      = 9;
const int leftMotorEncoder  = 3;
const int rightMotorDir     = 8;
const int rightMotorPWM     = 7;
const int rightMotorEncoder = 2;
const int testButton        = 53;
const int piezoPin          = 45;

const unsigned long debounceDelay = 300L;  // milliseconds
const unsigned long motorRunDelay = 3000L; // milliseconds

boolean motorsOn = false;


unsigned long debounceTime = 0L;
unsigned long motorStopTime = 0;
unsigned long nextTickCheck;
unsigned long tpsL = 0L;
unsigned long tpsR = 0L;

volatile unsigned long ticksL = 0L;
volatile unsigned long ticksR = 0L;


void setup() {
  Serial.begin(9400);

  Serial.println("Initializing I2C interface");
  Wire.begin(); // as master
  
  Serial.println("Initializing pin modes");
  pinMode(leftMotorDir, OUTPUT);
  pinMode(leftMotorPWM, OUTPUT);
  pinMode(leftMotorEncoder, INPUT);
  
  pinMode(rightMotorDir, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);
  pinMode(rightMotorEncoder, INPUT);
  
  pinMode(testButton, INPUT_PULLUP);
  
  digitalWrite(leftMotorDir, LOW);
  analogWrite(leftMotorPWM, 0);
  
  digitalWrite(rightMotorDir, LOW);
  analogWrite(rightMotorPWM, 0);

  Serial.println("Initializing interrupts");
  attachInterrupt(digitalPinToInterrupt(leftMotorEncoder), leftEncoderTick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightMotorEncoder), rightEncoderTick, CHANGE);

  Serial.println("Initializing tick timer");
  nextTickCheck = millis();

  Serial.println("Playing start tone");
  playTada();
}


void loop() {
  unsigned long m = millis();
  
  if (m >= nextTickCheck) {
    noInterrupts();
    tpsL = ticksL;
    ticksL = 0L;
    tpsR = ticksR;
    ticksR = 0L;
    interrupts();
    if (tpsL > 0L || tpsR > 0L) {
      Serial.print("TPS L: ");
      Serial.print(tpsL);
      Serial.print(", R: ");
      Serial.println(tpsR);
    }
    nextTickCheck = m + 1000L;
  }

  if (!digitalRead(testButton) && (m > debounceDelay)) {
    debounceTime = m + debounceDelay;
    if (motorsOn) stopMotors();
    else {
      startMotors();
      motorStopTime = millis() + motorRunDelay;
    }
  }

  if (motorsOn && m > motorStopTime) stopMotors();
}

void startMotors() {
  playCharge();
  Serial.println("Motors on");
  analogWrite(leftMotorPWM, 200);
  analogWrite(rightMotorPWM, 200);
  motorsOn = true;
}

void stopMotors() {
  Serial.println("Motors off");
  analogWrite(rightMotorPWM, 0);
  analogWrite(leftMotorPWM, 0);
  motorsOn = false;
}

// Interrupt handler for left wheel encoder that counts the ticks
void leftEncoderTick() {
  ticksL++;
}


// Interrupt handler for right wheel encoder that counts the ticks
void rightEncoderTick() {
  ticksR++;
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

