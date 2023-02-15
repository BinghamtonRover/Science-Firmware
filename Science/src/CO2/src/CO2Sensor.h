#ifndef CO2Sensor_H
#define CO2Sensor_H

#include <Arduino.h>

#define READ_SAMPLE_TIMES 20

class CO2Sensor{
  public:
    CO2Sensor(int input);
    void setup();
    void calibrate(float v400, float v1000);
    float readPPM();
    float raw();
  private:
    int CO2Pin;
    int ZERO_POINT_VOLTAGE;
    int REACTION_VOLTAGE;
};


#endif
