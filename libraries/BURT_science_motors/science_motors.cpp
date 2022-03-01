#include "science_motors.h"

// ---------- Stepper Motors ---------- 

void StepperMotor::setup() {
	pinMode(stepPin, OUTPUT);
	pinMode(directionPin, OUTPUT);
	pinMode(limitPin, INPUT_PULLUP);
	// 1A configuration: LOW, INPUT
	pinMode(currentPin1, OUTPUT);
	pinMode(currentPin2, INPUT);
	digitalWrite(currentPin1, LOW);
}

int StepperMotor::distanceToSteps(float distance) { return distance / DISTANCE_PER_STEP; }
int StepperMotor::degreesToSteps(float degrees) { return degrees / DEGREE_PER_STEP; }

bool StepperMotor::isHittingLimit() { return limitPin != 0 && digitalRead(limitPin) == 1; }

void StepperMotor::writeSteps(int steps) {
	digitalWrite(directionPin, steps > 0 ? HIGH : LOW);
	short direction = steps > 0 ? 1 : -1;
	steps = abs(steps);
	for (int step = 0; step < steps; step++) {
		if ((isHittingLimit() && direction < 0) || distance >= limit) break;
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(pulseWidthDelay);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(pulseWidthDelay);
		distance += DISTANCE_PER_STEP * direction;
	}
	Serial.print("Moved to ");
	Serial.println(distance);
}

void StepperMotor::moveDistance(float distance) {
	int steps = distanceToSteps(distance);
	Serial.print("  Writing x steps: ");
	Serial.println(steps);
	writeSteps(steps);
}

void StepperMotor::rotate(float degrees) {
	Serial.print("  Rotating to: ");
	Serial.println(degrees);
	int steps = degreesToSteps(degrees);
	Serial.print("  (That's ");
	Serial.print(steps);
	Serial.println(" steps).");
	writeSteps(steps);
}

void StepperMotor::calibrate() {
	if (limitPin != 0) {
		while (!isHittingLimit()) writeSteps(-10);
	}
	distance = 0;
}

// ---------- DC Motors ----------

void DCMotor::setup() {
	pinMode(pwmPin, OUTPUT);
	pinMode(in1Pin, OUTPUT);
	pinMode(in2Pin, OUTPUT);
}

void DCMotor::setSpeed(byte speed) {
	/* Maps the input [-100, 100] to [50, 255] with direction. */
	if (speed == 0) return softBrake();
	bool isForward = speed > 0;
	speed = map(abs(speed), -100, 100, 50, 255);

	analogWrite(pwmPin, speed);
	digitalWrite(in1Pin, isForward ? HIGH : LOW);
	digitalWrite(in2Pin, isForward ? LOW : HIGH);
}

void DCMotor::softBrake() {
	digitalWrite(pwmPin, HIGH);
	digitalWrite(in1Pin, LOW);
	digitalWrite(in2Pin, LOW);
}

void DCMotor::hardBrake() {
	digitalWrite(pwmPin, HIGH);
	digitalWrite(in1Pin, HIGH);
	digitalWrite(in2Pin, HIGH);
}
