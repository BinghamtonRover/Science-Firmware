#include "CO2Sensor.h"

CO2Sensor::CO2Sensor(int pin) : pin(pin) { }

void CO2Sensor::setup() {
  pinMode(pin, INPUT);
}

float CO2Sensor::read() {
  float voltage = (float) analogRead(pin) * 5 / 1024 / CO2_GAIN;
  if (voltage >= CO2_VOLTAGE_400) return -1;
  // Old logic:
  return pow(10, ((voltage - CO2_OLD_1) / CO2_OLD_2 + CO2_OLD_0));
  // New logic: 
  return (voltage - 400) * CO2_SLOPE + CO2_VOLTAGE_400; 
}
