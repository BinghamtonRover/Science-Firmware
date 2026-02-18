#pragma once

#include "src/TMC-Firmware/BURT_TMC.h"

// --------------- Dirt Carousel  ---------------

StepperGeneralConfig dirtCarouselGeneralConfig ={
	name: "dirtCarousel",
	steps_per_unit: microsteps_per_deg * -1,
};

StepperMotorPins dirtCarouselPins = {
	chip_select: 3,
};

InternalRampConfig dirtCarouselConfig = {
	current: 250, 
	speed: 20'000,
	acceleration: 200'000,
};

// LimitSwitch dirtCarouselLimit {
// 	pin: 0, 
// 	triggeredValue: HIGH,
// 	direction: 1,
// 	position: 0,
// };

StepperMotor dirtCarouselMotor(dirtCarouselPins, dirtCarouselConfig);
