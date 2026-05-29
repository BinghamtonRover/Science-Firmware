#pragma once

#include <Arduino.h>
#include "Wire.h"

class TempHumiditySensor{
  private:
    uint8_t buf[4] = {0};
    uint8_t reg;
    uint8_t addr;
    bool isReady = false;
    void readReg();
  
  public:
    void setup();
    float getTemperature();
    float getHumidity();
    TempHumiditySensor(uint8_t reg, uint8_t addr);
};