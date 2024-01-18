#pragma once 

#include <Arduino.h>
#include <Servo.h>

#include "../tmc/BURT_TMC.h"

/// A DC motor that is controlled by speed and direction, not position. 
/// 
/// A positive velocity indicates clockwise motion, while negative indicates
/// counter-clockwise motion. There are two ways of braking: 
/// 
/// - softBrake(): cuts off power to the motor, allowing it to slow to a stop
/// - hardBrake(): powers the motor to run in-place, causing it to suddenly stop
class Scooper {
	private: 
		/// The TMC motor that rotates the whole scooper 
		StepperMotor stepper; 
		/// The servo that opens and closes the scooper 
		Servo servo; 
		int servoPin;
		
	public: 
		Scooper(StepperMotor stepper, int servoPin) : 
			stepper(stepper),
			servoPin(servoPin) { }

		void setup();

	    /// Rotates the scooper to this amount of degrees (eg, 0 is flat, 90 is straight up)
	    void rotateTo(float degrees);
	    /// Rotates the scooper by this amount of degrees (relative to where it is right now)
	    void rotateBy(float degrees);
	    /// Opens the claw of the scooper
	    void open();
	    /// Closes the claw of the scooper
	    void close();
};
