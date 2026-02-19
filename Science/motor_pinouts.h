#pragma once

#include "src/tmc/BURT_TMC.h"

// --------------- Dirt Carousel  ---------------

StepperMotor dirtCarouselMotor(StepperGeneralConfig{
	.name = "dirtCarousel",
	.steps_per_unit = microsteps_per_deg * -1,
},

StepperMotorPins {
	.chip_select = 3,
},

InternalRampConfig {
	.current = 250, 
	.speed = 20'000,
	.acceleration = 200'000,
});
