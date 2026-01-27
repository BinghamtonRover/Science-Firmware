#pragma once
#include "src/temp_humidity/temp_humidity.h"
#include "src/subsurface/subsurface.h"
#include "src/tmc/BURT_TMC.h"

// -------------------- Pinouts --------------------

const int servo1PIN = 0; // TBD
const int servo2PIN = 0; // TBD

SubSurfaceBox servo1(servo1PIN);
SubSurfaceBox servo2(servo2PIN);

StepperMotorPins stepper1PINS = {0,0}; // TBD
StepperMotorPins stepper2PINS = {0,0}; // TBD

StepperMotorConfig stepper1CONFIG = {
    "null",
    0,
    0,
    0,
    0.0
};

StepperMotorConfig stepper2CONFIG = {
    "null",
    0,
    0,
    0,
    0.0
};

// Limit swithces, if part of the stepper

StepperMotor stepper1(stepper1PINS, stepper1CONFIG);
StepperMotor stepper2(stepper2PINS, stepper2CONFIG);

uint8_t reg;
uint8_t addr;

TempHumiditySensor tempHumidity(reg, addr);