#pragma once

#include <Arduino.h>
#include "Wire.h"

#define address 0x40 //SDA Ground

class TempHumiditySensor {
  private:
    uint8_t Data[100];
    uint8_t buff[100];
    uint8_t buf[4];
    uint16_t data, data1;
    float temp;
    float hum;
  public:
    TempHumiditySensor();
    void setup();
    float getTemperature();
    float getHumidity();
    uint8_t readReg(uint8_t reg, const void* pBuf, size_t size);
};
