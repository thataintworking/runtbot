// driver.h
// Author: Ron Smith
// Created: 2018-03-23
// Copyright ©2018 That Ain't Working, All Rights Reserved

#ifndef DRIVER_H_
#define DRIVER_H_

#include <Arduino.h>
#include "wheel.h"

class Driver {

    public:

        Driver(Wheel* leftWheel, Wheel* rightWheel);

        ~Driver();


}

#endif // DRIVER_H_
