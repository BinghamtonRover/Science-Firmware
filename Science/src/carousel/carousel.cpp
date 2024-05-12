#include "carousel.h"

const int tubesPerSection = 3;
const int num_sections = 4;
const int totalTubes = tubesPerSection * num_sections;
const int funnelOpen = 35;
const int funnelClose = 90;
const int pourDelay = 200;
const int testOffset = 15;

DirtCarousel::DirtCarousel(StepperMotor stepper, int dirtReleasePin) : 
  stepper(stepper),
  dirtReleasePin(dirtReleasePin)
  { }

void DirtCarousel::setup() {
  dirtRelease.attach(dirtReleasePin);
  goHome();
  closeFunnel();
}

void DirtCarousel::handleCommand(ScienceCommand command) {
  if (command.dirtRelease == DirtReleaseState::DirtReleaseState_OPEN_DIRT) {
    openFunnel();
  } else if (command.dirtRelease == DirtReleaseState::DirtReleaseState_CLOSE_DIRT) {
    closeFunnel();
  }

  if (command.next_tube) nextTube();
}

void DirtCarousel::goHome() {
  stepper.moveTo(0);
  stepper.block();
}

void DirtCarousel::nextTube() {
  stepper.moveBy(360 / totalTubes);
  stepper.block();
}

void DirtCarousel::nextSection() {
  for (int index = tubeIndex; index < tubesPerSection; index++) {
    nextTube();
  }
}

void DirtCarousel::goToSectionStart() {
  for (int index = tubeIndex; index > 0; index--) {
    prevTube();
  }
}

void DirtCarousel::prevTube() {
  stepper.moveBy(360 / totalTubes * -1);
  stepper.block();
}

void DirtCarousel::prevSection() {
  goToSectionStart();
  for (int i = 0; i < tubesPerSection; i++) {
    prevTube();
  }
}

void DirtCarousel::goToSection(int section) {
  goHome();
  for (int i = 0; i < section; i++) {
    nextSection();
  }
}

void DirtCarousel::openFunnel() {
  dirtRelease.write(funnelOpen);
}

void DirtCarousel::closeFunnel() {
  dirtRelease.write(funnelClose);
}

void DirtCarousel::fillTube() {
  openFunnel();
  delay(pourDelay);
  closeFunnel();
}

void DirtCarousel::fillSection() {
  // goToSectionStart();
  for (int i = 0; i < tubesPerSection; i++) {
    if (i != 0) nextTube();
    fillTube();
  }
}

void DirtCarousel::goToTests() {
  stepper.moveBy(testOffset); 
  stepper.block();
  nextSection();
}

void DirtCarousel::goToPicture() {
  stepper.moveBy(testOffset * -1);
  stepper.block();
  nextSection();
  nextTube();
}
