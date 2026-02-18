#pragma once

#include "src/TMC-Firmware/BURT_TMC.h"

// --------------- Dirt Linear  ---------------

StepperGeneralConfig linearSliderGeneralConfig ={
    name: "linearSlider",
    steps_per_unit: microsteps_per_deg,
};

StepperMotorPins linearSliderPins = {
	chip_select: 5, 
};

InternalRampConfig linearSliderConfig = {
	current: 250, 
	speed: 100'000,
	acceleration: 200'000,
};

// LimitSwitch dirtLinearLimit = {
// 	pin: 2, 
// 	triggeredValue: HIGH,
// 	direction: 1,
// 	position: 0,
// 	maxLimit: INFINITY,
// };

StepperMotor linearSlider(linearSliderGeneralConfig, linearSliderPins, linearSliderConfig);

// // ---------------  Auger  ---------------

StepperMotorPins augerMotorPins = {
	enable: 8,
};

StepDirConfig augerMotorConfig = {
	current: 250, 
	speed: 20'000,
	acceleration: 200'000,
};

StepperMotor augerMotor(augerMotorPins, augerMotorConfig);
