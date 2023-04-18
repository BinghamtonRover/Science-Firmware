#ifndef pH_H
#define pH_H

#include <Arduino.h>
#include <EEPROM.h>
#include "DFRobot_PH.h"


class pHSensor {
public:
  pHSensor(int PIN);
  void setup();
  void sample_pH(float temp);
  float readTemperature(float temp);

private:
  int pHPIN;
  DFRobot_PH pH;
};

#endif