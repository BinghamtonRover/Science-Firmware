#pragma once
#include <Arduino.h>
#include <Servo.h>

#include "../science.pb.h"

class AugerServo {
    private:
      Servo servo;
      int servoPIN;
      int openPOS;
      int closePOS;

    public:    
      AugerServo(int servoPIN, int openPOS, int closePOS);
      void setup();
      void open();
      void close();
      bool attached();
      void handleCommand(ServoState servo_state);
};
