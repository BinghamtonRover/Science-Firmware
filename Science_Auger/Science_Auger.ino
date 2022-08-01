// Contains all the StepperMotor and DCMotor objects.
// Also includes <science_motors.h> 
// #include <science_pinouts.h>
#include <science_motors.h>

#define AUGER_FAST_SPEED 50
#define AUGER_DROP_SPEED -25
#define AUGER_DIG_DELAY 1000  // ms
#define AUGER_DROP_DELAY 1000  // ms

#define AUGER_LINEAR_MAX_HEIGHT 0  // mm
#define DIRT_LINEAR_OUTER_RADIUS 0  // mm
#define DIRT_LINEAR_INNER_RADIUS 0  // mm
#define SCIENCE_LINEAR_MAX_HEIGHT 0  // mm

#define SENSORS_READ_DELAY 100  // ms

#define PUMP_SPEED 0
#define PUMP_DELAY 0  // ms

// TODO: Move properly to science_pinouts

#define R_DRIVE_PIN 1
#define R_ENABLE_PIN 2
#define R_PWM_PIN 3
#define L_DRIVE_PIN 4
#define L_ENABLE_PIN 5
#define L_PWM_PIN 6

Auger auger = Auger(
	R_PWM_PIN, 
	L_PWM_PIN, 
	R_ENABLE_PIN, 
	L_ENABLE_PIN, 
	R_DRIVE_PIN, 
	L_DRIVE_PIN
);


void setup() {
	Serial.begin(9600);
	// TODO: Add CAN bus initialization here.
	Serial.println("Interface initialized.");

	// augerLinear.setup();
	// dirtLinear.setup();
	// scienceLinear.setup();
	// dirtCarousel.setup();
	calibrate();
	Serial.println("Stepper motors initialized.");

	auger.setup();
	// pump1.setup();
	// pump2.setup();
	// pump3.setup();
	// pump4.setup();
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
	// dirtLinear.calibrate();
	// scienceLinear.calibrate();
	// augerLinear.calibrate();  // WARNING: will go through the table!
}

// void dig() {  // all motor movements are blocking
// 	// Lower and dig
// 	// dirtLinear.setPosition(0);
// 	// augerLinear.setPosition(0);
// 	auger.setSpeed(AUGER_FAST_SPEED); 
// 	delay(AUGER_DIG_DELAY); 
// 	auger.softBrake();

// 	// Lift and dump
// 	augerLinear.setPosition(AUGER_LINEAR_MAX_HEIGHT);
// 	dirtLinear.setPosition(DIRT_LINEAR_OUTER_RADIUS);
// 	dirtCarousel.nextTube();

// 	// Drop in each tube.
// 	// 
// 	// Tube 1
// 	auger.setSpeed(AUGER_DROP_SPEED);
// 	delay(AUGER_DROP_DELAY);
// 	auger.hardBrake();

// 	// Tube 2
// 	dirtCarousel.nextTube();
// 	auger.setSpeed(AUGER_DROP_SPEED);
// 	delay(AUGER_DROP_DELAY);
// 	auger.hardBrake();

// 	// Inner tube
// 	dirtLinear.setPosition(DIRT_LINEAR_INNER_RADIUS);
// 	auger.setSpeed(AUGER_DROP_SPEED);
// 	delay(AUGER_DROP_DELAY);
// 	auger.hardBrake();
// 	dirtLinear.setPosition(DIRT_LINEAR_OUTER_RADIUS);

// 	// Tube 3
// 	dirtCarousel.nextTube();
// 	auger.setSpeed(AUGER_DROP_SPEED);
// 	delay(AUGER_DROP_DELAY);
// 	auger.hardBrake();

// 	// Tube 4
// 	dirtCarousel.nextTube();
// 	auger.setSpeed(AUGER_DROP_SPEED);
// 	delay(AUGER_DROP_DELAY);
// 	auger.hardBrake();

// 	// Reset
// 	dirtLinear.setPosition(0);	
// 	auger.setSpeed(AUGER_DROP_SPEED);
// 	delay(AUGER_DROP_DELAY);
// 	auger.hardBrake();

// 	// testSamples();
// }

// void testSamples() {
// 	dirtLinear.setPosition(0);
// 	dirtCarousel.nextSection();
// 	scienceLinear.setPosition(SCIENCE_LINEAR_MAX_HEIGHT);
// 	pump1.setSpeed(PUMP_SPEED);
// 	pump2.setSpeed(PUMP_SPEED);
// 	pump3.setSpeed(PUMP_SPEED);
// 	pump4.setSpeed(PUMP_SPEED);
// 	delay(PUMP_DELAY);
// 	pump1.hardBrake();
// 	pump2.hardBrake();
// 	pump3.hardBrake();
// 	pump4.hardBrake();
// 	Serial.println("Transmitting data. (not really)..");
// 	delay(SENSORS_READ_DELAY);
// 	scienceLinear.setPosition(0);
// }

void parseSerialCommand(String input) {
	/* Temporary Serial Monitor interface for testing. */

	// Accept the command
	// if (input == "calibrate") return calibrate();
	// else if (input  == "dig") return dig();

	// Parse the command
	int delimiter = input.indexOf(" ");
	if (delimiter == -1) return;
	String part1 = input.substring(0, delimiter);
	String part2 = input.substring(delimiter + 1);
	String motor = part1;
	// float distance = part2.toFloat();
	int speed = part2.toInt();

	// Execute the command
	// if (motor == "auger-linear") augerLinear.moveDistance(distance);
	// else if (motor == "dirt-linear") dirtLinear.moveDistance(distance);
	// else if (motor == "science-linear") scienceLinear.moveDistance(distance);
	// else if (motor == "dirt-carousel") dirtCarousel.rotate(distance);
	if (motor == "auger") auger.setSpeed(speed);
	// else if (motor == "pump1") pump1.setSpeed(speed);
	// else if (motor == "pump2") pump2.setSpeed(speed);
	// else if (motor == "pump3") pump3.setSpeed(speed);
	// else if (motor == "pump4") pump4.setSpeed(speed);
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
