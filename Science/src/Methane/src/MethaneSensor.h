#ifndef METHANE_H
#define METHANE_H

#include <Arduino.h>

#define METHANE_CALIBRATION_VALUE 945

class MethaneSensor {
	private:
		int pin;
	
	public:
		MethaneSensor(int pin);
		void setup();
		float read();
};

#endif
