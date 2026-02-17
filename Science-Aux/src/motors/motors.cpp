#include "motors.h"

Motors::Motors(StepperMotor dirtCarousel) : 
  dirtCarousel(dirtCarousel)
  { }

void Motors::setup() {
  dirtCarousel.presetup();
  dirtCarousel.setup();
}

void Motors::calibrate() {
  dirtCarousel.calibrate(); block();
}

void Motors::block() {
  dirtCarousel.block();
}

void Motors::stop() {
  dirtCarousel.stop();
}

void Motors::update() {
  dirtCarousel.update();
}

void Motors::handleCommand(ScienceCommand command) {
  if (command.carousel_motor != 0) dirtCarousel.moveBySteps(command.carousel_motor);
}
