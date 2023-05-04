#pragma once

#include "tmc/BURT_TMC.h"
#include "DcMotor/DcMotor.h"
#include "Vacuum/Vacuum.h"

// -------------------- Pinouts --------------------

/// Vacuum 
#define VACUUM_PIN A7

///Vacuum Servo
#define VACUUM_SERVO_PIN 18

// Pump 1: <LABEL WITH CHEMICAL>
#define PUMP1_PWM_PIN 33 
#define PUMP1_IN1_PIN 35
#define PUMP1_IN2_PIN 34

// Pump 2: <LABEL WITH CHEMICAL>
#define PUMP2_PWM_PIN 37 
#define PUMP2_IN1_PIN 36
#define PUMP2_IN2_PIN 38

// Pump 3: <LABEL WITH CHEMICAL>
#define PUMP3_PWM_PIN 29 
#define PUMP3_IN1_PIN 27
#define PUMP3_IN2_PIN 23

// Pump 4: <LABEL WITH CHEMICAL>
#define PUMP4_PWM_PIN 28
#define PUMP4_IN1_PIN 26
#define PUMP4_IN2_PIN 25

//Stepper Motor Constants (taken from arm code, double check)
#define ACCEL 0x00030d4d //200013 (unclear what units but this is the decimal version)
#define SPEED 0x000327e7 //206823 (unclear what units but this is the decimal version)

// --------------- Motor definitions ---------------

///Stepper1 on PCB (IC3)
StepperMotorPins vacuumLinearPins = {
	chipSelect: 6, 
	enable: 7, 
	limitSwitch: 0, //for now 0, later replace with 24 (bump1)
};
StepperMotorConfig vacuumLinearConfig = {
	name: "vacuumLinear", 
	current: 1500, 
	limitSwitchPosition: 0, 
	minLimit: -INFINITY, 
	maxLimit: INFINITY, 
	isPositive: true, 
	gearboxRatio: 1, 
	motorStepsPerRotation: 200, 
	speed: SPEED, 
	accel: ACCEL, 
};

StepperMotor vacuumLinear(vacuumLinearPins, vacuumLinearConfig);


///Stepper2 on PCB (IC5)
StepperMotorPins dirtLinearPins = {
	chipSelect: 5, 
	enable: 10, 
	limitSwitch: 0, //for now, later replace with 31 (bump2)
};
StepperMotorConfig dirtLinearConfig = {
	name: "dirtLinear", 
	current: 1500, 
	limitSwitchPosition: 0, 
	minLimit: -INFINITY, 
	maxLimit: INFINITY, 
	isPositive: true, 
	gearboxRatio: 1, 
	motorStepsPerRotation: 200, 
	speed: SPEED, 
	accel: ACCEL, 
};

StepperMotor dirtLinear(dirtLinearPins, dirtLinearConfig);


///Stepper3 on PCB (IC4)
StepperMotorPins scienceLinearPins = {
	chipSelect: 4, 
	enable: 9, 
	limitSwitch: 0, //for now, later replace with 32  (bump3)
};
StepperMotorConfig scienceLinearConfig = {
	name: "scienceLinear", 
	current: 1500, 
	limitSwitchPosition: 0,
	minLimit: -INFINITY, 
	maxLimit: INFINITY, 
	isPositive: true, 
	gearboxRatio: 1, 
	motorStepsPerRotation: 200, 
	speed: SPEED, 
	accel: ACCEL,
};

StepperMotor scienceLinear(scienceLinearPins, scienceLinearConfig);

///Stepper4 on PCB (IC2)
StepperMotorPins dirtCarouselPins = {
	chipSelect: 3, 
	enable: 8, 
	limitSwitch: 0, //(bump4) FIND OUT ACTUAL PIN NUMBER
};
StepperMotorConfig dirtCarouselConfig = {
	name: "dirtCarousel", 
	current: 1500, 
	limitSwitchPosition: 0, 
	minLimit: -INFINITY, 
	maxLimit: INFINITY,
	isPositive: true, 
	gearboxRatio: 1, 
	motorStepsPerRotation: 200, 
	speed: SPEED/10, //changed values came from testing
	accel: ACCEL/10, 
};

StepperMotor dirtCarousel(dirtCarouselPins, dirtCarouselConfig);

///Vacuum canister
DirtRelease dirtRelease(VACUUM_SERVO_PIN);

/// Controls the vacuum. 
Vacuum vacuum = Vacuum(VACUUM_PIN);

DCMotor pump1(PUMP1_PWM_PIN, PUMP1_IN1_PIN, PUMP1_IN2_PIN);  ///< Pump 1
DCMotor pump2(PUMP2_PWM_PIN, PUMP2_IN1_PIN, PUMP2_IN2_PIN);  ///< Pump 2
DCMotor pump3(PUMP3_PWM_PIN, PUMP3_IN1_PIN, PUMP3_IN2_PIN);  ///< Pump 3
DCMotor pump4(PUMP4_PWM_PIN, PUMP4_IN1_PIN, PUMP4_IN2_PIN);  ///< Pump 4
