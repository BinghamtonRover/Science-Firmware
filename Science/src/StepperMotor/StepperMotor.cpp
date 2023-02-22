#include "StepperMotor.h"

// ---------- StepperMotor ----------

/// A "step" consists of one pulse on the #stepPin.
void StepperMotor::writeStep() {
	Serial.print("Delay: ");
	Serial.println(config->pwmDelay);
	digitalWrite(pins.step, HIGH);
	delayMicroseconds(config->pwmDelay);
	digitalWrite(pins.step, LOW);
	delayMicroseconds(config->pwmDelay);
}

void StepperMotor::writeSteps(int steps) {
	/* Stops the motor from moving too far by keeping track of the current position. */
	Serial.print("  Moving ");
	Serial.print(steps);
	Serial.println(" steps");
	digitalWrite(pins.direction, steps > 0 ? HIGH : LOW); 
	int direction = steps > 0 ? 1 : -1;
	for (int step = 0; step < abs(steps); step++) {
		if (readLimitSwitch() && direction < 0) continue;  // too close to switch
		if (currentPosition >= config->limit && direction > 0) continue;  // too far from switch
		writeStep();
		currentPosition += config->distancePerStep * direction;  // update position
		delayMicroseconds(config->pwmDelay);
	}
}

int StepperMotor::distanceToSteps(float distance) { 
	return distance / config->distancePerStep; 
}

StepperMotor::StepperMotor(MotorPins pins, MotorConfig* config) : pins(pins), config(config) { }

void StepperMotor::setup() {
	// For direction: HIGH means clockwise
	pinMode(pins.step, OUTPUT);
	pinMode(pins.direction, OUTPUT);
	pinMode(pins.limitSwitch, INPUT_PULLUP);
	// Set the current for the motor
	switch (config->current) {  // in mA
		case 500:   // 0.5A = INPUT, INPUT
			pinMode(pins.current1, INPUT);
			pinMode(pins.current2, INPUT);
			break;
		case 1000:  // 1.0A = LOW, INPUT
			pinMode(pins.current1, OUTPUT);
			pinMode(pins.current2, INPUT);
			digitalWrite(pins.current1, LOW);		
			break;
		case 1500:   // 1.5A = INPUT, LOW
			pinMode(pins.current1, INPUT);
			pinMode(pins.current2, OUTPUT);
			digitalWrite(pins.current2, LOW);
			break;
		case 2000:   // 2.0A = LOW, LOW
			pinMode(pins.current1, OUTPUT);
			pinMode(pins.current2, OUTPUT);
			digitalWrite(pins.current1, LOW);
			digitalWrite(pins.current2, LOW);
			break;
		default: 
			Serial.print("Unrecognized current level for stepper motor: ");
			Serial.println(config->current);
			while(true);
	}
}

void StepperMotor::calibrate() {
	while (!readLimitSwitch()) writeSteps(-10);
	currentPosition = 0;
}

void StepperMotor::moveBy(float distance) {
	int steps = distanceToSteps(distance);
	writeSteps(steps);
}

void StepperMotor::moveTo(float position) {
	int stepDifference = distanceToSteps(position - currentPosition);
	writeSteps(stepDifference);
}

bool StepperMotor::readLimitSwitch() {
	return digitalRead(pins.limitSwitch) == HIGH;
}

// ---------- DirtCarousel ----------

void DirtCarousel::nextTube() { moveBy(30); }
void DirtCarousel::nextSection() { moveBy(120); }
