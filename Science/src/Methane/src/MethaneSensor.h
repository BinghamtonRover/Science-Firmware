#ifndef METHANE_H
#define METHANE_H

#include <Arduino.h>

class MethaneSensor {
	private:
		int pin;
		int calibrationValue;
	
	public:
		MethaneSensor(int pin, int calibrationValue);
		void setup();
		float read();
};

#endif
