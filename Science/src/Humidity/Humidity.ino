#include "src/HumiditySensor.h"

#define HUMPIN 15

HumiditySensor humSensor = HumiditySensor(HUMPIN);

void setup() {
  Serial.begin(9600);
  humSensor.setup();
}
void loop() {
  Serial.println(humSensor.readTemperature());
  Serial.println(humSensor.readHumidity());
}
