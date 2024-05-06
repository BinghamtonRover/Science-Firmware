#pragma once

#include <Arduino.h>
#include <Servo.h>

#include "../tmc/BURT_TMC.h"
#include "../science.pb.h"

class DirtCarousel {
  private: 
    StepperMotor stepper;
    Servo dirtRelease;
    int tubeIndex = 0;
    int sectionIndex = 0;
    int dirtReleasePin;

    void goToSectionStart();
    void fillTube();

  public:
    DirtCarousel(StepperMotor stepper, int dirtReleasePin);

    void setup();
    void goHome();
    void goToTests();
    void goToPicture();
    void handleCommand(ScienceCommand command);

    void nextTube();
    void prevTube();

    void nextSection();
    void prevSection();
    void goToSection(int section);
    void fillSection();
        
    void openFunnel();
    void closeFunnel();
};
