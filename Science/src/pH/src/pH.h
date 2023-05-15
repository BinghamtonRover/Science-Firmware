#ifndef pH_H
#define pH_H

#include <Arduino.h>
#include <EEPROM.h>

#include "DFRobot_PH.h"

class pHSensor {
  private:
    int pin;
    DFRobot_PH sensor;

  public:
    pHSensor(int pin);
    void setup();
    float read(float temperature);
};

#endif
