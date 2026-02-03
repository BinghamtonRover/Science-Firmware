#pragma once
#include <Arduino.h>

class Encoder{
    private:
        bool homed = false;
        int indexPin;
    public:
        bool isHomed();
        void setup();
        void setHome();
        Encoder(int indexPin);
}