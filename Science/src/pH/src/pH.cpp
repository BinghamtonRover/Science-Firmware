#include "pH.h"

pHSensor::pHSensor(int PIN){
  pHPIN = PIN;
}

void pHSensor::setup(){
  pH.begin();
}

float pHSensor::sample_pH(){
  float voltage = 0;
  float pHvalue = 0;
  float temperature = 25;
  temperature = humsensor.readTemperature();           // read your temperature sensor to execute temperature compensation
  voltage = analogRead(pHPIN)/1024.0*5000;   // read the voltage
  pHvalue = pH.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
  Serial.print("temperature:");
  Serial.print(temperature,1);
  Serial.print("^C  pH:");
  Serial.println(pHvalue,2);
  return pHvalue;
}

