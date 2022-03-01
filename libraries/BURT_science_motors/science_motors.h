#ifndef BURT_science_motors_h
#define BURT_science_motors_h

#include "Arduino.h"

#define DISTANCE_PER_STEP 0.01519  // mm / step
#define DEGREE_PER_STEP 0.1125  // deg / step

class StepperMotor {
	private: 
		byte directionPin, stepPin, currentPin1, currentPin2, limitPin;
		float limit;
		int pulseWidthDelay;
		float distance = 0;

		int distanceToSteps(float distance);
		int degreesToSteps(float degrees);
		bool isHittingLimit();

	public: 
		StepperMotor(byte directionPin, byte stepPin, byte currentPin1, byte currentPin2, byte limitPin = 0, float limit = INFINITY, int pulseWidthDelay = 1000) : 
			directionPin(directionPin), 
			stepPin(stepPin), 
			currentPin1(currentPin1), 
			currentPin2(currentPin2),
			limitPin(limitPin), 
			limit(limit), 
			pulseWidthDelay(pulseWidthDelay) { }

		void setup();
		void writeSteps(int steps);
		void moveDistance(float distance);
		void rotate(float degrees);
		void calibrate();
};

class DCMotor { 
	private: 
		byte pwmPin, in1Pin, in2Pin;

	public: 
		DCMotor(byte pwmPin, byte in1Pin, byte in2Pin) : 
			pwmPin(pwmPin),
			in1Pin(in1Pin),
			in2Pin(in2Pin) { }

		void setup();
		void setSpeed(byte speed);
		void softBrake();
		void hardBrake();
};

#endif
