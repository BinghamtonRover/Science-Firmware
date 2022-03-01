// Contains all the StepperMotor and DCMotor objects.
// Also includes <science_motors.h> 
#include <science_pinouts.h>

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

	auger.setup();
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
	dirtLinear.calibrate();
	scienceLinear.calibrate();
	dirtCarousel.calibrate();

	// WARNING: Auger will go through the table!
	// augerLinear.calibrate();
}

void dig() {
	Serial.println("Automatic digging not yet implemented.");
}

void parseSerialCommand(String input) {
	/* Temporary Serial Monitor interface for testing. */

	// Accept the command
	if (input == "calibrate") return calibrate();
	else if (input  == "dig") return dig();

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
	else if (motor == "auger") auger.setSpeed(speed);
	else if (motor == "pump1") pump1.setSpeed(speed);
	else if (motor == "pump2") pump2.setSpeed(speed);
	else if (motor == "pump3") pump3.setSpeed(speed);
	else if (motor == "pump4") pump4.setSpeed(speed);
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

// 2550 steps / rotation