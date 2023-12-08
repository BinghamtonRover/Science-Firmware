#ifndef METHANE_H
#define METHANE_H

#include <Arduino.h>

class MethaneSensor {
  public:
    MethaneSensor(int PIN, int R0);
    void calibrate();
    float getMethanePPM();
	

  private:
    int MethanePIN;
    int R_0;
};

#endif
