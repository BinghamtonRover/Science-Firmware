#pragma once

#include <Arduino.h>

#include "../science.pb.h"
#include "../tmc/BURT_TMC.h"

class Motors {
  public: 
    StepperMotor dirtCarousel;
    
    Motors(StepperMotor dirtCarousel);

    void setup();
    void calibrate();
    void stop();
    void block();
    void update();
    void handleCommand(ScienceCommand command);
};
