// MinIMU9.h
// Author: Ron Smith
// Created: 2018-02-18
// Copyright ©2018 That Ain't Working, All Rights Reserved

#ifndef MinIMU9_H_
#define MinIMU9_H_

#include <Arduino.h>
#include <Wire.h>
#include <LSM6.h>
#include <LIS3MDL.h>


struct Xyz {
  int x;
  int y;
  int z;
  
  Xyz(int _x, int _y, int _z): x(_x), y(_y), z(_z) { }
};


struct Readings {
  Xyz a;
  Xyz g;
  Xyz m;

  Readings(Xyz _a, Xyz _g, Xyz _m) : a(_a), g(_g), m(_m) { }
  Readings(int ax, int ay, int az, int gx, int gy, int gz, int mx, int my, int mz) : 
    a(Xyz(ax, ay, az)), g(Xyz(gx, gy, gz)), m(Xyz(mx, my, mz)) { }
};

const int DEFAULT_SENSOR_SIGNS[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};


class MinIMU9 {

  public:

    MinIMU9(const int sensorSigns[]) : _ok(false), _ss(sensorSigns) { }
    MinIMU9() : MinIMU9(DEFAULT_SENSOR_SIGNS) { }

    boolean setup() {
      _ok = _imu.init() && _mag.init();
      if (_ok) {
        _imu.enableDefault();
        _mag.enableDefault();
      }
      return _ok;
    }

    Xyz readAccelerometer() {
      _imu.read();
      return Xyz(_imu.a.x * _ss[0], _imu.a.y * _ss[1], _imu.a.z * _ss[2]);
    }

    Xyz readGyroscope() {
      _imu.read();
      return Xyz(_imu.g.x * _ss[3], _imu.g.y * _ss[4], _imu.g.z * _ss[5]);
    }

    Xyz readMagnetometer() {
      _mag.read();
      return Xyz(_mag.m.x * _ss[6], _mag.m.y * _ss[7], _mag.m.z * _ss[8]);
    }

    Readings readAll() {
      _imu.read();
      _mag.read();
      return Readings(
        _imu.a.x * _ss[0], _imu.a.y * _ss[1], _imu.a.z * _ss[1],
        _imu.g.x * _ss[3], _imu.g.y * _ss[4], _imu.g.z * _ss[5],
        _mag.m.x * _ss[6], _mag.m.y * _ss[7], _mag.m.z * _ss[8]);
    }

    boolean ok() {
      return _ok;
    }

private:

    boolean _ok;
    LSM6 _imu;
    LIS3MDL _mag;
    const int* _ss;
};

#endif

