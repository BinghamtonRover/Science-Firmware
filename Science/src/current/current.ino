// THIS FILE IS FOR TESTING CURRENT SENSOR, IT IS NOT A CLASS YET
#include <Arduino.h>
#include <math.h> // for fabsf
#include <float.h>

// Initialized Deadband
const float DB = 0.06f; // represents the threshold for noise

// State for the estimate current for the innovation formula 
bool inited = false; // detects whether there's an estimated current value (first setting it to false)
float s_hat = 0.0f; // filtered/estimated current value

float u90 = 0.5;  // if current spike (innovation) is (u90) Amps or greater, filter is almost fully open
float K = 1.4722 / u90;

const float alpha_min = 0.08f;
const float alpha_max = 0.4;

void setup()
{
  // put your setup code here, to run once:
  pinMode(23, INPUT); // sets the ADC pin   
  analogReadAveraging(4); // takes four sample voltage values and averages them to reduce noise
}

void loop()
{
  // put your main code here, to run repeatedly:
  float x = (((3.3 / 1023) * analogRead(23)) - 1.65) / 0.132; // Convert ADC --> volts --> amps 

  if (!inited) {
    s_hat = x; inited = true; // If an estimated current has been detected then set the state to true
  }
  /* Measuring the accuracy of the current with the Innovation Formula */
  float delta = x - s_hat; // measuring how far the new current is from the estimated current (amps)
  float a = fabsf(delta); 
  
  // Deadband Conditions to measure how large the difference is
  float delta_db;
  if (a <= DB) {
    delta_db = 0.0f;                   // ignore tiny noise if the noise is smaller than the threshold
  } else {
    float sign = (delta >= 0.0f) ? 1.0f : -1.0f; // If the noise outside of the threshold then it reduces it by 
    delta_db = sign * (a - DB);                  // the deadband threshold value so that the sensor doesn't react 
  }                                              // to the initial noise

  float f_delta_db = tanh(K * delta_db);  // this is openness variable
  float f01 = 0.5f * (f_delta_db + 1.0f);
  float alpha = alpha_min + (alpha_max - alpha_min) * f01;
  s_hat = s_hat + alpha*(delta);  // update the estimate

  Serial.print(x);
  Serial.print(",");
  Serial.println(s_hat);
}