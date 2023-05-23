#pragma once

#include <Arduino.h>
#include <Servo.h>

#include "../science.pb.h"

//for dirt collection/vacuum control
class Vacuum {
private:
	int pin;

public: 
	Vacuum(int pin) : pin(pin) { }
	void setup() { pinMode(pin, OUTPUT); disable();  }  // 255=off
	void enable() {
			digitalWrite(pin, LOW);
	}
	void disable() {
		digitalWrite(pin, HIGH);
	}

	void handleCommand(PumpState state) {
		if (state == PumpState_PUMP_ON) enable();
		else if (state == PumpState_PUMP_OFF) disable();
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

		void handleCommand(DirtReleaseState state) {
			if (state == DirtReleaseState_OPEN_DIRT) open();
			else if (state == DirtReleaseState_CLOSE_DIRT) close();
		}
};
