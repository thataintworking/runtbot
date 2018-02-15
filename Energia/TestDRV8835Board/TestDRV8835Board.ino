// MotorController
// Author: Ron Smith
// Created: 2018-02-03
// Copyright ©2018 That Ain't Working, All Rights Reserved

#define MOTOR_DIR_L P2_2
#define MOTOR_PWM_L P2_1
#define MOTOR_ENC_L P2_0

#define MOTOR_DIR_R P2_3
#define MOTOR_PWM_R P2_4
#define MOTOR_ENC_R P2_5

const int debounceDelay = 100;

unsigned long lastDebounceTime = 0L;
unsigned long nextTickCheck;
unsigned long tpsL = 0L;
unsigned long tpsR = 0L;

boolean motorsOn = false;

volatile unsigned long ticksL = 0L;
volatile unsigned long ticksR = 0L;
volatile boolean buttonFlag = false;

void encoderLtick() {
  ticksL++;
}

void encoderRtick() {
  ticksR++;
}

void buttonPress() {
  buttonFlag = true;
}

void setup() {
  Serial.begin(9600);
  
  Serial.println("Initializing pin modes");
  pinMode(MOTOR_DIR_L, OUTPUT);
  pinMode(MOTOR_PWM_L, OUTPUT);
  pinMode(MOTOR_ENC_L, INPUT_PULLDOWN);
  
  pinMode(MOTOR_DIR_R, OUTPUT);
  pinMode(MOTOR_PWM_R, OUTPUT);
  pinMode(MOTOR_ENC_R, INPUT_PULLDOWN);
  
  pinMode(PUSH2, INPUT_PULLUP);

  digitalWrite(MOTOR_DIR_L, LOW);
  analogWrite(MOTOR_PWM_L, 0);
  
  digitalWrite(MOTOR_DIR_R, LOW);
  analogWrite(MOTOR_PWM_R, 0);
  
  Serial.println("Initializing interrupts");
  attachInterrupt(MOTOR_ENC_L, encoderLtick, CHANGE);
  attachInterrupt(MOTOR_ENC_R, encoderRtick, CHANGE);
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
  
  if (buttonFlag) {
    buttonFlag = false;
    if (m - lastDebounceTime > debounceDelay) {
      lastDebounceTime = m;
      motorsOn = !motorsOn;
      if (motorsOn) {
        Serial.println("Motors on");
        analogWrite(MOTOR_PWM_L, 150);
        analogWrite(MOTOR_PWM_R, 150);
      } else {
        Serial.println("Motors off");
        analogWrite(MOTOR_PWM_R, 0);
        analogWrite(MOTOR_PWM_L, 0);
      }
    }
  }
}
