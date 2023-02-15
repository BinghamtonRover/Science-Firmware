#include "BURT_science_motors.h"

// ---------- Stepper Motor ---------- 

void StepperMotor::writeStep() {
	/* A "step" consists of one pulse on the PWM pin. */
	digitalWrite(stepPin, HIGH);
	delayMicroseconds(PWM_DELAY);
	digitalWrite(stepPin, LOW);
	delayMicroseconds(PWM_DELAY);
}

void StepperMotor::writeSteps(int steps) {
	// For direction: HIGH means clockwise
	digitalWrite(directionPin, steps > 0 ? HIGH : LOW);
	for (int step = 0; step < abs(steps); step++) writeStep();
}

void StepperMotor::setup() {
	pinMode(stepPin, OUTPUT);
	pinMode(directionPin, OUTPUT);
	// The motors have different current ratings, denoted by currentPin's 1 and 2
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
	/* Stops the motor from moving too far by keeping track of the current position. */
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
	// HIGH means the switch is being depressed.
	return digitalRead(limitPin) == HIGH;
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

//----------VacuumLinearStepperMotor--------

void VacuumLinearStepperMotor::setup() {
	StepperMotor::setup();
	pinMode(limitPin, INPUT_PULLUP);
}

void VacuumLinearStepperMotor::writeSteps(int steps) {
	/* Stops the motor from moving too far by keeping track of the current position. */
	digitalWrite(directionPin, steps > 0 ? HIGH : LOW); 
	short direction = steps > 0 ? 1 : -1;
	for (int step = 0; step < abs(steps); step++) {
		if (readLimitSwitch() && direction < 0) continue;  // too close to switch
		if (distance >= limit && direction > 0) continue;  // too far from switch
		writeStep();
		distance += DISTANCE_PER_STEP_GEAR * direction;  // update position
	}
}

int VacuumLinearStepperMotor::distanceToSteps(float distance) { 
	return distance / DISTANCE_PER_STEP_GEAR; 
}

bool VacuumLinearStepperMotor::readLimitSwitch() { 
	// HIGH means the switch is being depressed.
	return digitalRead(limitPin) == HIGH;
}

void VacuumLinearStepperMotor::calibrate() {
	while (!readLimitSwitch()) writeSteps(10); //Changed from -10 to account for limit switch at top
	distance = 0;
} 

void VacuumLinearStepperMotor::moveDistance(float offset) {
	int steps = distanceToSteps(offset);
	writeSteps(steps);
}

void VacuumLinearStepperMotor::setPosition(float position) {
	int stepDifference = distanceToSteps(position - distance);
	writeSteps(stepDifference);
}


// ---------- RotatingStepperMotor ----------

void RotatingStepperMotor::writeSteps(int steps) {
	digitalWrite(directionPin, steps > 0 ? HIGH : LOW);
	short direction = steps > 0 ? 1 : -1;
	for (int step = 0; step < abs(steps); step++) {
		if (readLimitSwitch() && direction < 0) continue;  // too close to switch
		if (angle >= limit && direction > 0) continue;  // too far from switch
		writeStep();
		angle += DEGREE_PER_STEP * direction;
	}
}

int RotatingStepperMotor::degreesToSteps(float degrees) { 
	return degrees / DEGREE_PER_STEP; 
}

void RotatingStepperMotor::calibrate() {
	while (!readLimitSwitch()) writeSteps(-10);
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
	rotate(30);  
}

void RotatingStepperMotor::nextSection() {
	rotate(120);
}

bool RotatingStepperMotor::readLimitSwitch() { 
	// HIGH means the switch is being depressed.
	return digitalRead(limitPin) == HIGH;
} 

//----------Vacuum Servo---------//
void vacuum_servo::open() {
	for (int pos = 0; pos <= 180; pos += 1) { 
    myservo.write(pos);              
    delay(15); 
	}
};

void vacuum_servo::close() {
    for (int pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);              
    delay(15); 
    }
};

// ---------- DC Motors ----------

void DCMotor::setup() {
	pinMode(pwmPin, OUTPUT);
	pinMode(in1Pin, OUTPUT);
	pinMode(in2Pin, OUTPUT);
}

void DCMotor::setSpeed(int speed) {
	if (speed > 100 || speed < -100) {
		Serial.println("ERROR: Speed needs to be between -100 and 100. Got: " + speed);
		return;
	}
	if (speed == 0) return softBrake();
	bool isForward = speed > 0;
	int voltage = map(speed, -100, 100, -255, 255);
	analogWrite(pwmPin, voltage);
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

// ---------- Vacuum ----------  

void Vacuum::setup() {
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

void Vacuum::setSpeed(int newSpeed) {
	if (newSpeed > 100 || newSpeed < -100) {
		Serial.println("Speed must be between -100 and 100. Got: " + newSpeed);
		return;
	}

	bool isForward = newSpeed > 0;
	speed = abs(newSpeed);
	int voltage = map(newSpeed, -100, 100, -255, 255);
	analogWrite(rPWMPin, isForward ? voltage : 0);
	analogWrite(lPWMPin, isForward ? 0 : voltage);
}

void Vacuum::softBrake() {
	// TODO: Find a way to kill power instead. 
	// Workaround: slowly ramp down.
	if (speed > 0) {  // ramp down
		for (int newSpeed = speed; newSpeed > 0; newSpeed -= 10) {
			setSpeed(newSpeed);
			delay(5);
		}
	} else {  // ramp up
		for (int newSpeed = speed; newSpeed < 0; newSpeed += 10) {
			setSpeed(newSpeed);
			delay(5);
		}
	}
}

void Vacuum::hardBrake() {
	// TODO: Find a way to hard brake (see DCMotor.hardBrake)
	// Workaround: immediately set speed to 0.
	setSpeed(0);
}
