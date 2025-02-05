#pragma once

#include "motor_pinouts.h"
#include "src/scooper/scooper.h"
#include "src/temp_humidity/temp_humidity.h"
#include "src/pumps/pumps.h"
#include "src/carousel/carousel.h"
#include "src/co2/co2_sensor.h"
#include "src/motors/motors.h"

// -------------------- Pinouts --------------------

const int servo1 = 25;
const int servo2 = 33;
const int co2_address = 105;

Motors motors(scoopArmMotor, dirtCarouselMotor, dirtLinearMotor);
Scooper scooper(scoopArmMotor, servo1);
DirtCarousel carousel(dirtCarouselMotor, servo2);

TempHumiditySensor tempHumidity;
Co2Sensor co2(co2_address);

Pumps pumps;
