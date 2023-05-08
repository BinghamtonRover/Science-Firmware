#ifndef CO2Sensor_H
#define CO2Sensor_H

#include <Arduino.h>

// Can we redo this logic with more documentation? I understood the curve parts but it didn't 
// explain all the constants or why we needed a curve. Let's redo this in terms of two points

///< The DC of the amplifier
#define CO2_GAIN 8.5

///< The voltage at 400 ppm
#define CO2_VOLTAGE_400 0.435

///< The voltage at 1,000 ppm
#define CO2_VOLTAGE_1000 0.430  // <-- Is this correct?

///< The slope of the curve of the voltage of the CO2 sensor
#define CO2_SLOPE ( (CO2_VOLTAGE_1000 - CO2_VOLTAGE_400) / (1000 - 400) )

class CO2Sensor {
  private: 
    int pin;

  public:
    CO2Sensor(int pin);
    void setup();
    float read();
};

// The old constants. TODO: Remove these in favor of above.
#define CO2_OLD_0 2.602
#define CO2_OLD_1 0.435
#define CO2_OLD_2 0.050 

#endif
