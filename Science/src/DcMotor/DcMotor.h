#pragma once

#include <Arduino.h>

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
};

/// A digging tool powered by a DC motor. 
/// 
/// This class is similar to #DCMotor in that it has all the same methods, but 
/// they are implemented differently, so the classes are kept separate. There 
/// are also more pins to keep track of. The two classes could be reduced to one
/// or this be made a subclass of #DCMotor in the future. 
class Vacuum {
	private:
		byte rPWMPin;  ///< The right PWM pin, analogous to DCMotor::in1Pin.
		byte lPWMPin; ///< The left PWM pin, analogous to DCMotor::in2Pin.
		byte rEnablePin; ///< The right enable pin. 
		byte lEnablePin; ///< The left enable pin.
		byte rDrivePin;  ///< The right drive pin. Set to LOW. 
		byte lDrivePin;  ///< The left drive pin. Set to LOW.
		int speed;  ///< The current speed, between -100 and 100 inclusive. 

	public: 
		Vacuum(byte rPWMPin, byte lPWMPin, byte rEnablePin, byte lEnablePin, byte rDrivePin, byte lDrivePin) : 
			rPWMPin(rPWMPin),
			lPWMPin(lPWMPin),
			rEnablePin(rEnablePin),
			lEnablePin(lEnablePin),
			rDrivePin(rDrivePin),
			lDrivePin(lDrivePin) { }

		void setup();  ///< Initializes the auger.
		void setSpeed(int speed);  ///< Sets the speed, from -100 to 100 inclusive.
		void softBrake();  ///< Slowly brings the auger to a stop.
		void hardBrake();  ///< Immediately stops the auger. 
};
