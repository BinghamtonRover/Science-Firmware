#pragma once
#include <Arduino.h>
#include <Servo.h>

#include "../science.pb.h"

class AugerServo {
    private:
      Servo servo;
      int servoPIN;

    public:    
      AugerServo(int servoPIN);
      void setup();
      void open();
      void close();
      bool attached();
      void handleCommand(ScienceCommand command);
};
