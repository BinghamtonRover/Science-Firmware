#ifndef  TEMP_HUMIDITY_H
#define  TEMP_HUMIDITY_H

#include <Arduino.h>
#include "Wire.h"

class TempHumiditySensor{
  private:
    uint8_t buf[4] = {0};
    void readReg();
  
  public:
    void setup();
    float getTemperature();
    float getHumidity();
};

#endif