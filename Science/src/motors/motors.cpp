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
  if (command.carousel_motor != 0) dirtCarousel.moveBySteps(command.carousel_motor);
  if (command.scoop_motor != 0) scooperArm.moveBySteps(command.scoop_motor);
  if (command.subsurface_motor != 0) dirtLinear.moveBySteps(command.subsurface_motor);
}
