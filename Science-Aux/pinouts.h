#pragma once
#include "src/temp_humidity/temp_humidity.h"
#include "src/subsurface/subsurface.h"
#include "src/tmc/BURT_TMC.h"
#include "src/current_sensor/current_sensor.h"

// -------------------- Pinouts --------------------

const int servo1PIN = 10;
const int servo2PIN = 3;

SubSurfaceBox servo1(servo1PIN);
SubSurfaceBox servo2(servo2PIN);

/*struct StepperMotorPins {
  int enable;
  int chipSelect;
};

struct StepperMotorConfig {
  String name;
  int current;
  int speed;
  int acceleration;
  double stepsPerUnit;
};

StepperMotorPins stepper1PINS = {0,15};  // 7, 5
StepperMotorPins stepper2PINS = {0,14};  // 2, 6


StepperMotorConfig stepper1CONFIG = { // arm swivel configs
    "step1",
    2000,
    200'000,
    200'000,
    microstepsPerRadian * 47
};

StepperMotorConfig stepper2CONFIG = {
    "step2",
    2000,
    200'000,
    200'000,
    microstepsPerRadian * 47
};

StepperMotor stepper1(stepper1PINS, stepper1CONFIG);
StepperMotor stepper2(stepper2PINS, stepper2CONFIG);
*/

// TODO: Initialize refactored Steppers

uint8_t reg = 0x00;
uint8_t addr = 0x40;

TempHumiditySensor tempHumidity(reg, addr);

CurrentSensor currentSensor(23);