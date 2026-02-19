#pragma once
#include "src/auger_servo/auger_servo.h"
#include "src/temp_humidity/temp_humidity.h"
#include "src/subsurface/subsurface.h"
#include "src/tmc/BURT_TMC.h"
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

StepperMotor linearSlider(
  StepperGeneralConfig{
    .name ="linearSlider",
    .steps_per_unit = microsteps_per_deg,
},

StepperMotorPins {
	.chip_select = 15, 
},

InternalRampConfig{
	.current = 250, 
	.speed = 100'000,
	.acceleration = 200'000,
});


// // ---------------  Auger  ---------------

StepperMotor augerMotor(
  StepperGeneralConfig{
    .name ="augerMotor",
    .steps_per_unit = microsteps_per_deg,
},

StepperMotorPins {
	.chip_select = 14,
  .step_pin = 6,
  .dir_pin = 2
},

StepDirConfig {
    .gear_ratio = 24,
    .double_edge = true,
    .run_current_scale = 16,
    .hold_current_scale = 4,
    .ihold_delay_scale = 8,
    .invert_dir = false,
    .stealth_chop_en = false,
    .spread_cycle_start_thrs = 8
});