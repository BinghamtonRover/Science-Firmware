#pragma once

#include <Arduino.h>

enum MotorCurrent {  // in mA
	HALF_AMP = 500,
	ONE_AMP = 1000,
	ONE_AND_HALF_AMP = 1500,
	TWO_AMPS = 2000,
};

struct MotorPins { 
	int direction; 
	int step; 
	int current1; 
	int current2;
	int limitSwitch; 
};

struct MotorConfig {
	float limit;
	float distancePerStep;  // TODO: change to stepsPerDistance
	float pwmDelay;
	MotorCurrent current;
};

/// Controls a stepper motor that moves along a finite path. 
/// 
/// These motors are attached to a limit switch that signals the end of the 
/// path in one direction. The motor must be stopped from going past the 
/// limit switch, and must not go too far in the other direction as well. 
/// 
/// The limit switch is considered #distance = 0. 
class StepperMotor {
	private: 
		MotorPins pins;
		MotorConfig* config;
		float currentPosition = 0;

		void writeStep();
		void writeSteps(int steps);
		int distanceToSteps(float distance);

	public: 
		StepperMotor(MotorPins pins, MotorConfig* config);

		void setup();
		void calibrate();
		void moveBy(float distance);
		void moveTo(float position);
		bool readLimitSwitch();
};

/// A stepper motor that controls a rotating component. 
/// 
/// Test tubes are located 30 degrees apart. Groups of 4 tubes are referred to 
/// as sections, and are 120 degrees apart. 
class DirtCarousel : public StepperMotor {
	public: 
		using StepperMotor::StepperMotor;  // inherit the constructor from StepperMotor

		void nextTube();  ///< Rotates 30 degrees clockwise to the next tube.
		void nextSection();  ///< Rotates 120 degrees clockwise to the next section.
};
