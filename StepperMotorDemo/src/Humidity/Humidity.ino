#include "src/HumiditySensor.h"

#define HUMPIN 15

HumiditySensor humSensor = HumiditySensor(HUMPIN);

void setup() {
  Serial.begin(9600);
  humSensor.setup();
  Serial.clear();
}
void loop() {
  Serial.print("temp(C)");
  Serial.println(humSensor.readTemperature());
  Serial.print("humidity(%)");
  Serial.println(humSensor.readHumidity());
  Serial.println("------------");
  delay(5000);
}
