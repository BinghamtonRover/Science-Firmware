#include "src/MethaneSensor.h"


#define methanePIN 16
#define R_0 945 //Change this value

MethaneSensor Methane = MethaneSensor(methanePIN, R_0);

void setup() {
  Serial.begin(9600);
}
void loop() {
 Serial.println(Methane.getMethanePPM());
}
