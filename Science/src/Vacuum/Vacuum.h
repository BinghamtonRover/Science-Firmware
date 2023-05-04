#pragma once

#include <Arduino.h>
#include <Servo.h>

//for dirt collection/vacuum control
class Vacuum {
private:
	int pin;

public: 
	Vacuum(int pin) : pin(pin) { }
	void setup() { pinMode(pin, OUTPUT); setSpeed(0);  }  // 255=off
	void setSpeed(float speed) {
		int signal = -255*speed + 255;
		analogWrite(pin, signal);
	}
};

//for dirt release/servo control
class DirtRelease {
	private: 
		Servo servo;
		int pin;
		int currentDegrees = 0;

	public: 
		DirtRelease(int pin) : pin(pin) { }
		void setup() { 
			servo.attach(pin); 
			servo.write(0);
		}
		void moveBy(int degrees) { 
			currentDegrees += degrees;
			if (currentDegrees >= 90) currentDegrees = 90;
			else if (currentDegrees <= 0) currentDegrees = 0;
			servo.write(currentDegrees);
		}
		void open() {
			servo.write(49); //49 comes from testing
		}
		void close() {
			servo.write(-49); //-49 comes from testing
		}
};
