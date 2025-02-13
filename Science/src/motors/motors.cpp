#include "motors.h"

Motors::Motors(StepperMotor dirtCarousel, StepperMotor dirtLinear) : 
  dirtCarousel(dirtCarousel),
  dirtLinear(dirtLinear)
  { }

void Motors::setup() {
  dirtCarousel.presetup();
  dirtLinear.presetup();

  dirtCarousel.setup();
  dirtLinear.setup();
}

void Motors::calibrate() {
  dirtLinear.calibrate(); block();
  dirtCarousel.calibrate(); block();
}

void Motors::block() {
  dirtCarousel.block();
  dirtLinear.block();
}

void Motors::stop() {
  dirtCarousel.stop();
  dirtLinear.stop();
}

void Motors::update() {
  dirtCarousel.update();
  dirtLinear.update();
}

void Motors::handleCommand(ScienceCommand command) {
  if (command.carousel_motor != 0) dirtCarousel.moveBySteps(command.carousel_motor);
  if (command.subsurface_motor != 0) dirtLinear.moveBySteps(command.subsurface_motor);
}
