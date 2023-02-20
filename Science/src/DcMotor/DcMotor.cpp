#include "DcMotor.h"

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

