#pragma once

#include "src/TMC-Firmware/BURT_TMC.h"

// --------------- Dirt Carousel  ---------------

StepperMotorPins dirtCarouselPins = {
	enable: 8,
	chipSelect: 3,
};

StepperMotorConfig dirtCarouselConfig = {
	name: "dirtCarousel", 
	current: 250, 
	speed: 20'000,
	acceleration: 200'000,
	stepsPerUnit: microstepsPerDegree * -1,
};

// LimitSwitch dirtCarouselLimit {
// 	pin: 0, 
// 	triggeredValue: HIGH,
// 	direction: 1,
// 	position: 0,
// };

StepperMotor dirtCarouselMotor(dirtCarouselPins, dirtCarouselConfig);
