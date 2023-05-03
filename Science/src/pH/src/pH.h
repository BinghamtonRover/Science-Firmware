#ifndef pH_H
#define pH_H

#include <Arduino.h>
#include <EEPROM.h>
#include "DFRobot_PH.h"
#include "../../Humidity/src/HumiditySensor.h"


class pHSensor {
public:
  pHSensor(int PIN);
  void setup();
  void sample_pH();
  float returnpH();
private:
  int pHPIN;
  DFRobot_PH pH;
  float pHvalue;
  HumiditySensor humsensor = HumiditySensor(15);
};

#endif