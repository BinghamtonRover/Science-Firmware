#pragma once

#include "motor_pinouts.h"
#include "src/pumps/pumps.h"
#include "src/carousel/carousel.h"
#include "src/co2/co2_sensor.h"
#include "src/motors/motors.h"

// -------------------- Pinouts --------------------
const int co2_address = 105;

Motors motors(dirtCarouselMotor);

DirtCarousel carousel(dirtCarouselMotor);

Co2Sensor co2(co2_address);

Pumps pumps;
// 8 - 9 seconds