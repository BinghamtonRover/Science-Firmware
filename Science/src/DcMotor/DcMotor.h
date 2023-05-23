#pragma once

#include <Arduino.h>

#include "../science.pb.h"

/// A DC motor that is controlled by speed and direction, not position. 
/// 
/// A positive velocity indicates clockwise motion, while negative indicates
/// counter-clockwise motion. There are two ways of braking: 
/// 
/// - softBrake(): cuts off power to the motor, allowing it to slow to a stop
/// - hardBrake(): powers the motor to run in-place, causing it to suddenly stop
class DCMotor { 
	private: 
		byte pwmPin;  ///< Set this pin to an analog value representing speed.
		byte in1Pin;  ///< Set this pin to HIGH to indicate clockwise motion.
		byte in2Pin;  ///< Set this pin to LOW to indicate clockwise motion.

	public: 
		DCMotor(byte pwmPin, byte in1Pin, byte in2Pin) : 
			pwmPin(pwmPin),
			in1Pin(in1Pin),
			in2Pin(in2Pin) { }

		void setup();  ///< Initializes the DC motor and its pins.
		void setSpeed(int speed);  ///< Sets the speed, in the interval [-100, 100].
		void softBrake();  ///< Slowly slows the motor to a stop.
		void hardBrake();  ///< Immediately stops the motor. 
		void handleCommand(PumpState state);
};
