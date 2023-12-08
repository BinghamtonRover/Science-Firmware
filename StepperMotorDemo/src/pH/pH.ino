#include "src/pH.h"

#define pHPIN 14

pHSensor ph = pHSensor(pHPIN);
float temperature = 25;

void setup()
{
    Serial.begin(9600);  
    ph.setup();
}

void loop()
{
    ph.sample_pH(temperature);         
}

//use enterph to go into calibration mode
//use calph to start calibration (expose to either 7 or 4)
//use exitph to end calibration and save values
