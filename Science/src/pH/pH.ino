#include "src/pH.h"


#define pHPIN 14

pHSensor mySensor = pHSensor(pHPIN);

void setup() {
  Serial.begin(9600);
}
void loop() {
  mySensor.sample_pH();
}
