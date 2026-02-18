#pragma once
#include "src/auger_servo/auger_servo.h"
#include "src/temp_humidity/temp_humidity.h"
#include "src/subsurface/subsurface.h"
#include "src/TMC-Firmware/BURT_TMC.h"
#include "src/current_sensor/current_sensor.h"

// -------------------- Pinouts --------------------

const int upper_servo_pin = 10;
const int lower_servo_pin = 3;

AugerServo upper_servo(upper_servo_pin);
AugerServo lower_servo(lower_servo_pin);

uint8_t reg = 0x00;
uint8_t addr = 0x40;

TempHumiditySensor tempHumidity(reg, addr);

CurrentSensor currentSensor(23);

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