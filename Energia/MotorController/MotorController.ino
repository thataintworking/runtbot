// MotorController
// Author: Ron Smith
// Created: 2018-02-03
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Wire.h>

#define MOTOR_DIR_L P2_2
#define MOTOR_PWM_L P2_1
#define MOTOR_ENC_L P2_0

#define MOTOR_DIR_R P2_3
#define MOTOR_PWM_R P2_4
#define MOTOR_ENC_R P2_5

unsigned long nextTickCheck;
unsigned long tpsL = 0L;
unsigned long tpsR = 0L;

volatile unsigned long ticksL = 0L;
volatile unsigned long ticksR = 0L;


void setup() {
  Serial.begin(9600);

  Serial.println("Initializing I2C interface");
  Wire.begin(0x25);
  Wire.onReceive(i2cReceive);
  Wire.onRequest(i2cRequest);
  
  Serial.println("Initializing pin modes");
  pinMode(MOTOR_DIR_L, OUTPUT);
  pinMode(MOTOR_PWM_L, OUTPUT);
  pinMode(MOTOR_ENC_L, INPUT_PULLDOWN);
  
  pinMode(MOTOR_DIR_R, OUTPUT);
  pinMode(MOTOR_PWM_R, OUTPUT);
  pinMode(MOTOR_ENC_R, INPUT_PULLDOWN);
  
  digitalWrite(MOTOR_DIR_L, LOW);
  analogWrite(MOTOR_PWM_L, 0);
  
  digitalWrite(MOTOR_DIR_R, LOW);
  analogWrite(MOTOR_PWM_R, 0);
  
  Serial.println("Initializing interrupts");
  attachInterrupt(MOTOR_ENC_L, encoderLtick, CHANGE);
  attachInterrupt(MOTOR_ENC_R, encoderRtick, CHANGE);

  Serial.println("Initializing tick timer");
  nextTickCheck = millis();
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
}


// Interrupt handler for left wheel encoder that counts the ticks
void encoderLtick() {
  ticksL++;
}


// Interrupt handler for right wheel encoder that counts the ticks
void encoderRtick() {
  ticksR++;
}


// I2C callback for receive
void i2cReceive(int sz) {
  Serial.print("I2C Received: ");
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
}


// I2C callback for request
void i2cRequest() {
  Serial.println("I2C responding to request");
  Wire.write("Hi, I am MSP430");
}

