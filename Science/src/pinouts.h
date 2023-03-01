#pragma once

#include "StepperMotor/StepperMotor.h"
#include "DcMotor/DcMotor.h"
#include "VacuumServo/VacuumServo.h"
#include "Vacuum/Vacuum.h"

// -------------------- Pinouts --------------------

/// Vacuum 
#define VACUUM_R_PWM_PIN 3
#define VACUUM_L_PWM_PIN 5
#define VACUUM_R_ENABLE_PIN 2
#define VACUUM_L_ENABLE_PIN 4
#define VACUUM_R_DRIVE_PIN 6
#define VACUUM_L_DRIVE_PIN 7

///Vacuum Servo
#define VACUUM_SERVO_PIN 13 //DOUBLE CHECK

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
#define PUMP3_IN2_PIN 30

// Pump 4: <LABEL WITH CHEMICAL>
#define PUMP4_PWM_PIN 28
#define PUMP4_IN1_PIN 26
#define PUMP4_IN2_PIN 25

// --------------- Motor definitions ---------------

/// Stepper1 on the PCB
MotorPins vacuumLinearPins = {
	direction: 39,   // DIR
	step: 40,        // STP 
	current1: 18,    // I1
	current2: 41,    // I2
	limitSwitch: 24, // Bump1
};

MotorConfig vacuumLinearConfig = {
	limit: -370,  // TODO: verify this
	distancePerStep: 1,  // mm / step. TODO: verify this
	pwmDelay: 450,  // or 250
	current: MotorCurrent::ONE_AND_HALF_AMP,  // TODO: Change to 1500
};

/// Moves the vacuum vertically
StepperMotor vacuumLinear(vacuumLinearPins, &vacuumLinearConfig);

/// Stepper2 on the PCB
MotorPins dirtLinearPins = {
	direction: 9,    // DIR
	step: 10,        // STP
	current1: 12,    // I1
	current2: 11,    // I2
	limitSwitch: 31, // Bump2
};

MotorConfig dirtLinearConfig = {
	limit: 35,  // TODO: verify this
	distancePerStep: 1,  // mm / step. TODO: verify this
	pwmDelay: 450,  // by experiment
	current: MotorCurrent::ONE_AND_HALF_AMP,
};

StepperMotor dirtLinear(dirtLinearPins, &dirtLinearConfig);

/// Stepper3 on the PCB
MotorPins scienceLinearPins = {
	direction: 5,    // DIR
	step: 6,         // STP
	current1: 8,     // I1
	current2: 7,     // I2
	limitSwitch: 32, // Bump3
};

MotorConfig scienceLinearConfig = {
	limit: 35,  // TODO: verify this
	distancePerStep: 1,  // mm / step. TODO: verify this
	pwmDelay: 450,  // really nice
	current: MotorCurrent::ONE_AND_HALF_AMP,
};

StepperMotor scienceLinear(scienceLinearPins, &scienceLinearConfig);

/// Stepper4 on the PCB
MotorPins dirtCarouselPins = {	
	direction: 1,    // DIR
	step: 2,         // STP
	current1: 4,     // I1
	current2: 3,     // I2
	limitSwitch: 0,  // Bump4
};

MotorConfig dirtCarouselConfig = {
	limit: 35,  // TODO: verify this
	distancePerStep: 1,  // deg / step
	pwmDelay: 4000,  // not a typo
	current: MotorCurrent::ONE_AND_HALF_AMP,
};

DirtCarousel dirtCarousel(dirtCarouselPins, &dirtCarouselConfig);

///Vacuum canister
DirtRelease dirtRelease(VACUUM_SERVO_PIN);

/// Controls the vacuum. 
Vacuum vacuum = Vacuum(19);

DCMotor pump1(PUMP1_PWM_PIN, PUMP1_IN1_PIN, PUMP1_IN2_PIN);  ///< Pump 1
DCMotor pump2(PUMP2_PWM_PIN, PUMP2_IN1_PIN, PUMP2_IN2_PIN);  ///< Pump 2
DCMotor pump3(PUMP3_PWM_PIN, PUMP3_IN1_PIN, PUMP3_IN2_PIN);  ///< Pump 3
DCMotor pump4(PUMP4_PWM_PIN, PUMP4_IN1_PIN, PUMP4_IN2_PIN);  ///< Pump 4
