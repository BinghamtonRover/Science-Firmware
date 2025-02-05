#include "co2_sensor.h"
#define time_ 500
#define WIRE Wire1

Co2Sensor::Co2Sensor(int address) : address(address) {}

void Co2Sensor::setup()
{
  WIRE.begin();
}

float Co2Sensor::read()
{
  byte code = 0;
  byte in_char = 0;
  int i = 0;
  char Co2_data[20];
  int Co2_int = 0;

  // i2c implementation
  WIRE.beginTransmission(address);  //call the circuit by its ID number.
  WIRE.write('r');
  WIRE.endTransmission();

  // if (strcmp(computerdata, "sleep") != 0) {  //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.
                                             //if it is the sleep command, we do nothing. Issuing a sleep command and then requesting data will wake the Co2 sensor.

    delay(time_);  //wait the correct amount of time for the circuit to complete its instruction.

    WIRE.requestFrom(address, 20, 1);  //call the circuit and request 20 bytes (this may be more than we need)
    code = WIRE.read();                //the first byte is the response code, we read this separately.

    switch (code) {                 //switch case based on what the response code is.
      case 1:                       //decimal 1.
        Serial.println("Success");  //means the command was successful.
        break;                      //exits the switch case.

      case 2:                      //decimal 2.
        Serial.println("Failed");  //means the command has failed.
        break;                     //exits the switch case.

      case 254:                     //decimal 254.
        Serial.println("Pending");  //means the command has not yet been finished calculating.
        break;                      //exits the switch case.

      case 255:                     //decimal 255.
        Serial.println("No Data");  //means there is no further data to send.
        break;                      //exits the switch case.
    }


    Serial.print("Available Bytes: ");
    Serial.println(WIRE.available());
    while (WIRE.available()) {  //are there bytes to receive.
      in_char = WIRE.read();
      Co2_data[i] = in_char;  //load this byte into our array.
      i += 1;                 //incur the counter for the array element.
      if (in_char == 0) {     //if we see that we have been sent a null command.
        i = 0;                //reset the counter i to 0.
        break;                //exit the while loop.
      }
    }
  // }

  //Uncomment this section if you want to take the Co2 value and convert it into integer number.
  Co2_int = atoi(Co2_data);
  return Co2_int;

}
