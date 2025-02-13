#ifndef CO2Sensor_H
#define CO2Sensor_H

#include <Arduino.h>
#include <Wire.h>

class Co2Sensor {
  private:
    int address;
    bool hasError();

  public:
    Co2Sensor(int addresss);
    void setup();
    int read();
};

#endif
