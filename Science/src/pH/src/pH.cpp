#include "pH.h"

pHSensor::pHSensor(int PIN){
  pHPIN = PIN;
}

void pHSensor::setup(){
  pH.begin();
}

void pHSensor::sample_pH(){
  static unsigned long timepoint = millis();
  float voltage = 0;
  float pHvalue = 0;
  float temperature = 25;
    if(millis()-timepoint>1000U){                  //time interval: 1s
        timepoint = millis();
        temperature = humsensor.readTemperature();           // read your temperature sensor to execute temperature compensation
        voltage = analogRead(pHPIN)/1024.0*5000;   // read the voltage
        pHvalue = pH.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        Serial.print("temperature:");
        Serial.print(temperature,1);
        Serial.print("^C  pH:");
        Serial.println(pHvalue,2);
    }
    pH.calibration(voltage,temperature);
}

float pHSensor::returnpH()
{
  return pHvalue;
}
