#include "src/TempSensor.h"
#define Index 0

TempSensor Temp = TempSensor(Index);

void setup() {
  Serial.begin(9600);
  Temp.begin();
}

void loop() {
  Temp.requestTemperatures();
  Serial.println(Temp.getTempCByIndex(Index));
  delay(1000);
}