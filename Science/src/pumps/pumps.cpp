#include "pumps.h"

const int pumpDelay1 = 2000; //Not being used
const int pumpDelay2 = 6600; //H202 
const int pumpDelay3 = 3300; //FDA
const int pumpDelay4 = 10000; //Water
const int pumpDelay5 = 6000; //Buffer

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
  turnOn();

  delay(pumpDelay3);

  digitalWrite(PUMP3, LOW);

  delay(pumpDelay2 - pumpDelay3);

  digitalWrite(PUMP2, LOW);
  digitalWrite(, LOW);

  delay(pumpDelay4 - pumpDelay2);

  digitalWrite(PUMP4, LOW);

  turnOff();
}
