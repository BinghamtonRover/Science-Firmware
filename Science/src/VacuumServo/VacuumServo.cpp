#include "VacuumServo.h"

void VacuumServo::setup() { 
  servo.attach(pin);
}

void VacuumServo::open() {
	for (int pos = 0; pos <= 180; pos += 1) { 
    servo.write(pos);              
    delay(15); 
	}
};

void VacuumServo::close() {
	for (int pos = 180; pos >= 0; pos -= 1) { 
    servo.write(pos);              
    delay(15); 
  }
};

void VacuumServo::write(int angle) { 
  servo.write(angle); 
}