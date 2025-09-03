#ifndef CO2Sensor_H
#define CO2Sensor_H

#include <Arduino.h>
#include <Wire.h>

class Co2Sensor {
  private:
    int address;
    int checkError();

  public:
    Co2Sensor(int addresss);
    void setup();
    float read();
};

#endif
