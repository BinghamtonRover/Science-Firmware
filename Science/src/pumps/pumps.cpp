#include "pumps.h"

const int pumpDelay1 = 6600; //H202 
const int pumpDelay2 = 3300; //FDA
const int pumpDelay3 = 10000; //Water
const int pumpDelay4 = 6000; //Buffer

void Pumps::setup() {
  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  pinMode(PUMP3, OUTPUT);
  pinMode(PUMP4, OUTPUT);
  turnOff();
}

void Pumps::turnOn() {
  digitalWrite(PUMP1, HIGH);
  digitalWrite(PUMP2, HIGH);
  digitalWrite(PUMP3, HIGH);
  digitalWrite(PUMP4, HIGH);
}

void Pumps::turnOff() {
  digitalWrite(PUMP1, LOW);
  digitalWrite(PUMP2, LOW);
  digitalWrite(PUMP3, LOW);
  digitalWrite(PUMP4, LOW);
}

void Pumps::handleCommand(ScienceCommand command) { 
  
  if (command.pumps == PumpState_PUMP_ON) {
    turnOn();
  } else if (command.pumps == PumpState_PUMP_OFF) {
    turnOff();
  } else if (command.pumps == PumpState_FILL) {
    fillTubes();
  }

}

void Pumps::fillTubes() {
  // Timings to be tested
  turnOn();
  delay(7000);
  // delay(pumpDelay3);

  // digitalWrite(PUMP2, LOW);

  // delay(pumpDelay3 - pumpDelay2);

  // digitalWrite(PUMP1, LOW);
  // digitalWrite(PUMP4, LOW);

  // delay(pumpDelay3 - pumpDelay1);

  // digitalWrite(PUMP3, LOW);

  turnOff();
}
