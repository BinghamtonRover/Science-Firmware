#ifndef HUM_H
#define HUM_H

#include "DHT.h"

class HumiditySensor {
	public:
		HumiditySensor(int _pin) : pin(_pin), dht(DHT(pin, 11)){}
		float readTemperature();
		void setup();
		float readHumidity();
	
	private:
		int pin;
		DHT dht;
};

#endif