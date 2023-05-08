#include "pH.h"

pHSensor::pHSensor(int pin) : pin(pin) { }

void pHSensor::setup() {
	sensor.begin();
}

float pHSensor::read(float temperature) {
	float voltage = analogRead(pin) / 1024.0 * 5000;  // read the voltage
	return sensor.readPH(voltage, temperature);  // convert voltage to pH with temperature compensation
}
