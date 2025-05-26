#ifndef TEMP_HUM_SENSOR_H
#define TEMP_HUM_SENSOR_H

#include <Arduino.h>
#include "Wire.h"

class Sensor{
  private:
    uint8_t buf[4] = {0};
    void readReg();
  
  public:
    void setup();
    float getTemp();
    float getHum();
};

#endif