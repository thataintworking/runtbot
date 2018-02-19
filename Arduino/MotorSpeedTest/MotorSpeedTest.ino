// MotorController
// Author: Ron Smith
// Created: 2018-02-14
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Wire.h>
#include "pitches.h"

const int leftMotorDir      = 7;
const int leftMotorPWM      = 6;
const int leftMotorEncoder  = 3;
const int rightMotorDir     = 9;
const int rightMotorPWM     = 8;
const int rightMotorEncoder = 2;
const int testButton        = 53;
const int piezoPin          = 45;

const unsigned long debounceDelay = 300L;  // milliseconds

const int motorRunTicks = 5;

int motorRunCount = 0;
int motorSpeed = 0;

boolean motorsOn = false;

unsigned long debounceTime = 0L;
unsigned long nextTickCheck;
unsigned long tpsL = 0L;
unsigned long tpsR = 0L;
unsigned long lastTickTimeL = 0L;
unsigned long lastTickTimeR = 0L;

volatile unsigned long ticksL = 0L;
volatile unsigned long ticksR = 0L;
volatile unsigned long tickTimeL = 0L
volatile unsigned long tickTimeR = 0L;

void setup() {
  Serial.begin(9400);

//  Serial.println("Initializing I2C interface");
//  Wire.begin(); // as master
  
//  Serial.println("Initializing pin modes");
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

//  Serial.println("Initializing interrupts");
  attachInterrupt(digitalPinToInterrupt(leftMotorEncoder), leftEncoderTick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightMotorEncoder), rightEncoderTick, CHANGE);

//  Serial.println("Initializing tick timer");
  nextTickCheck = millis();

//  Serial.println("Playing start tone");
  playTaDa();
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
//    if (tpsL > 0L || tpsR > 0L) {
//      Serial.print("TPS L: ");
//      Serial.print(tpsL);
//      Serial.print(", R: ");
//      Serial.println(tpsR);
//    }
    if (motorsOn) {
      Serial.print(motorSpeed);
      Serial.print(',');
      Serial.print(tpsL);
      Serial.print(',');
      Serial.print(tickTimeL);
      Serial.print(',');
      Serial.print(tpsR);
      Serial.print(',');
      Serial.println(tickTimeR);
      motorRunCount++;
      if (motorRunCount > motorRunTicks) {
        motorSpeed -= 10;
        changeMotorSpeed(motorSpeed);
        motorRunCount = 0;
      }
    }
    nextTickCheck = m + 1000L;
  }

  if (!digitalRead(testButton) && (m > debounceDelay)) {
    debounceTime = m + debounceDelay;
    if (motorsOn) {
      stopMotors();
    } else {
      motorRunCount = 0;
      motorSpeed = 250;
      Serial.println("PWM,Left TPS,Left Time,Right TPS,Right Time");
      startMotors(motorSpeed);
    }
  }
}

void startMotors(int s) {
  playCharge();
//  Serial.println("Motors on");
  changeMotorSpeed(s);
}

void changeMotorSpeed(int s) {
  if (s > 0) {
    motorsOn = true;
    analogWrite(leftMotorPWM, s);
    analogWrite(rightMotorPWM, s);
//    Serial.print("Speed ");
//    Serial.println(s);
    tone(piezoPin, NOTE_C4, 300);
    delay(300);
    noTone(piezoPin);
  } else {
    stopMotors();
  }
}

void stopMotors() {
  analogWrite(rightMotorPWM, 0);
  analogWrite(leftMotorPWM, 0);
//  Serial.println("Motors off");
  motorsOn = false;
  playDaTa();
}

// Interrupt handler for left wheel encoder that counts the ticks
void leftEncoderTick() {
  m = millis()
  tickTimeL = m - lastTickTimeL;
  lastTickTimeL = m;
  ticksL++;
}


// Interrupt handler for right wheel encoder that counts the ticks
void rightEncoderTick() {
  m = millis()
  tickTimeR = m - lastTickTimeR;
  lastTickTimeR = m;
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

