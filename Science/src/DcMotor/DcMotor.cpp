#include "DcMotor.h"

DcMotor::DcMotor(int pwmPin, int in1Pin, int in2Pin) : pwmPin(pwmPin), in1Pin(in1Pin), in2Pin(in2Pin) { }

void DcMotor::setup() {
	pinMode(pwmPin, OUTPUT);
	pinMode(in1Pin, OUTPUT);
	pinMode(in2Pin, OUTPUT);
}

void DcMotor::setSpeed(float speed) {
	if (speed == 0) return softBrake();
	speed = constrain(speed, -1, 1);
	bool isForward = speed > 0;
	analogWrite(pwmPin, speed * 255);
	digitalWrite(in1Pin, isForward ? HIGH : LOW);
	digitalWrite(in2Pin, isForward ? LOW : HIGH);
}

void DcMotor::softBrake() {
	digitalWrite(pwmPin, HIGH);
	digitalWrite(in1Pin, LOW);
	digitalWrite(in2Pin, LOW);
}

void DcMotor::hardBrake() {
	digitalWrite(pwmPin, HIGH);
	digitalWrite(in1Pin, HIGH);
	digitalWrite(in2Pin, HIGH);
}
