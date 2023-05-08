#ifndef pH_H
#define pH_H

#include <Arduino.h>
#include <EEPROM.h>
#include "DFRobot_PH.h"
#include "../../Humidity/src/HumiditySensor.h"


class pHSensor {
public:
  pHSensor(int pin, HumiditySensor humidity);
  void setup();
  float sample_pH();
private:
  int pin;
  DFRobot_PH pH;
  float pHvalue;
  HumiditySensor humidity;
};

#endif