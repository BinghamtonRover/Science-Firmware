#include "DcMotor.h"

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
