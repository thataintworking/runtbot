// MotorController
// Author: Ron Smith
// Created: 2018-02-03
// Copyright ©2018 That Ain't Working, All Rights Reserved

#define MOTOR_LF P2_2
#define MOTOR_LB P2_1
#define ENCDR_L  P2_0

#define MOTOR_RF P2_5
#define MOTOR_RB P2_4
#define ENCDR_R  P2_3

boolean lastFlag = false;
unsigned long nextTickCheck;
unsigned long tpsL = 0L;
unsigned long tpsR = 0L;

volatile unsigned long ticksL = 0L;
volatile unsigned long ticksR = 0L;
volatile boolean flag = false;

void encoderLtick() {
  ticksL++;
}

void encoderRtick() {
  ticksR++;
}

void buttonPress() {
  flag = !flag;
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Initializing pin modes");
  pinMode(MOTOR_LF, OUTPUT);
  pinMode(MOTOR_LB, OUTPUT);
  pinMode(ENCDR_L, INPUT_PULLDOWN);
  pinMode(MOTOR_RF, OUTPUT);
  pinMode(MOTOR_RB, OUTPUT);
  pinMode(ENCDR_R, INPUT_PULLDOWN);
  pinMode(PUSH2, INPUT_PULLUP);

  analogWrite(MOTOR_LF, 0);
  analogWrite(MOTOR_LB, 0);
  analogWrite(MOTOR_RF, 0);
  analogWrite(MOTOR_RB, LOW);

  Serial.println("Initializing interrupts");
  attachInterrupt(ENCDR_L, encoderLtick, CHANGE);
  attachInterrupt(ENCDR_R, encoderRtick, CHANGE);
  attachInterrupt(PUSH2, buttonPress, FALLING);

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
  if (flag != lastFlag) {
    lastFlag = flag;
    if (flag) {
      Serial.println("Motors on");
      analogWrite(MOTOR_LF, 150);
      analogWrite(MOTOR_RF, 150);
    } else {
      Serial.println("Motors off");
      analogWrite(MOTOR_LF, 0);
      analogWrite(MOTOR_RF, 0);
    }
  }
}
