// i2c_handler.h
// Author: Ron Smith
// Created: 2018-03-31
// Copyright ©2018 That Ain't Working, All Rights Reserved

#ifndef I2C_HANDLER_H_
#define I2C_HANDLER_H_

#include <Arduino.h>
#include "wheel.h"


// Direction Constants
#define DIR_STOP        0
#define DIR_FORWARD     1
#define DIR_REVERSE     2


// Command Constants
#define CMD_STOP_BOTH   0x01
#define CMD_STOP_LEFT   0x02
#define CMD_STOP_RIGHT  0x03
#define CMD_FWD_BOTH    0x04    // next byte is PWM
#define CMD_FWD_LEFT    0x05    // next byte is PWM
#define CMD_FWD_RIGHT   0x06    // next byte is PWM
#define CMD_REV_BOTH    0x07    // next byte is PWM
#define CMD_REV_LEFT    0x08    // next byte is PWM
#define CMD_REV_RIGHT   0x09    // next byte is PWM
#define CMD_PLAY_CHARGE 0xF0
#define CMD_PLAY_TADA   0xF1
#define CMD_PLAY_DATA   0xF2
#define CMD_PLAY_PLUS   0xF3
#define CMD_PLAY_MINUS  0xF4
#define CMD_PLAY_BONK   0xF5

#define CMD_BUF_SIZE    2

struct WheelRegisters {
    byte dir;
    byte pwm;
    unsigned int tps;
}


struct Registers {
    struct WheelRegisters left;
    struct WheelRegisters right;
};


const int REG_SIZE = sizeof(struct Registers);


union RegBuf {
    struct Registers registers;
    byte buffer[REG_SIZE];
}


class _I2C_Slave {

    public:

        _I2C_Slave();

        ~_I2C_Slave() {}

        void begin(Wheel* left, Wheel* right);

        byte leftWheelDir() {
            return _regbuf.registers.left.dir;
        }

        void leftWheelDir(byte dir) {
            _regbuf.registers.left.dir = dir;
        }

        byte leftWheelPWM() {
            return _regbuf.registers.left.pwm;
        }

        void leftWheelPWM(byte pwm) {
            _regbuf.registers.left.pwm = pwm;
        }

        unsigned int leftWheelTPS() {
            return _regbuf.registers.left.tps;
        }

        void leftWheelTPS(unsigned int tps) {
            _regbuf.registers.left.tps = tps;
        }

        byte rightWheelDir() {
            return _regbuf.registers.right.dir;
        }

        void rightWheelDir(byte dir) {
            _regbuf.registers.right.dir = dir;
        }

        byte rightWheelPWM() {
            return _regbuf.registers.right.pwm;
        }

        void rightWheelPWM(byte pwm) {
            _regbuf.registers.left.pwm = pwm;
        }

        unsigned int rightWheelTPS() {
            return _regbuf.registers.right.tps;
        }

        void rightWheelTPS(unsigned int tps) {
            _regbuf.registers.right.tps = tps;
        }

        byte* registerBuf() {
            return _regbuf.buffer;
        }

    private:

        struct RegBuf _regbuf;

        Wheel* _leftWheel;
        Wheel* _rightWheel;

        unsigned long _leftLastTick;
        unsigned long _rightLastTick;

        byte _cmdbuf[CMD_BUF_SIZE];
}

_I2C_Slave I2C_Slave;

#endif // I2C_HANDLER_H_
