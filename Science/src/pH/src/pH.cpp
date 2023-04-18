#include "pH.h"

pHSensor::pHSensor(int PIN){
  pHPIN = PIN;
}

void pHSensor::setup(){
  pH.begin();
}

void pHSensor::sample_pH(float temp){
  static unsigned long timepoint = millis();
  float voltage = 0;
  float phValue = 0;
  float temperature = 25;
    if(millis()-timepoint>1000U){                  //time interval: 1s
        timepoint = millis();
        temperature = readTemperature(temp);           // read your temperature sensor to execute temperature compensation
        voltage = analogRead(pHPIN)/1024.0*5000;   // read the voltage
        phValue = pH.readPH(voltage,temperature);  // convert voltage to pH with temperature compensation
        Serial.print("temperature:");
        Serial.print(temperature,1);
        Serial.print("^C  pH:");
        Serial.println(phValue,2);
    }
    pH.calibration(voltage,temperature);
}

float pHSensor::readTemperature(float temp)
{
  return temp;
}
