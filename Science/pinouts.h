#pragma once

#include "motor_pinouts.h"
#include "src/scooper/scooper.h"
#include "src/temp_humidity/temp_humidity.h"
#include "src/pumps/pumps.h"
#include "src/carousel/carousel.h"
#include "src/co2/co2_sensor.h"
#include "src/motors/motors.h"

// -------------------- Pinouts --------------------

#define CO2_PIN 17
#define FUNNEL_SERVO 38  // Methane sensor connector, on SPI 1
#define SCOOP_SERVO 39  // pH sensor connector, SPI 1

Motors motors(scoopArmMotor, dirtCarouselMotor, dirtLinearMotor);
Scooper scooper(scoopArmMotor, SCOOP_SERVO);
DirtCarousel carousel(dirtCarouselMotor, FUNNEL_SERVO);

TempHumiditySensor tempHumidity;
Co2Sensor co2(CO2_PIN);

Pumps pumps;
