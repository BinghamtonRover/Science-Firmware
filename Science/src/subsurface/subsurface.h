#pragma once
#include <Arduino.h>
#include "Servo.h"


class SubSurfaceBox {

    private:
      Servo subSurfaceServo;
    public:
    
      void setup();
      void open();
      void close();
      void handleCommand(ScienceCommand command);
};

