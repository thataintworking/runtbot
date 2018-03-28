// piezo.cpp
// Author: Ron Smith
// Created: 2018-03-17
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Arduino.h>
#include "piezo.h"


void playCharge(int piezo) {
  tone(piezo, NOTE_C5, 150);
  delay(150);
  tone(piezo, NOTE_E5, 150);
  delay(150);
  tone(piezo, NOTE_F5, 150);
  delay(200);
  tone(piezo, NOTE_G5, 300);
  delay(400);
  tone(piezo, NOTE_E5, 150);
  delay(150);
  tone(piezo, NOTE_G5, 500);
  delay(500);
  noTone(piezo);
}


void playTaDa(int piezo) {
  tone(piezo, NOTE_C5, 200);
  delay(200);
  tone(piezo, NOTE_G5, 500);
  delay(500);
  noTone(piezo);
}


void playDaTa(int piezo) {
  tone(piezo, NOTE_G5, 200);
  delay(200);
  tone(piezo, NOTE_C5, 500);
  delay(500);
  noTone(piezo);
}

void playPlus(int piezo) {
  tone(piezo, NOTE_G5, 500);
  delay(500);
  noTone(piezo);
}

void playMinus(int piezo) {
  tone(piezo, NOTE_C5, 500);
  delay(500);
  noTone(piezo);
}

void playBonk(int piezo) {
  tone(piezo, NOTE_C3, 500);
  delay(500);
  noTone(piezo);
}

