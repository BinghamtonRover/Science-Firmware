#include "scooper.h"

#define SCOOP_OPEN 150
#define SCOOP_CLOSE 30

Scooper::Scooper(StepperMotor stepper, int servoPin) : 
  stepper(stepper),
  servoPin(servoPin)
  { }

void Scooper::setup() {
  servo.attach(servoPin);
}

void Scooper::handleCommand(ScienceCommand command) {
  switch (command.scoop) {
    case ServoState_SERVO_STATE_UNDEFINED: break;
    case ServoState_SERVO_OPEN: 
      open(); break;
    case ServoState_SERVO_CLOSE: 
      close(); break;
  }
}

void Scooper::rotateBy(float degrees) {
  stepper.moveBy(degrees);
}

void Scooper::rotateTo(float degrees) {
  stepper.moveTo(degrees);
}

void Scooper::open() {
  servo.write(SCOOP_OPEN);
}

void Scooper::close() {
  servo.write(SCOOP_CLOSE);
}
