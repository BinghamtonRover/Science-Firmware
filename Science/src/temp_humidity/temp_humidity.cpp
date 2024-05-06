#include "temp_humidity.h"

TempHumiditySensor::TempHumiditySensor() : sensor(&Wire, 0x44, 4) { }

void TempHumiditySensor::setup() {
  Serial.print("Initializing temp/humidity sensor...");
  while (sensor.begin() != 0) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Done!");
  if (!sensor.softReset()) {
    Serial.println("Failed to Initialize the chip....");
  }
}

float TempHumiditySensor::getTemperature() {
  return sensor.getTemperatureC();
}

float TempHumiditySensor::getHumidity() {
  return sensor.getHumidityRH();
}
