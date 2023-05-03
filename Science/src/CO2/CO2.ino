#include "src/CO2Sensor.h"

CO2Sensor co2 = CO2Sensor();

void setup() {
  Serial.begin(9600);
}

void loop() {
  int percentage;
  float volts = co2.MGRead();
  Serial.print( "SEN0159:" );
    Serial.print(volts);
    Serial.print( "V           " );

    percentage = co2.getPercentage();
    Serial.print("CO2:");
    if (percentage == -1) {
        Serial.print( "<400" );
    } else {
        Serial.print(percentage);
    }

    Serial.print( "ppm" );
    Serial.print("\n");

    if (digitalRead(BOOL_PIN) ){
        Serial.print( "=====BOOL is HIGH======" );
    } else {
        Serial.print( "=====BOOL is LOW======" );
    }

    Serial.print("\n");

    delay(500);
}
