#include "pH.h"

pHSensor::pHSensor(int pin, HumiditySensor humidity) : pin(pin), humidity(humidity) { }

void pHSensor::setup() {
  pH.begin();
}

float pHSensor::sample_pH(){
  float temperature = humidity.readTemperature();           // read your temperature sensor to execute temperature compensation
  float voltage = analogRead(pin) / 1024.0 * 5000;   // read the voltage
  float pHvalue = pH.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
  return pHvalue;
}
