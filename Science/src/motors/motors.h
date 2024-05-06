#pragma once

#include <Arduino.h>

#include "../tmc/BURT_TMC.h"

class Motors {
  public: 
    StepperMotor scooperArm;
    StepperMotor dirtCarousel;
    StepperMotor dirtLinear;
    
    Motors(StepperMotor scooperArm, StepperMotor dirtCarousel, StepperMotor dirtLinear);

    void setup();
    void calibrate();
    void stop();
    void block();
    void update();
};
