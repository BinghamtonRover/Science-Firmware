#ifndef Vacuum_Servo_h
#define Vacuum_Servo_h

#include "Servo.h"
#include "Arduino.h"

///Vacuum servo: This controls the valve that drops the dirt into the dirt carousel
class VacuumServo {
	private:
		int pin;
		Servo servo;
	
	public:
		VacuumServo(int pin) : pin(pin) {}
		void setup();
		void open();
		void close();
		void write(int angle);
};

#endif