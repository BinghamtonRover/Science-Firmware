
#include "src/CO2/src/CO2Sensor.h"
#include "src/Methane/src/MethaneSensor.h"
#include "src/pH/src/pH.h"
#include "src/Humidity/src/HumiditySensor.h"

// Contains all the StepperMotor and DCMotor objects.
#include "src/BURT_science_pinouts.h"
#include "src/BURT_science_motors.h"

/* This script controls everything except for the Auger. */

#define VACUUM_FAST_SPEED 50
//#define AUGER_DROP_SPEED -25 don"t need?
#define VACUUM_DIG_DELAY 1000 // ms
#define VACUUM_DROP_DELAY 1000  // ms

#define VACUUM_LINEAR_MAX_HEIGHT -370 //Changed from 215 mm, Double check this
#define DIRT_LINEAR_OUTER_RADIUS 35  // mm
#define DIRT_LINEAR_INNER_RADIUS 35  // mm
#define DIRT_LINEAR_TEST_OFFSET 10   // mm
#define SCIENCE_LINEAR_MAX_HEIGHT 35  // mm

#define SENSORS_READ_DELAY 1000  // ms

#define PUMP_SPEED -100 //must be negative to pump, positive blows bubbles
#define PUMP_DELAY 10000  // 2000  // ms

void block() {
	while (!Serial.available());
	Serial.readString();  // empty the buffer
}

void setup() {
	Serial.begin(9600);
	// TODO: Add CAN bus initialization here.
	Serial.println("Interface initialized.");

	vacuumLinear.setup();
	dirtLinear.setup();
	scienceLinear.setup();
	dirtCarousel.setup();
	calibrate();
	Serial.println("Stepper motors initialized.");

	//vacuum.setup(); 
	pump1.setup();
	pump2.setup();
	pump3.setup();
	pump4.setup();
	Serial.println("DC motors initialized.");

	Serial.println("Science Subsystem ready.");
}

void loop() {
	// Temporary Serial Monitor interface for testing
	String input = Serial.readString();
	parseSerialCommand(input);
	delay(500);
}

void calibrate() {
	/* Calibrates the 4 stepper motors. */
	dirtCarousel.calibrate();
	dirtLinear.calibrate();
	scienceLinear.calibrate();
	vacuumLinear.calibrate();  // WARNING: will go through the table!, theoretically this won't happen anymore?-be careful first testing
}

void dig() {  // all motor movements are blocking
	// Lower and dig
	dirtLinear.setPosition(0);
  vacuumLinear.setPosition(0); 

	vacuum.setSpeed(VACUUM_FAST_SPEED); 
	delay(VACUUM_DIG_DELAY); 
	vacuum.softBrake();
	Serial.println("Press Enter when the digging is finished.");
	block();

	// Lift and dump
	vacuumLinear.setPosition(VACUUM_LINEAR_MAX_HEIGHT);
	dirtLinear.setPosition(DIRT_LINEAR_OUTER_RADIUS);
/*
	// Drop in each tube. NEED TO CHANGE
	// 
	// Tube 1
	//vservo.open();
	//delay(VACUUM_DROP_DELAY);
  //vservo.close();
	//Serial.println("Press Enter when the drop is finished.");
	//block();

	// Tube 2
	dirtCarousel.nextTube();
	vservo.open();
	delay(VACUUM_DROP_DELAY);
	vservo.close();
	Serial.println("Press Enter when the drop is finished.");
	block();

	// Inner tube
	dirtLinear.setPosition(DIRT_LINEAR_INNER_RADIUS);
	vservo.open();
	delay(VACUUM_DROP_DELAY);
	vservo.close();
	Serial.println("Press Enter when the drop is finished.");
	block();


	// Tube 3
	dirtLinear.setPosition(DIRT_LINEAR_OUTER_RADIUS);
	dirtCarousel.nextTube();
	vservo.open();
	delay(VACUUM_DROP_DELAY);
	versvo.close();
	Serial.println("Press Enter when the drop is finished.");
	block();

	// // Tube 4
	dirtCarousel.nextTube();
	vservo.open();
	delay(AUGER_DROP_DELAY);
	vservo.close();
	Serial.println("Press Enter when the drop is finished.");
	block();
	dirtCarousel.nextSection();

	// // Reset
	dirtLinear.setPosition(0);	
	vservo.open();
	delay(AUGER_DROP_DELAY);
	vservo.close();
	Serial.println("Press Enter when the excess is dropped.");
	block();
*/
	testSamples();
}

