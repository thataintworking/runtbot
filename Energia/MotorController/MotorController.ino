// MotorController
// Author: Ron Smith
// Created: 2018-02-03
// Copyright ©2018 That Ain't Working, All Rights Reserved

#define ENA   P2_0
#define DIRA1 P2_1
#define DIRA2 P2_2
#define ENCA  P1_5

#define ENB   P2_3
#define DIRB1 P2_4
#define DIRB2 P2_5
#define ENCB  P1_4

boolean lastFlag = false;
unsigned long nextTickCheck;
unsigned long tpsA = 0L;
unsigned long tpsB = 0L;

volatile unsigned long ticksA = 0L;
volatile unsigned long ticksB = 0L;
volatile boolean flag = false;

void encoderAtick() {
  ticksA++;
}

void encoderBtick() {
  ticksB++;
}

void buttonPress() {
  flag = !flag;
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Initializing pin modes");
  pinMode(ENA, OUTPUT);
  pinMode(DIRA1, OUTPUT);
  pinMode(DIRA2, OUTPUT);
  pinMode(ENCA, INPUT_PULLDOWN);
  pinMode(ENB, OUTPUT);
  pinMode(DIRB1, OUTPUT);
  pinMode(DIRB2, OUTPUT);
  pinMode(ENCB, INPUT_PULLDOWN);
  pinMode(PUSH2, INPUT_PULLUP);

  digitalWrite(ENA, LOW);
  analogWrite(DIRA1, 0);
  analogWrite(DIRA2, 0);
  digitalWrite(ENB, LOW);
  analogWrite(DIRB1, 0);
  analogWrite(DIRB2, LOW);

  Serial.println("Initializing interrupts");
  attachInterrupt(ENCA, encoderAtick, CHANGE);
  attachInterrupt(ENCB, encoderBtick, CHANGE);
  attachInterrupt(PUSH2, buttonPress, FALLING);

  Serial.println("Initializing tick timer");
  nextTickCheck = millis();
}

void loop() {
  unsigned long m = millis();
  if (m >= nextTickCheck) {
    noInterrupts();
    tpsA = ticksA;
    ticksA = 0L;
    tpsB = ticksB;
    ticksB = 0L;
    interrupts();
    if (tpsA > 0L || tpsB > 0L) {
      Serial.print("TPS A: ");
      Serial.print(tpsA);
      Serial.print(", B: ");
      Serial.println(tpsB);
    }
    nextTickCheck = m + 1000L;
  }
  if (flag != lastFlag) {
    lastFlag = flag;
    if (flag) {
      Serial.println("Motors on");
      analogWrite(DIRA1, 200);
      analogWrite(DIRB1, 200);
      digitalWrite(ENA, HIGH);
      digitalWrite(ENB, HIGH);
    } else {
      Serial.println("Motors off");
      digitalWrite(ENB, LOW);
      digitalWrite(ENA, LOW);
      analogWrite(DIRB1, 0);
      analogWrite(DIRA1, 0);
    }
  }
}
