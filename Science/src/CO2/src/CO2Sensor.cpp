#include "CO2Sensor.h"

void CO2Sensor::setup() {
  pinMode(CO2_PIN, INPUT);
}

float CO2Sensor::getPercentage() {
  float volts = (float) analogRead(CO2_PIN) * 5 / 1024;
  if ((volts / DC_GAIN) >= ZERO_POINT_VOLTAGE) return -1;
  return pow(10, (((volts / DC_GAIN) - CO2Curve[1]) / CO2Curve[2] + CO2Curve[0]));
}
