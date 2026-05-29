#pragma once
#include "src/auger_servo/auger_servo.h"
#include "src/lidar/lidar.h"
#include "src/temp_humidity/temp_humidity.h"
#include "/src/TFMini-Plus/src/TFMPlus.h"

// UART LiDAR (TFMini-Plus)
TFMPlus tfmp;
#include "src/tmc/BURT_TMC.h"
#include "src/current_sensor/current_sensor.h"

// -------------------- Pinouts --------------------

const int upper_servo_pin = 10;
const int lower_servo_pin = 3;

AugerServo upper_servo(upper_servo_pin, 0, 87);
AugerServo lower_servo(lower_servo_pin, 160, 97);

LidarSensor lidar;

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
  .step_pin = 2,
  .dir_pin = 6
},

StepDirConfig {
    .gear_ratio = 48,
    .double_edge = false,
    .run_current_scale = 28,
    .hold_current_scale = 4,
    .ihold_delay_scale = 8,
    .invert_dir = false,
    .stealth_chop_en = false,
    .spread_cycle_start_thrs = 8
});