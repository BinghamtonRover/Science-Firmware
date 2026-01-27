#include  <Arduino.h>

#include "../science.pb.h"

#define PUMP1 21
#define PUMP2 22
#define PUMP3 23
#define PUMP4 39
#define PUMP5 38

class Pumps {
  public:
    void setup();
    void turnOn();
    void turnOff();
    void handleCommand(ScienceCommand command);
    void fillTubes();
};
