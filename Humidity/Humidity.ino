#include "src/DFRobot_SHT3x/DFRobot_SHT3x.h"

// Teensy 4.1 pin numbers
// Wire = 18 19
// Wire1 = 17 16
// Wire2 = 25 24

DFRobot_SHT3x sht3x(&Wire2,/*address=*/0x44,/*RST=*/4);

// DFRobot_SHT3x sht3x;

void setup() {

  Serial.begin(9600);
    //Initialize the chip to detect if it can communicate properly.
  while (sht3x.begin() != 0) {
    Serial.println("Failed to initialize the chip, please confirm the chip connection");
    delay(1000);
  }
  

  Serial.print("chip serial number: ");
  Serial.println(sht3x.readSerialNumber());

   if(!sht3x.softReset()){
     Serial.println("Failed to reset the chip");
   }


  //if(!sht3x.pinReset()){
    //Serial.println("Failed to reset the chip");
  //}


  //if(!sht3x.heaterEnable()){
    // Serial.println("Failed to turn on the heater");
  //}
         
  if(!sht3x.startPeriodicMode(sht3x.eMeasureFreq_1Hz)){
    Serial.println("Failed to enter the periodic mode");
  }
  Serial.println("------------------Read data in cycle measurement mode-----------------------");
}

void loop() {

  Serial.print("Ambient temperature(°C/F):");

  Serial.print(sht3x.getTemperatureC());
  Serial.print(" C/");

  Serial.print(sht3x.getTemperatureF());
  Serial.print(" F ");
  Serial.print("Relative humidity(%RH):");

  Serial.print(sht3x.getHumidityRH());
  Serial.println(" %RH");
  //Please adjust the frequency of reading according to the frequency of the chip collection data.
  //The frequency to read data must be greater than the frequency to collect the data, otherwise the returned data will go wrong.
  delay(100);
  if(millis() > 10000 && millis() < 10200){

    sht3x.stopPeriodicMode();
    Serial.println("Exited from the cycle measurement mode, enter the single measurement mode");
  }

}