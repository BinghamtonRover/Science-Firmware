#include "motors.h"

Motors::Motors(StepperMotor scooperArm, StepperMotor dirtCarousel, StepperMotor dirtLinear) : 
  scooperArm(scooperArm),
  dirtCarousel(dirtCarousel),
  dirtLinear(dirtLinear)
  { }

void Motors::setup() {
  scooperArm.presetup();
  dirtCarousel.presetup();
  dirtLinear.presetup();

  scooperArm.setup();
  dirtCarousel.setup();
  dirtLinear.setup();
}

void Motors::calibrate() {
  scooperArm.calibrate(); block();
  dirtLinear.calibrate(); block();
  dirtCarousel.calibrate(); block();
}

void Motors::block() {
  scooperArm.block();
  dirtCarousel.block();
  dirtLinear.block();
}

void Motors::stop() {
  scooperArm.stop();
  dirtCarousel.stop();
  dirtLinear.stop();
}

void Motors::update() {
  scooperArm.calibrate();
  dirtCarousel.calibrate();
  dirtLinear.calibrate();
}

void Motors::handleCommand(ScienceCommand command) {
  if (command.dirt_carousel != 0) dirtCarousel.moveBy(command.dirt_carousel);
  if (command.science_linear != 0) scooperArm.moveBy(command.science_linear);
  if (command.dirt_linear != 0) dirtLinear.moveBy(command.dirt_linear);
}

