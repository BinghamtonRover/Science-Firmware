#pragma once

#include <Arduino.h>

/// A DC motor that is controlled by speed and direction, not position. 
/// 
/// A positive velocity indicates clockwise motion, while negative indicates
/// counter-clockwise motion. There are two ways of braking: 
/// 
/// - softBrake(): cuts off power to the motor, allowing it to slow to a stop
/// - hardBrake(): powers the motor to run in-place, causing it to suddenly stop
class DcMotor { 
	private: 
		int pwmPin;  ///< Set this pin to an analog value representing speed.
		int in1Pin;  ///< Set this pin to HIGH to indicate clockwise motion.
		int in2Pin;  ///< Set this pin to LOW to indicate clockwise motion.

	public: 
		DcMotor(int pwmPin, int in1Pin, int in2Pin);

		void setup();  ///< Initializes the DC motor and its pins.
		void setSpeed(float speed);  ///< Sets the speed, as a percentage (0.0-1.0)
		void softBrake();  ///< Slowly slows the motor to a stop.
		void hardBrake();  ///< Immediately stops the motor. 
};
