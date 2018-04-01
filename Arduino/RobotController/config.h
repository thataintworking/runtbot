// config.h
// Author: Ron Smith
// Created: 2018-03-23
// Copyright ©2018 That Ain't Working, All Rights Reserved

#ifndef CONFIG_H_
#define CONFIG_H_

#define I2C_ADDR    0x22

// NOTE: Pins are set for Arduino UNO, NANO, or similar
#define LEFT_DIR1   14  // A0
#define LEFT_DIR2   15  // A1
#define LEFT_PWM    9   // OC1A
#define LEFT_ENC    2   // INT0
#define RIGHT_DIR1  16  // A2
#define RIGHT_DIR2  17  // A3
#define RIGHT_PWM   10  // OC1B
#define RIGHT_ENC   3   // INT1
#define A_BTN       5
#define B_BTN       6
#define B_BTN       7
#define PLUS_BTN    11
#define MINUS_BTN   12
#define PIEZO       4
#define LED         13

#define DEBOUNCE_DELAY      300UL  // milliseconds
#define SENSOR_REPORT_FREQ 1000UL  // milliseconds

#define WHEEL_DEBUG     true

#endif // CONFIG_H_
