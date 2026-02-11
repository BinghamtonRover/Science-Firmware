// THIS FILE IS FOR TESTING CURRENT SENSOR, IT IS NOT A CLASS YET
#include <Arduino.h>
#include <math.h> // for fabsf
#include <float.h>
#include <TimerOne.h>

// Initialized Deadband
const float DB = 0.5f; // represents the threshold for noise (Any difference smaller than +- 0.5 A is treated as noise and ignored)

// ISR Shared Variables
volatile int adcRaw = 0; // stores the most recent ADC pin reading
volatile bool newSample = false; // a flag that tells the main loop when new data is ready 

// ISR Timer --> every 1 milisecond the processor pauses and uses this function to execute the sudden noise being captured and returns to the main loop
void sampleISR() {
  adcRaw = analogRead(23);   // Read ADC at fixed interval --> guarantees that the data is sampled at a fixed and precise interval 
  newSample = true;          // Signal loop() that data is ready 
}

// State for the estimate current for the innovation formula 
bool inited = false; // detects whether there's an estimated current value (first setting it to false)
float s_hat = 0.0f; // filtered/estimated current value

float u90 = 0.5;  // if current spike (innovation) is (u90) Amps or greater, filter is almost fully open
float K = 1.4722 / u90; // controls how quickly the filter opens up when measured current suddenly changes 
const float alpha_min = 0.5f;
const float alpha_max = 0.5; // Controls how fast the filter reacts to the noisy current readings   
 
void setup() {
  pinMode(23, INPUT);

  analogReadAveraging(4);    // when analogread function is called, take 4 adc samples and return their average instead of one raw sample 
  Serial.begin(115200);

  // Timer1 → 1 kHz sampling rate
  Timer1.initialize(1000);   // 1000 µs = 1 ms
  Timer1.attachInterrupt(sampleISR);
}

// ================== Main Loop ================== //

void loop() {
  if (!newSample) return;    // Wait for ISR sample

  noInterrupts(); // Prevents sampleISR() from running temporarily 
  int raw = adcRaw; // copies the isr flagged value into the local variable 
  newSample = false; // A flag that signals loop when fresh data is available 
  interrupts();

  // ADC → Voltage → Current
  float x = (((3.3f / 1023.0f) * raw) - 1.65f) / 0.134f;

  // Initialize filter
  if (!inited) {
    s_hat = x;
    inited = true;
    return;
  }

  // Innovation
  float delta = x - s_hat;
  float a = fabsf(delta);
  float delta_db = 0.0f;
  float alpha = alpha_min;

  // Deadband + adaptive openness
  if (a > DB) {
    float sign = (delta >= 0.0f) ? 1.0f : -1.0f;
    delta_db = sign * (a - DB);

    float f_delta_db = tanh(K * fabsf(delta_db));
    float f01 = 0.5f * (f_delta_db + 1.0f);

    alpha = alpha_min + (alpha_max - alpha_min) * f01; 
  }

  // Filter update
  s_hat = s_hat + alpha * (x - s_hat);

  // Output (outside ISR)
  Serial.print(x, 2);
  Serial.print(", ");
  Serial.println(s_hat, 2); 
}
