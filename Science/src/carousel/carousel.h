#pragma once

#include <Arduino.h>
#include <Servo.h>

#include "../TMC-Firmware/BURT_TMC.h"
#include "../science.pb.h"
#include "../motors/motors.h"

class DirtCarousel {
  private: 
    StepperMotor stepper;
    int tubeIndex = 0;
    int sectionIndex = 0;

    void goToSectionStart();
    void fillTube();

  public:
    DirtCarousel(StepperMotor stepper);

    void setup();
    void goToTests();
    void goToPicture();
    void handleCommand(ScienceCommand command);

    void nextTube();
    void prevTube();

    void nextSection();
    void prevSection();
    void goToSection(int section);
    void fillSection();
};
