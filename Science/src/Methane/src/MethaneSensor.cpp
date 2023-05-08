#include "MethaneSensor.h"

MethaneSensor::MethaneSensor(int pin, int calibrationValue) : pin(pin), calibrationValue(calibrationValue) { }

void MethaneSensor::setup() {
  pinMode(pin, INPUT);
}

float MethaneSensor::read() {
  float analog = analogRead(pin);
  float voltage = analog * 5 / 1023;
  float reading = (5 - voltage) * 1000 / voltage;  // <-- Document this
  return pow(reading / calibrationValue, -2.95) * 1000;
}
