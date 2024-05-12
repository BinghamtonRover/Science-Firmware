#include "pumps.h"

const int pumpDelay = 5000;  // milliseconds

void Pumps::setup() {
  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  pinMode(PUMP3, OUTPUT);
  pinMode(PUMP4, OUTPUT);
  pinMode(PUMP5, OUTPUT);
  turnOff();
}

void Pumps::turnOn() {
  digitalWrite(PUMP1, HIGH);
  digitalWrite(PUMP2, HIGH);
  digitalWrite(PUMP3, HIGH);
  digitalWrite(PUMP4, HIGH);
  digitalWrite(PUMP5, HIGH);
}

void Pumps::turnOff() {
  digitalWrite(PUMP1, LOW);
  digitalWrite(PUMP2, LOW);
  digitalWrite(PUMP3, LOW);
  digitalWrite(PUMP4, LOW);
  digitalWrite(PUMP5, LOW);
}

void Pumps::handleCommand(ScienceCommand command) {
  if (command.pump1 == PumpState_PUMP_ON) {
    turnOn();
  } else if (command.pump1 == PumpState_PUMP_OFF) {
    turnOff();
  }
}

void Pumps::fillTubes() {
  turnOn();
  delay(pumpDelay);
  turnOff();
}
