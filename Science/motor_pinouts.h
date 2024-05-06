#pragma once

#include "src/tmc/BURT_TMC.h"

#define ACCEL 0x00030d4d //200013 (unclear what units but this is the decimal version)
#define SPEED 0x000327e7 //206823 (unclear what units but this is the decimal version)

// --------------- Dirt Linear  ---------------

StepperMotorPins dirtLinearPins = {
	chipSelect: 5, 
	enable: 10, 
};
StepperMotorConfig dirtLinearConfig = {
	name: "dirtLinear", 
	unitName: "Steps",
	current: 250, 
	speed: SPEED / 2, 
	accel: ACCEL, 
	isPositive: true, 
	toUnits: 1.0,
	toSteps: 1.0,
};

LimitSwitch dirtLinearLimit;
// 	pin: 2, 
// 	triggeredValue: HIGH,
// 	direction: 1,
// 	position: 0,
// };

StepperMotor dirtLinearMotor(dirtLinearPins, dirtLinearConfig, dirtLinearLimit);

// --------------- Scooper Arm  ---------------

StepperMotorPins scoopArmPins = {
	chipSelect: 4, 
	enable: 9, 
};
StepperMotorConfig scoopArmConfig = {
	name: "scienceLinear", 
	unitName: "Steps",
	current: 400,
	speed: SPEED / 5, 
	accel: ACCEL,
	isPositive: true, 
	toUnits: 1.0,
	toSteps: 1.0,
};

LimitSwitch scoopArmLimit;
// 	pin: 32, 
// 	triggeredValue: HIGH,
// 	direction: -1,
// 	position: 0,
// };

StepperMotor scoopArmMotor(scoopArmPins, scoopArmConfig, scoopArmLimit);

// --------------- Dirt Carousel  ---------------

StepperMotorPins dirtCarouselPins = {
	chipSelect: 3, 
	enable: 8, 
};
StepperMotorConfig dirtCarouselConfig = {
	name: "dirtCarousel", 
	unitName: "Steps",
	current: 250, 
	speed: SPEED / 20, //changed values came from testing
	accel: ACCEL, 
	isPositive: true, 
	toUnits: 1.0,
	toSteps: 1.0,
};

LimitSwitch dirtCarouselLimit;
// 	pin: 0, 
// 	triggeredValue: HIGH,
// 	direction: 1,
// 	position: 0,
// };

StepperMotor dirtCarouselMotor(dirtCarouselPins, dirtCarouselConfig, dirtCarouselLimit);