void testSamples() {
	Serial.println("Moving dirt linear");
	dirtLinear.setPosition(DIRT_LINEAR_TEST_OFFSET);
	Serial.println("Moving science linear");
	scienceLinear.setPosition(SCIENCE_LINEAR_MAX_HEIGHT);

	Serial.println("Pumping...");
	pump1.setSpeed(PUMP_SPEED);
	pump2.setSpeed(PUMP_SPEED);
	pump3.setSpeed(PUMP_SPEED);
	pump4.setSpeed(PUMP_SPEED);
	delay(PUMP_DELAY);
	pump1.hardBrake();
	pump2.hardBrake();
	pump3.hardBrake();
	pump4.hardBrake();

	Serial.println("Transmitting data. (not really)..");
	delay(SENSORS_READ_DELAY);
	scienceLinear.setPosition(0);
}

void bubbles() {
	pump1.setSpeed(-PUMP_SPEED);
	pump2.setSpeed(-PUMP_SPEED);
	pump3.setSpeed(-PUMP_SPEED);
	pump4.setSpeed(-PUMP_SPEED);
	delay(PUMP_DELAY);
	pump1.hardBrake();
	pump2.hardBrake();
	pump3.hardBrake();
	pump4.hardBrake();
}

void parseSerialCommand(String input) {
	/* Temporary Serial Monitor interface for testing. */

	// Accept the command
	if (input == "calibrate") return calibrate();
	else if (input  == "dig") return dig();
	else if (input == "temp") return dirtCarousel.nextSection();
	else if (input == "test") return testSamples();
	else if (input == "bubbles") return bubbles();

	// Parse the command
	int delimiter = input.indexOf(" ");
	if (delimiter == -1) return;
	String part1 = input.substring(0, delimiter);
	String part2 = input.substring(delimiter + 1);
	String motor = part1;
	float distance = part2.toFloat();
	int speed = part2.toInt();

	// Execute the command
	if (motor == "vacuum-linear") vacuumLinear.moveDistance(distance);
	else if (motor == "dirt-linear") dirtLinear.moveDistance(distance);
	else if (motor == "science-linear") 
  {
    scienceLinear.moveDistance(distance);
  }
	else if (motor == "dirt-carousel") dirtCarousel.rotate(distance);
	else if (motor == "vacuum") vacuum.setSpeed(speed);
	else if (motor == "pump1") {
		pump1.setSpeed(speed);
		delay(PUMP_DELAY);
		pump1.hardBrake();
	} else if (motor == "pump2") {
		pump2.setSpeed(speed);
		delay(PUMP_DELAY);
		pump2.hardBrake();
	} else if (motor == "pump3") {
		pump3.setSpeed(speed);
		delay(PUMP_DELAY);
		pump3.hardBrake();
	} else if (motor == "pump4") {
		Serial.println(speed);
		pump4.setSpeed(speed);
		delay(PUMP_DELAY);
		pump4.hardBrake();
	}
	else {
		Serial.println("Command not recognized: " + input);
		Serial.println("  Commands are of the form: motor-name distance/speed.");
		Serial.println("  For stepper motors: positive distance is away from the limit switch.");
		Serial.println("  For DC motors: speed is in the range [-100, 100]. Positive = clockwise.");
		Serial.println("  For example: vacuum-linear 20, would move the vacuum 20mm up.");
		Serial.println("  Example 2: auger 100 spins the auger at full speed.");
		Serial.println("");
	}
}
