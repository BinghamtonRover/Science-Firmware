#pragma once

#include "src/tmc/BURT_TMC.h"

// --------------- Dirt Linear  ---------------

StepperMotorPins dirtLinearPins = {
	enable: 10, 
	chipSelect: 5, 
};

StepperMotorConfig dirtLinearConfig = {
	name: "dirt linear", 
	current: 250, 
	speed: 100'000,
	acceleration: 200'000,
	stepsPerUnit: 1,
};

// LimitSwitch dirtLinearLimit = {
// 	pin: 2, 
// 	triggeredValue: HIGH,
// 	direction: 1,
// 	position: 0,
// 	maxLimit: INFINITY,
// };

StepperMotor dirtLinearMotor(dirtLinearPins, dirtLinearConfig);

// --------------- Scooper Arm  ---------------

StepperMotorPins scoopArmPins = {
	enable: 9, 
	chipSelect: 4, 
};

StepperMotorConfig scoopArmConfig = {
	name: "science linear", 
	current: 400,
	speed: 40'000,
	acceleration: 200'000,
	stepsPerUnit: stepsPerDegree,
};

// LimitSwitch scoopArmLimit = {
// 	pin: 32, 
// 	triggeredValue: HIGH,
// 	direction: -1,
// 	position: 0,
// 	minLimit: -PI / 2,
// };

StepperMotor scoopArmMotor(scoopArmPins, scoopArmConfig);

// --------------- Dirt Carousel  ---------------

StepperMotorPins dirtCarouselPins = {
	enable: 8,
	chipSelect: 3,
};

StepperMotorConfig dirtCarouselConfig = {
	name: "dirtCarousel", 
	current: 250, 
	speed: 10'000,
	acceleration: 200'000,
	stepsPerUnit: stepsPerDegree,
};

// LimitSwitch dirtCarouselLimit {
// 	pin: 0, 
// 	triggeredValue: HIGH,
// 	direction: 1,
// 	position: 0,
// };

StepperMotor dirtCarouselMotor(dirtCarouselPins, dirtCarouselConfig);
