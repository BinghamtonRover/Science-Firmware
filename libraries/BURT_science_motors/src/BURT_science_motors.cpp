#include "BURT_science_motors.h"

// ---------- Stepper Motor ---------- 

void StepperMotor::writeStep() {
	digitalWrite(stepPin, HIGH);
	delayMicroseconds(PWM_DELAY);
	digitalWrite(stepPin, LOW);
	delayMicroseconds(PWM_DELAY);
}

void StepperMotor::writeSteps(int steps) {
	digitalWrite(directionPin, steps > 0 ? HIGH : LOW);
	for (int step = 0; step < abs(steps); step++) writeStep();
}

void StepperMotor::setup() {
	pinMode(stepPin, OUTPUT);
	pinMode(directionPin, OUTPUT);
	if (current == 0.5) {         // 0.5mA = INPUT, INPUT
		pinMode(currentPin1, INPUT);
		pinMode(currentPin2, INPUT);
	} else if (current == 1) {    // 1.0mA = LOW, INPUT
		pinMode(currentPin1, OUTPUT);
		pinMode(currentPin2, INPUT);
		digitalWrite(currentPin1, LOW);		
	} else if (current == 1.5) {  // 1.5mA = INPUT, LOW
		pinMode(currentPin1, INPUT);
		pinMode(currentPin2, OUTPUT);
		digitalWrite(currentPin2, LOW);
	} else {
		Serial.print("Unrecognized current level for stepper motor: ");
		Serial.println(current);
	}
}

// ---------- LinearStepperMotor ----------

void LinearStepperMotor::setup() {
	StepperMotor::setup();
	pinMode(limitPin, INPUT_PULLUP);
}

void LinearStepperMotor::writeSteps(int steps) {
	/* Will stop the motor from moving too far in one direction. */
	digitalWrite(directionPin, steps > 0 ? HIGH : LOW); 
	short direction = steps > 0 ? 1 : -1;
	for (int step = 0; step < abs(steps); step++) {
		if (readLimitSwitch() && direction < 0) continue;  // too close to switch
		if (distance >= limit && direction > 0) continue;  // too far from switch
		writeStep();
		distance += DISTANCE_PER_STEP * direction;  // update position
	}
}

int LinearStepperMotor::distanceToSteps(float distance) { 
	return distance / DISTANCE_PER_STEP; 
}

bool LinearStepperMotor::readLimitSwitch() { 
	return digitalRead(limitPin) == 1; 
}

void LinearStepperMotor::calibrate() {
	while (!readLimitSwitch()) writeSteps(-10);
	distance = 0;
}

void LinearStepperMotor::moveDistance(float offset) {
	int steps = distanceToSteps(offset);
	writeSteps(steps);
}

void LinearStepperMotor::setPosition(float position) {
	int stepDifference = distanceToSteps(position - distance);
	writeSteps(stepDifference);
}

// ---------- RotatingStepperMotor ----------

void RotatingStepperMotor::writeSteps(int steps) {
	digitalWrite(directionPin, steps > 0 ? HIGH : LOW);
	short direction = steps > 0 ? 1 : -1;
	for (int step = 0; step < abs(steps); step++) {
		writeStep();
		angle += DEGREE_PER_STEP * direction;
	}
}

int RotatingStepperMotor::degreesToSteps(float degrees) { 
	return degrees / DEGREE_PER_STEP; 
}

void RotatingStepperMotor::calibrate() {
	angle = 0;
}

void RotatingStepperMotor::rotate(float degrees) {
	int steps = degreesToSteps(degrees);
	writeSteps(steps);
}

void RotatingStepperMotor::setAngle(float newAngle) {
	int stepDifference = degreesToSteps(newAngle - angle);
	writeSteps(stepDifference);
}

void RotatingStepperMotor::nextTube() {
	/* Each tube is 30 degrees apart from the last. */
	// okay so the motor isn't working, let's do 15 instead of 30
	rotate(15);  
}

void RotatingStepperMotor::nextSection() {
	/* There are 3 sections, hence each occupies 120 degrees. */
	// rotate(120);
	nextTube();
	delay(500);
	nextTube();
	delay(500);
	nextTube();
	delay(500);
	nextTube();
	delay(500);	
}

// ---------- DC Motors ----------

void DCMotor::setup() {
	pinMode(pwmPin, OUTPUT);
	pinMode(in1Pin, OUTPUT);
	pinMode(in2Pin, OUTPUT);
}

void DCMotor::setSpeed(int speed) {
	Serial.println(speed);
	if (speed == 0) return softBrake();
	bool isForward = speed > 0;

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

// ---------- Auger ---------- 

void Auger::setup() {
	pinMode(rPWMPin, OUTPUT);
	pinMode(lPWMPin, OUTPUT);
	pinMode(rEnablePin, OUTPUT);
	pinMode(lEnablePin, OUTPUT);
	pinMode(rDrivePin, OUTPUT);
	pinMode(lDrivePin, OUTPUT);
	digitalWrite(rEnablePin, HIGH);
	digitalWrite(lEnablePin, HIGH);
	digitalWrite(rDrivePin, LOW);
	digitalWrite(lDrivePin, LOW);
}

void Auger::setSpeed(int newSpeed) {
	/* Sets a speed between [-255, 255]. */
	speed = newSpeed;
	bool isForward = speed > 0;
	speed = abs(speed);

	analogWrite(rPWMPin, isForward ? speed : 0);
	analogWrite(lPWMPin, isForward ? 0 : speed);
}

void Auger::softBrake() {
	// TODO: Find a way to kill power instead
	for (int newSpeed = speed; newSpeed > 0; newSpeed -= 10) {
		setSpeed(newSpeed);
	}
}

void Auger::hardBrake() {
	setSpeed(0);
}
