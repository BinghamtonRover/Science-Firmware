#include "co2_sensor.h"

#define CO2_READ_DELAY 20

// The Teensy has two I2C interfaces: Wire and Wire1.
// This lets us easily switch between them.
#define WIRE Wire1

/// The constant offset to add to the CO2 sensor's readings.
///
/// The CO2 sensor is *precise*, but not accurate. Meaning, it will accurately detect spikes and
/// dips, but the actual value it reports may not be accurate. A simple fix is to just add a 
/// constant amount to all its readings, preserving the shape while fixing the accuracy.
const int co2Offset = 10;

Co2Sensor::Co2Sensor(int address) : address(address) {}

void Co2Sensor::setup() {
  WIRE.begin();
}

bool Co2Sensor::hasError() {
  // The exact cases are:
  // - 1: ok
  // - 2: failed
  // - 254: pending
  // - 255: no data
  // For now we can just check for any error at all.
  return WIRE.read() != 1;
}

int Co2Sensor::read() {
  // Send the 'r' (read) command to the CO2 sensor.
  WIRE.beginTransmission(address);
  WIRE.write('r');
  WIRE.endTransmission();
  delay(CO2_READ_DELAY);

  // Read the response (a null-terminated string).
  WIRE.requestFrom(address, 20, 1);
  if (hasError()) return -1;
  int i = 0;
  char response[20];
  while (WIRE.available()) {
    char character = WIRE.read();
    response[i++] = character;
    if (character == 0) break;
  }

  // The response is an ASCII-encoded string of the ppm value.
  return atoi(response) + co2Offset;
}
