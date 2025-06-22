#ifndef  TEMP_HUMIDITY_H
#define  TEMP_HUMIDITY_H

#include <Arduino.h>
#include "Wire.h"

class TempHumiditySensor{
  private:
    uint8_t buf[4] = {0};
    uint8_t reg;
    uint8_t addr;
    void readReg();
  
  public:
    void setup();
    float getTemperature();
    float getHumidity();
    TempHumiditySensor(uint8_t reg, uint8_t addr);
};

#endif