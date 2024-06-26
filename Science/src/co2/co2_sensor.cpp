#include "co2_sensor.h"

Co2Sensor::Co2Sensor(int pin) : pin(pin) { }

void Co2Sensor::setup() {
  pinMode(pin, INPUT);
}

float Co2Sensor::read() { 
  // Old logic:
  float voltageOld = (float) analogRead(pin) * 5 / 1024 / CO2_GAIN;
  if (voltageOld >= CO2_OLD_VOLTAGE_400) return -1;
  float oldValue = pow(10, ((voltageOld - CO2_OLD_1) / CO2_OLD_2 + CO2_OLD_0));

  // New logic:
  // float voltage = (float) analogRead(pin) / 1024 * 5;
  // float exponent = (voltage - CO2_LOWER_VOLTAGE) * log10(CO2_LOWER_PPM / CO2_UPPER_PPM) / CO2_VOLTAGE_DROP / CO2_GAIN;
  // float newValue = CO2_LOWER_PPM * pow(10, exponent);

  return oldValue / 10 * 3.14;
}
