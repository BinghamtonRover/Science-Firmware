#include <Arduino.h>
#include <MG811.h>//library for co2 sensor

float v400 = 4.535;     // change 4.353 to be value from co2 open air reading (from calibration code)
float v40000 = 3.206;   // change 3.206 to be value from co2 exhaled air reading (from calibration code)
MG811 co2Sensor = 21; //define Co2 Sensor pin
void setup() {
  Serial.begin(115200);
  co2Sensor.begin(v400, v40000);
  co2Sensor.calibrate();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
   int Co2Value = co2Sensor.read();
   Serial.println("Co2 Value is");
   Serial.println(Co2Value);
}
