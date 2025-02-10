

//This code is for the "Arduino CO2 Meter" project.
//The CO2 Meter uses an Atlas Scientific Co2 sensor connected to an Arduino Uno and I2C LCD display.

//Project link: https://www.hackster.io/atlasscientific/arduino-co2-meter-c030ce

#include "src/Ezo_i2c.h"                         //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
// #include <sequencer2.h>                       //imports a 2 function sequencer. This controls the I2C read/write timing without blocking other operations    
#include "src/Ezo_i2c_util.h"                     //brings in common print statements

Ezo_board CO2 = Ezo_board(105, "CO2");        //create a CO2 circuit object, whose address is 105 and name is "CO2"

void setup() {
  Serial.begin(9600);                         //enables the serial port and sets the baud rate to 9600
}

void loop() {
  CO2.send_read_cmd();  
  delay(1000);
  receive_and_print_reading(CO2);             //get the reading from the CO2 sensor
  Serial.println();                           //and send it out through the serial monitor (this is for debugging, you do not need to do this)
}
