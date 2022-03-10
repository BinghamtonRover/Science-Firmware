// Contains all the StepperMotor and DCMotor objects.
// Also includes <science_motors.h> 
#include <science_pinouts.h>

/* This script controls everything except for the Auger. */

// #define AUGER_FAST_SPEED 50
// #define AUGER_DROP_SPEED -25
// #define AUGER_DIG_DELAY 1000 // ms
// #define AUGER_DROP_DELAY 1000  // ms

#define AUGER_LINEAR_MAX_HEIGHT 215 // 200  // mm
#define DIRT_LINEAR_OUTER_RADIUS 35  // mm
#define DIRT_LINEAR_INNER_RADIUS 35  // mm
#define DIRT_LINEAR_TEST_OFFSET 10   // mm
#define SCIENCE_LINEAR_MAX_HEIGHT 35  // mm

#define SENSORS_READ_DELAY 1000  // ms

#define PUMP_SPEED 100
#define PUMP_DELAY 10000  // 2000  // ms

void block() {
	while (!Serial.available());
	Serial.readString();  // empty the buffer
}

void setup() {
	Serial.begin(9600);
	// TODO: Add CAN bus initialization here.
	Serial.println("Interface initialized.");

	augerLinear.setup();
	dirtLinear.setup();
	scienceLinear.setup();
	dirtCarousel.setup();
	calibrate();
	Serial.println("Stepper motors initialized.");

	// auger.setup();
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
	// augerLinear.calibrate();  // WARNING: will go through the table!
}

void dig() {  // all motor movements are blocking
	// Lower and dig
	// dirtLinear.setPosition(0);
	// augerLinear.setPosition(0);

	// auger.setSpeed(AUGER_FAST_SPEED); 
	// delay(AUGER_DIG_DELAY); 
	// auger.softBrake();
	// Serial.println("Press Enter when the digging is finished.");
	// block();

	// Lift and dump
	// augerLinear.setPosition(AUGER_LINEAR_MAX_HEIGHT);
	dirtLinear.setPosition(DIRT_LINEAR_OUTER_RADIUS);

	// Drop in each tube.
	// 
	// Tube 1
	// auger.setSpeed(AUGER_DROP_SPEED);
	// delay(AUGER_DROP_DELAY);
	// auger.softBrake();
	Serial.println("Press Enter when the drop is finished.");
	block();

	// Tube 2
	dirtCarousel.nextTube();
	// auger.setSpeed(AUGER_DROP_SPEED);
	// delay(AUGER_DROP_DELAY);
	// auger.softBrake();
	Serial.println("Press Enter when the drop is finished.");
	block();

	// Inner tube
	dirtLinear.setPosition(DIRT_LINEAR_INNER_RADIUS);
	// auger.setSpeed(AUGER_DROP_SPEED);
	// delay(AUGER_DROP_DELAY);
	// auger.softBrake();
	Serial.println("Press Enter when the drop is finished.");
	block();


	// Tube 3
	dirtLinear.setPosition(DIRT_LINEAR_OUTER_RADIUS);
	dirtCarousel.nextTube();
	// auger.setSpeed(AUGER_DROP_SPEED);
	// delay(AUGER_DROP_DELAY);
	// auger.softBrake();
	Serial.println("Press Enter when the drop is finished.");
	block();

	// // Tube 4
	dirtCarousel.nextTube();
	// auger.setSpeed(AUGER_DROP_SPEED);
	// delay(AUGER_DROP_DELAY);
	// auger.softBrake();
	Serial.println("Press Enter when the drop is finished.");
	block();
	dirtCarousel.nextSection();

	// // Reset
	dirtLinear.setPosition(0);	
	// auger.setSpeed(AUGER_DROP_SPEED);
	// delay(AUGER_DROP_DELAY);
	// auger.softBrake();
	Serial.println("Press Enter when the excess is dropped.");
	block();

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
	pump1.setSpeed(PUMP_SPEED);
	pump2.setSpeed(PUMP_SPEED);
	pump3.setSpeed(PUMP_SPEED);
	pump4.setSpeed(PUMP_SPEED);
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
	if (motor == "auger-linear") augerLinear.moveDistance(distance);
	else if (motor == "dirt-linear") dirtLinear.moveDistance(distance);
	else if (motor == "science-linear") scienceLinear.moveDistance(distance);
	else if (motor == "dirt-carousel") dirtCarousel.rotate(distance);
	// else if (motor == "auger") auger.setSpeed(speed);
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
		Serial.println("  For example: auger-linear 20, would move the auger 20mm up.");
		Serial.println("  Example 2: auger 100 spins the auger at full speed.");
		Serial.println("");
	}
}
