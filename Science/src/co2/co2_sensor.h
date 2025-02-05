#ifndef CO2Sensor_H
#define CO2Sensor_H

#include <Arduino.h>
#include <Wire.h>




/* See the logic at https://www.digikey.com/htmldatasheets/production/2066424/0/0/1/sen0159.pdf 

  They use a linear approximation, but the true graph is a logarithmic curve, so we take the extra
  steps to compute that here. See the math in #Co2Sensor::read. The linear approximation method
  is good, but leads to an error of +/- 50ppm. See the link below for the sensor's sensitivity:
  https://image.dfrobot.com/image/data/SEN0159/CO2b%20MG811%20datasheet.pdf
*/

class Co2Sensor {
  private: 
    int address;

  public:
    Co2Sensor(int addresss);
    void setup();
    float read();
};


#endif
