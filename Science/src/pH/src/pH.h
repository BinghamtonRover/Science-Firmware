#ifndef pH_H
#define pH_H

#include <Arduino.h>
#define SAMPLING_INTERVAL 20
#define PRINT_INTERVAL 800
#define ARRAY_LENGTH 40
#define OFFSET 0.00

class pHSensor {
  public:
    pHSensor(int PIN);
    void sample_pH();
    double avergearray(int* arr, int number);
  private:
    int pHPIN;
    int pHArray[ARRAY_LENGTH];
    int pHArrayIndex=0;
};

#endif
