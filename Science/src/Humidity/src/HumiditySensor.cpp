#include "HumiditySensor.h"

void HumiditySensor::setup()
{
	dht.begin();
}
	
float HumiditySensor::readTemperature() {
	return dht.readTemperature(false, false);
}

float HumiditySensor::readHumidity()
{
	return dht.readHumidity(false);
}
