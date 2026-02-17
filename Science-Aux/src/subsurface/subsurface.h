#pragma once
#include <Arduino.h>
#include <Servo.h>

#include "../science.pb.h"

class SubSurfaceBox {
    private:
      Servo subSurfaceServo;
      int servoPIN;

    public:    
      SubSurfaceBox(int servoPIN);
      void setup();
      void open();
      void close();
      bool attached();
      void handleCommand(ScienceCommand command);
};
