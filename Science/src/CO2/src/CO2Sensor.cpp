#include "CO2Sensor.h"

CO2Sensor::CO2Sensor(int input){
  CO2Pin = input;
  ZERO_POINT_VOLTAGE = 0.220; //Must calibrated
  REACTION_VOLTAGE = 0.03; //Must calibrate
}

void CO2Sensor::calibrate(float v400, float v1000) {
  ZERO_POINT_VOLTAGE = v400;
  REACTION_VOLTAGE = v1000;
}

float CO2Sensor::readPPM() {
  float buffer = 0;
  buffer = (ZERO_POINT_VOLTAGE - REACTION_VOLTAGE)/(log10(400) - log10(1000)); // Delta V
  buffer = (raw() - ZERO_POINT_VOLTAGE)/buffer;
  buffer += log10(400);
  return pow(10, buffer);
}

float CO2Sensor::raw() {
  uint8_t i = 0;
  float buffer = 0;
  for(i = 0; i < READ_SAMPLE_TIMES ; i++){
      buffer += analogRead(CO2Pin);
      delay(20); // 20ms
  }
  buffer /= i; // compute the mean
  return map(buffer, 0, 1023, 0, 5);
}
