#include "Scooper.h"

#define OPEN_POS 0
#define CLOSE_POS 45

void Scooper::setup() {
	servo.attach(servoPin);
}

void Scooper::open() {
	servo.write(OPEN_POS);
}

void Scooper::close() {
	servo.write(CLOSE_POS);
}	

void Scooper::rotateBy(float degrees) {
	stepper.moveBy(degrees);
}

void Scooper::rotateTo(float degrees) {
	stepper.moveTo(degrees);
}
