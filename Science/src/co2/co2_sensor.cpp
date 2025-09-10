#include "co2_sensor.h"

// The Teensy has two I2C interfaces: Wire and Wire1.
// This lets us easily switch between them.
#define WIRE Wire1

Co2Sensor::Co2Sensor(int address) : address(address) {}

void Co2Sensor::setup() {
  WIRE.begin();
}

int Co2Sensor::checkError() {
  // The exact cases are:
  // - 1: ok
  // - 2: failed
  // - 254: pending
  // - 255: no data
  return WIRE.read();
}

float Co2Sensor::read() {
  // Send the 'r' (read) command to the CO2 sensor.
  WIRE.beginTransmission(address);
  WIRE.write('r');
  WIRE.endTransmission();

  // Read the response (a null-terminated string).
  WIRE.requestFrom(address, 20, 1);
  int error = checkError();
  if (error != 1) {
    Serial.print("CO2 Error: ");
    Serial.println(error);
    return -1;
  }
  int i = 0;
  char response[20];
  while (WIRE.available()) {
    char character = WIRE.read();
    response[i++] = character;
    if (character == 0) break;
  }

  // The response is an ASCII-encoded string of the ppm value.
  return atoi(response);
}
