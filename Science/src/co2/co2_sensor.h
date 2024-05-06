#ifndef CO2Sensor_H
#define CO2Sensor_H

#include <Arduino.h>

/* See the logic at https://www.digikey.com/htmldatasheets/production/2066424/0/0/1/sen0159.pdf 

  They use a linear approximation, but the true graph is a logarithmic curve, so we take the extra
  steps to compute that here. See the math in #Co2Sensor::read. The linear approximation method
  is good, but leads to an error of +/- 50ppm. See the link below for the sensor's sensitivity:
  https://image.dfrobot.com/image/data/SEN0159/CO2b%20MG811%20datasheet.pdf
*/
#define CO2_GAIN          8.5     // The DC gain of the sensor
#define CO2_LOWER_PPM     400     // The sensor has a lower bound of 400ppm
#define CO2_UPPER_PPM     1000    // The sensor has an upper bound of 1,000 ppm
#define CO2_LOWER_VOLTAGE 7  // voltage reading at STANDARD_PPM
#define CO2_VOLTAGE_DROP  0.05    // The voltage drop from LOWER_PPM to UPPER_PPM

class Co2Sensor {
  private: 
    int pin;

  public:
    Co2Sensor(int pin);
    void setup();
    float read();
};

// The old constants. TODO: Remove these in favor of above.
#define CO2_OLD_0 2.602
#define CO2_OLD_1 0.435
#define CO2_OLD_2 -0.1256
#define CO2_OLD_VOLTAGE_400 0.435

#endif
