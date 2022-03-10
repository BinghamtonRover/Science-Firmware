#ifndef BURT_science_motors_h
#define BURT_science_motors_h

#include "Arduino.h"

#define DISTANCE_PER_STEP 0.01519  ///< Millimeters per step.
#define DEGREE_PER_STEP 0.375  ///< Degrees per step.

#define PWM_DELAY 1000  ///< Delay, in μs, between steps. 

/// Defines basic functionality for all stepper motors
class StepperMotor { 
	protected:
		byte directionPin;  ///< Indicates clockwise motion when HIGH.
		byte stepPin;  ///< Moves one step when pulsed to HIGH. 
		void writeStep();  ///< Moves the motor by one step by pulsing #stepPin.

	private: 
		byte currentPin1;  ///< Controls the current intake (see #setup).
		byte currentPin2;  ///< Controls the current intake (see #setup).
		float current;  ///< The current this motor is operating at, in mA.

		virtual void writeSteps(int steps);  ///< Moves the motor a number of steps.

	public: 
		StepperMotor(byte directionPin, byte stepPin, byte currentPin1, byte currentPin2, float current) : 
			directionPin(directionPin), 
			stepPin(stepPin),
			currentPin1(currentPin1),
			currentPin2(currentPin2),
			current(current) { }
		
		virtual void setup();  ///< Initializes and configures the motor.
};

/// Controls a stepper motor that moves along a finite path. 
/// 
/// These motors are attached to a limit switch that signals the end of the 
/// path in one direction. The motor must be stopped from going past the 
/// limit switch, and must not go too far in the other direction as well. 
/// 
/// The limit switch is considered #distance = 0. 
class LinearStepperMotor : public StepperMotor {
	private:
		byte limitPin;  ///< The pin on which to read the limit switch
		float limit;  ///< The maximum distance, in mm, from the limit switch
		float distance = 0;  ///< How far the motor is from the limit switch

		void writeSteps(int steps) override;  ///< Moves without passing limits.
		int distanceToSteps(float distance);  ///< Converts mm to steps.
		bool readLimitSwitch();  ///< Checks if the limit switch is pressed.

	public: 
		LinearStepperMotor(byte directionPin, byte stepPin, byte currentPin1, byte currentPin2, byte limitPin, float limit) : 
			StepperMotor(directionPin, stepPin, currentPin1, currentPin2, 1.5),
			limitPin(limitPin), 
			limit(limit) { }

		void setup() override;  ///< Initializes the motor and limit switch.
		void calibrate();  ///< Moves to the limit switch and resets #distance.

		/// Moves by the specified distance (in mm).
		/// 
		/// The motor will refuse to move past either limit. That is, if the switch 
		/// is being pressed, the motor will not move toward the limit switch. If 
		/// the motor has reached its further limit (specified by #limit), it will 
		/// not move in that direction.
		void moveDistance(float distance);

		/// Moves to a specified position (in mm) from the limit switch.
		/// 
		/// Like #moveDistance, this function will not allow the motor to pass its 
		/// limit in either direction. 
		void setPosition(float position);
};

/// A stepper motor that controls a rotating component. 
/// 
/// Like #LinearStepperMotor, this class keeps track of its current state, in
/// terms of its current rotation. Unlike #LinearStepperMotor, this class does 
/// not have limits to watch for. Rotating stepper motors can rotate by a given 
/// amount of degrees, or to a known rotation. 
/// 
/// Test tubes are located 30 degrees apart. Groups of 4 tubes are referred to 
/// as sections, and are 120 degrees apart. 
class RotatingStepperMotor : public StepperMotor {
	private: 
		float angle = 0;  ///< the current angle (in degrees) of the motor.

		int degreesToSteps(float degrees);  ///< Converts degrees to steps.
		void writeSteps(int steps) override;  ///< Moves while recording rotation.

	public: 
		RotatingStepperMotor(byte directionPin, byte stepPin, byte currentPin1, byte currentPin2) : 
			StepperMotor(directionPin, stepPin, currentPin1, currentPin2, 0.5) { }

		void calibrate();  ///< Resets #angle to 0. 
		void rotate(float degrees);  ///< Rotates by a number of degrees.
		void setAngle(float newAngle);  ///< Rotates to the desired angle.
		void nextTube();  ///< Rotates 30 degrees clockwise, to the next tube.
		void nextSection();  ///< Rotates 120 degrees clockwise to the next section.
};

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
		void setSpeed(int speed);  ///< Sets the speed, from -255 to 255 inclusive.
		void softBrake();  ///< Slowly slows the motor to a stop.
		void hardBrake();  ///< Immediately stops the motor. 
};

/// A digging tool powered by a DC motor. 
/// 
/// This class is similar to #DCMotor in that it has all the same methods, but 
/// they are implemented differently, so the classes are kept separate. There 
/// are also more pins to keep track of. The two classes could be reduced to one
/// or this be made a subclass of #DCMotor in the future. 
class Auger {
	private:
		byte rPWMPin;  ///< The right PWM pin, analogous to DCMotor::in1Pin.
		byte lPWMPin; ///< The left PWM pin, analogous to DCMotor::in2Pin.
		byte rEnablePin; ///< The right enable pin. 
		byte lEnablePin; ///< The left enable pin.
		byte rDrivePin;  ///< The right drive pin. Set to LOW. 
		byte lDrivePin;  ///< The left drive pin. Set to LOW
		int speed;  ///< The current speed, between -255 and 255 inclusive. 

	public: 
		Auger(byte rPWMPin, byte lPWMPin, byte rEnablePin, byte lEnablePin, byte rDrivePin, byte lDrivePin) : 
			rPWMPin(rPWMPin),
			lPWMPin(lPWMPin),
			rEnablePin(rEnablePin),
			lEnablePin(lEnablePin),
			rDrivePin(rDrivePin),
			lDrivePin(lDrivePin) { }

		void setup();  ///< Initializes the auger
		void setSpeed(int speed);  ///< Sets the speed, from -255 to 255 inclusive.
		void softBrake();  ///< Slowly slows the auger to a stop.
		void hardBrake();  ///< Immediately stops the auger. 
};

#endif
