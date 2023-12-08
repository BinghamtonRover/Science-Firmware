#pragma once

#include "tmc/BURT_TMC.h"

// -------------------- Pinouts --------------------

//Stepper Motor Constants (taken from arm code, double check)
#define ACCEL 0x00030d4d //200013 (unclear what units but this is the decimal version)
#define SPEED 0x000327e7 //206823 (unclear what units but this is the decimal version)

// --------------- Motor definitions ---------------

///Stepper1 on PCB (IC3)
StepperMotorPins stepperLinearPins1 = {
	chipSelect: 10, 
	enable: 38, 
};
StepperMotorConfig stepperLinearConfig1 = {
	name: "stepperLinear1", 
	unitName: "Steps",
	current: 1000, 
	speed: SPEED,
	accel: ACCEL, 
	minLimit: -100000, 
	maxLimit: 0, 
	isPositive: true, 
	toUnits: 1.0,
	toSteps: 1.0,
};

LimitSwitch stepperLimit1 = {
	pin: -1, 
	triggeredValue: HIGH,
	direction: 1,
	position: 0,
};

StepperMotor stepperLinear1(stepperLinearPins1, stepperLinearConfig1, stepperLimit1);

///Stepper2 on PCB (IC3)
StepperMotorPins stepperLinearPins2 = {
	chipSelect: 37, 
	enable: 34, 
};
StepperMotorConfig stepperLinearConfig2 = {
	name: "stepperLinear2", 
	unitName: "Steps",
	current: 1000, 
	speed: SPEED,
	accel: ACCEL, 
	minLimit: -100000, 
	maxLimit: 0, 
	isPositive: true, 
	toUnits: 1.0,
	toSteps: 1.0,
};

LimitSwitch stepperLimit2 = {
	pin: -1, 
	triggeredValue: HIGH,
	direction: 1,
	position: 0,
};

StepperMotor stepperLinear2(stepperLinearPins2, stepperLinearConfig2, stepperLimit2);