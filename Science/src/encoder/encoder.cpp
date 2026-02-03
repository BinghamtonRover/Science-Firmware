#include "encoder.h"

bool Encoder::isHomed(){
    return homed;
}

void Encoder::setHome(){
    homed = true;
}

void Encoder::setup(){
    pinMode(indexPin, INPUT);
}

Encoder::Encoder(int indexPin) : indexPin(indexPin) {}

// Usage: attachInterrupt(digitalPinToInterrupt(indexPin), [](){ <name>.setHome(); }, RISING);