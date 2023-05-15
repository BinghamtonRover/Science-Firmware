#pragma once

#include <Arduino.h>
#include <Servo.h>

#define DIRT_RELEASE_OPEN_ANGLE 49
#define DIRT_RELEASE_CLOSE_ANGLE -49  // <-- Make this positive

class Vacuum {
	private:
		int pin;

	public: 
		Vacuum(int pin);
		void setup();
		void enable();
		void disable();
};

//for dirt release/servo control
class DirtRelease {
	private: 
		Servo servo;
		int pin;
		int angle = 0;

	public: 
		DirtRelease(int pin);
		void setup(); 
		void moveBy(int degrees);
		void open();
		void close();
};
