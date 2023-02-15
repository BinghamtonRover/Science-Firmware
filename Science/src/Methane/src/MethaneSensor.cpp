#include "MethaneSensor.h"

MethaneSensor:: MethaneSensor(int PIN, int R0) {
  MethanePIN = PIN;
  R_0 = R0;
}

float MethaneSensor::getMethanePPM() {
  float a0 = analogRead(MethanePIN); // get raw reading from sensor
  float v_o = a0 * 5 / 1023; // convert reading to volts
  float R_S = (5-v_o) * 1000 / v_o; // apply formula for getting RS
  float PPM = pow(R_S/R_0,-2.95) * 1000; //apply formula for getting PPM
  return PPM;
}
