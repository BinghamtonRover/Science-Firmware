#ifndef BURT_science_pinouts_h
#define BURT_science_pinouts_h

#include "BURT_science_motors.h"

// -------------------- Pinouts --------------------

// Auger Linear Actuator: Stepper1 on the PCB
#define AUGER_LINEAR_DIRECTION_PIN 39   // DIR
#define AUGER_LINEAR_STEP_PIN 40  	    // STP
#define AUGER_LINEAR_CURRENT1_PIN 18    // I1
#define AUGER_LINEAR_CURRENT2_PIN 41    // I2
#define AUGER_LINEAR_LIMIT_PIN 24       // Bump1

// Dirt Carousel Linear Actuator: Stepper2 on the PCB
#define DIRT_LINEAR_DIRECTION_PIN 9     // DIR
#define DIRT_LINEAR_STEP_PIN 10  	      // STP
#define DIRT_LINEAR_CURRENT1_PIN 12     // I1
#define DIRT_LINEAR_CURRENT2_PIN 11     // I2
#define DIRT_LINEAR_LIMIT_PIN 31        // Bump2

// Science linear Actuator: Stepper3 on the PCB
#define SCIENCE_LINEAR_DIRECTION_PIN 5  // DIR
#define SCIENCE_LINEAR_STEP_PIN 6       // STP
#define SCIENCE_LINEAR_CURRENT1_PIN 8   // I1
#define SCIENCE_LINEAR_CURRENT2_PIN 7   // I2
#define SCIENCE_LINEAR_LIMIT_PIN 32     // Bump2

// Dirt Carousel: Stepper4 on the PCB
#define DIRT_CAROUSEL_DIRECTION_PIN 1   // DIR
#define DIRT_CAROUSEL_STEP_PIN 2        // STP
#define DIRT_CAROUSEL_CURRENT1_PIN 4    // I1
#define DIRT_CAROUSEL_CURRENT2_PIN 3    // I2

// Auger:
// #define AUGER_PWM_PIN 19
// #define AUGER_IN1_PIN 21
// #define AUGER_IN2_PIN 20

#define R_PWM_PIN 3
#define L_PWM_PIN 5
#define R_ENABLE_PIN 2
#define L_ENABLE_PIN 4
#define R_DRIVE_PIN 6
#define L_DRIVE_PIN 7

// Pump 1: <LABEL WITH CHEMICAL>
#define PUMP1_PWM_PIN 33 
#define PUMP1_IN1_PIN 35
#define PUMP1_IN2_PIN 34

// Pump 2: <LABEL WITH CHEMICAL>
#define PUMP2_PWM_PIN 37 
#define PUMP2_IN1_PIN 36
#define PUMP2_IN2_PIN 38

// Pump 3: <LABEL WITH CHEMICAL>
#define PUMP3_PWM_PIN 29 
#define PUMP3_IN1_PIN 27
#define PUMP3_IN2_PIN 30

// Pump 4: <LABEL WITH CHEMICAL>
#define PUMP4_PWM_PIN 24 
#define PUMP4_IN1_PIN 26
#define PUMP4_IN2_PIN 25

// --------------- Motor definitions ---------------

LinearStepperMotor augerLinear(  // Stepper1 on the PCB
	AUGER_LINEAR_DIRECTION_PIN,  
	AUGER_LINEAR_STEP_PIN,  
	AUGER_LINEAR_CURRENT1_PIN,  
	AUGER_LINEAR_CURRENT2_PIN, 
	AUGER_LINEAR_LIMIT_PIN,
	215
);

LinearStepperMotor dirtLinear(  // Stepper2 on the PCB
	DIRT_LINEAR_DIRECTION_PIN,
	DIRT_LINEAR_STEP_PIN,
	DIRT_LINEAR_CURRENT1_PIN,
	DIRT_LINEAR_CURRENT2_PIN,
	DIRT_LINEAR_LIMIT_PIN,
	35
);

LinearStepperMotor scienceLinear(  // Stepper3 on the PCB
	SCIENCE_LINEAR_DIRECTION_PIN,
	SCIENCE_LINEAR_STEP_PIN,
	SCIENCE_LINEAR_CURRENT1_PIN,
	SCIENCE_LINEAR_CURRENT2_PIN,
	SCIENCE_LINEAR_LIMIT_PIN,
	35
);

RotatingStepperMotor dirtCarousel(  // Stepper4 on the PCB
	DIRT_CAROUSEL_DIRECTION_PIN, 
	DIRT_CAROUSEL_STEP_PIN,
	DIRT_CAROUSEL_CURRENT1_PIN,
	DIRT_CAROUSEL_CURRENT2_PIN
);

Auger auger = Auger(
	R_PWM_PIN, 
	L_PWM_PIN, 
	R_ENABLE_PIN, 
	L_ENABLE_PIN, 
	R_DRIVE_PIN, 
	L_DRIVE_PIN
);

// DCMotor auger(AUGER_PWM_PIN, AUGER_IN1_PIN, AUGER_IN2_PIN);
DCMotor pump1(PUMP1_PWM_PIN, PUMP1_IN1_PIN, PUMP1_IN2_PIN);
DCMotor pump2(PUMP2_PWM_PIN, PUMP2_IN1_PIN, PUMP2_IN2_PIN);
DCMotor pump3(PUMP3_PWM_PIN, PUMP3_IN1_PIN, PUMP3_IN2_PIN);
DCMotor pump4(PUMP4_PWM_PIN, PUMP4_IN1_PIN, PUMP4_IN2_PIN);

#endif 
