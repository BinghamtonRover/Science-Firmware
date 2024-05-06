#pragma once

#include <Arduino.h>
#include <Servo.h>
#include "../tmc/BURT_TMC.h"

class Scooper {
  private: 
    StepperMotor stepper;
    Servo servo;
    int servoPin;

  public: 
    Scooper(StepperMotor stepper, int servoPin);

    void setup();
    void rotateTo(float degrees);
    void rotateBy(float degrees);
    void open();
    void close();
};
