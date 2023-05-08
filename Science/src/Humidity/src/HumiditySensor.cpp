#include "HumiditySensor.h"

HumiditySensor::HumiditySensor(int pin) : dht(DHT(pin, 11)) { }  // <-- Why 11?

void HumiditySensor::setup() {
	dht.begin();
}
	
float HumiditySensor::readTemperature() {
	return dht.readTemperature(false, false);
}

float HumiditySensor::readHumidity() {
	return dht.readHumidity(false);
}
