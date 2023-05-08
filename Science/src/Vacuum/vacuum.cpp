#include "vacuum.h"

// ========== Vacuum ==========

Vacuum::Vacuum(int pin) : pin(pin) { }

void Vacuum::setup() {
	pinMode(pin, OUTPUT); 
	disable();  // vacuum is enabled-low, so it will be on at boot
}

void Vacuum::enable() {
	digitalWrite(pin, LOW);
}

void Vacuum::disable() {
	digitalWrite(pin, HIGH);
}

DirtRelease::DirtRelease(int pin) : pin(pin) { }

// ========== DirtRelease ==========

void DirtRelease::setup() {
	servo.attach(pin);
	servo.write(0);
}

void DirtRelease::moveBy(int degrees) {
	angle += degrees;
	angle = constrain(angle, 0, 90);
	servo.write(angle);
}

void DirtRelease::open() {
	servo.write(DIRT_RELEASE_OPEN_ANGLE);
}

void DirtRelease::close() {
	servo.write(DIRT_RELEASE_CLOSE_ANGLE);
}
