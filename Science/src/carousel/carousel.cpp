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
  switch (command.funnel) {
    case ServoState_SERVO_STATE_UNDEFINED: break;
    case ServoState_SERVO_OPEN: 
      openFunnel(); break; 
    case ServoState_SERVO_CLOSE: 
      closeFunnel(); break;
  }
  switch (command.carousel) {
    case CarouselCommand_CAROUSEL_COMMAND_UNDEFINED: break;
    case CarouselCommand_NEXT_TUBE:
      nextTube(); break;
    case CarouselCommand_PREV_TUBE: 
      prevTube(); break;
    case CarouselCommand_NEXT_SECTION: 
      nextSection(); break;
    case CarouselCommand_PREV_SECTION: 
      prevSection(); break;
    case CarouselCommand_FILL_TUBE: 
      fillTube(); break;
    case CarouselCommand_FILL_SECTION: 
      fillSection(); break;
  }
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
