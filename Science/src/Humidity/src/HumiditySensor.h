#ifndef HUM_H
#define HUM_H

// - Do we need to include DHT here or can we use the Arduino library manager?
// - Can we get rid of Adafruit_Sensor?

#include "DHT.h"

class HumiditySensor {
	private:
		DHT dht;

	public:
		HumiditySensor(int pin);
		void setup();
		float readTemperature();
		float readHumidity();
};

#endif
