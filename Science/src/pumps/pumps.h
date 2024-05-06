#include  <Arduino.h>

#include "../science.pb.h"

#define PUMP1 21  // NOT PWM
#define PUMP2 22
#define PUMP3 23
#define PUMP4 0  // Avoid? On Serial1
#define PUMP5 25

class Pumps {
  public:
    void setup();
    void turnOn();
    void turnOff();
    void handleCommand(ScienceCommand command);
};
