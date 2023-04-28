#include "src/CO2Sensor.h"

#define CO2PIN 17
#define _V400 .5 //Change these values
#define _V1000 1

CO2Sensor co2 = CO2Sensor(CO2PIN);

void setup() {
  Serial.begin(9600);
  co2.calibrate(_V400, _V1000);
}

void loop() {
  Serial.println(co2.readPPM());
  delay(500);
}
