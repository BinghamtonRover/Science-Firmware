#include "encoder.h"

bool Encoder::isHomed(){
    return homed;
}

void setHome(){
    homed = true;
}

void Encoder::setup(){
    pinmode(indexPin, INPUT);
}

Encoder::Encoder(int indexPin) : indexPin(indexPin) {}

// Usage: attachInterrupt(digitalPinToInterrupt(indexPin), [](){ <name>.setHome(); }, RISING);