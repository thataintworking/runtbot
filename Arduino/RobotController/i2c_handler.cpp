// i2c_handler.cpp
// Author: Ron Smith
// Created: 2018-03-17
// Copyright ©2018 That Ain't Working, All Rights Reserved

#include <Arduino.h>
#include <Wire.h>
#include "i2c_handler.h"
#include "config.h"
#include "wheel.h"

#define MPS 1000000


unsigned int calcTPS(lastTick, currentTick) {
    if (lastTick > 0) {
        if (currentTick > lastTick) {
           return (MPS / (currentTick - lastTick);
        } else {
            return (MPS / (currentTick + (MPS - lastTick)));
        }
    }
    return 0;
}


void leftWheelEncoderInterrupt() {
    static unsigned long lastTick = 0;
    unsigned long currentTick = micros();
    I2C_Slave.leftWheelTPS(calcTPS(lastTick, currentTick));
    lastTick = currentTick;
}


void rightWheelEncoderInterrupt() {
    static unsigned long lastTick = 0;
    unsigned long currentTick = micros();
    I2C_Slave.rightWheelTPS(calcTPS(lastTick, currentTick));
    lastTick = currentTick;
}


void i2cRequest() {
    Wire.send(I2C_Slave.registerBuf(), REG_SIZE);
}


void i2cReceive(int bytesReceived) {
    for (int a = 0; a < bytesReceived; a++) {
        // TODO circular command queue?
    }
}


_I2C_Slave::_I2C_Slave() : _leftWheel(0), _rightWheel(0), _leftLastTick(0), _rightLastTick(0) {}


void _I2C_Slave::begin(Wheel* left, Wheel* right) {
  _leftWheel = left;
  _rightWheel = right;

  pinMode(LEFT_ENC, INPUT);
  pinMode(RIGHT_ENC, INPUT);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENC), leftWheelEncoderInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightWheelEncoderInterrupt, CHANGE);

  Wire.begin(I2C_ADDR); // as slave
  Wire.onRequest(i2cRequest);
  Wire.onReceive(i2cReceive);
}
