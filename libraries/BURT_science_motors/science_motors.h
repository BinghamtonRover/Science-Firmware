#ifndef BURT_science_motors_h
#define BURT_science_motors_h

#include "Arduino.h"

#define DISTANCE_PER_STEP 0.01519  // mm / step
#define DEGREE_PER_STEP 0.375  // deg / step

#define PWM_DELAY 1000  // μs

class StepperMotor { 
	/* Controls basic functionality for all stepper motors */
	protected:
		byte directionPin, stepPin;

	private: 
		byte currentPin1, currentPin2;
		float current;  // the current this motor is operating at

		virtual void writeSteps(int steps);

	public: 
		StepperMotor(byte directionPin, byte stepPin, byte currentPin1, byte currentPin2, float current) : 
			directionPin(directionPin), 
			stepPin(stepPin),
			currentPin1(currentPin1),
			currentPin2(currentPin2),
			current(current) { }

		virtual void setup();
};

class LinearStepperMotor : public StepperMotor {
	/* Controls a stepper motor that moves along a finite path. 

	 These motors are attached to a limit switch that signals the end of the path
	 in one direction. The motor must be stopped from going past the limit switch,
	 and must not go too far in the other direction as well. Calibration is done 
	 by setting the limit switch to x = 0, and tracking position from there.  
	*/
	private:
		byte limitPin;  // the pin on which to read the limit switch
		float limit;  // the maximum distance, in mm, from the limit switch
		float distance = 0;  // how far the motor is from the limit switch

		void writeSteps(int steps) override;
		int distanceToSteps(float distance);  // converts mm to number of steps
		bool isHittingLimit();  // whether the limit switch is currently being hit

	public: 
		LinearStepperMotor(byte directionPin, byte stepPin, byte currentPin1, byte currentPin2, byte limitPin, float limit) : 
			StepperMotor(directionPin, stepPin, currentPin1, currentPin2, 1.5),
			limitPin(limitPin), 
			limit(limit) { }

		void setup() override;
		void calibrate();
		void moveDistance(float distance);
		void setPosition(float position);
};

class RotatingStepperMotor : public StepperMotor {  // needs extended functionality
	private: 
		float angle = 0;  // the current angle, in degrees, of the motor
		int degreesToSteps(float degrees);

		void writeSteps(int steps) override;

	public: 
		RotatingStepperMotor(byte directionPin, byte stepPin, byte currentPin1, byte currentPin2) : 
			StepperMotor(directionPin, stepPin, currentPin1, currentPin2, 0.5) { }

		void calibrate();
		void rotate(float degrees);
		void nextTube();
		void nextSection();
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

class Auger {
	private:
		byte rPWMPin, lPWMPin, rEnablePin, lEnablePin, rDrivePin, lDrivePin;
		int speed;

	public: 
		Auger(byte rPWMPin, byte lPWMPin, byte rEnablePin, byte lEnablePin, byte rDrivePin, byte lDrivePin) : 
			rPWMPin(rPWMPin),
			lPWMPin(lPWMPin),
			rEnablePin(rEnablePin),
			lEnablePin(lEnablePin),
			rDrivePin(rDrivePin),
			lDrivePin(lDrivePin) { }

		void setup();
		void setSpeed(int speed);
		void softBrake();
		void hardBrake();
};

#endif
