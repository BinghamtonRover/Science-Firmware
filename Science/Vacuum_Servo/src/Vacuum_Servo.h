#ifndef Vacuum_Servo_h
#define Vacuum_Servo_h

#include "Servo.h"
#include "Arduino.h"

class vacuum_servo {
	private:
	int pin;
	
	public:
	void open();
	void close();
	servo myservo;
};

#endif