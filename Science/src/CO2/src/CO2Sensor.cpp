#include "CO2Sensor.h"

void CO2Sensor::setup() {
  pinMode(BOOL_PIN, INPUT);                        //set pin to input
  digitalWrite(BOOL_PIN, HIGH);
}

float CO2Sensor::MGRead() {
  for (int i=0; i<READ_SAMPLE_TIMES; i++) {
    volts += analogRead(CO2_PIN);
    delay(READ_SAMPLE_INTERVAL);
  }
  volts = (volts / READ_SAMPLE_TIMES) * 5 / 1024;
  return volts;
}

float CO2Sensor::getPercentage() {
  if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
    return -1;
  } else {
    return pow(10, ((volts / DC_GAIN) - CO2Curve[1]) / CO2Curve[2] + CO2Curve[0]);
  }
}
