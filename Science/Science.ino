//#include <BURT_utils.h>
#include "src/utils/BURT_utils.h"

// Hardware code
#include "src/CO2/src/CO2Sensor.h"
#include "src/Methane/src/MethaneSensor.h"
#include "src/pH/src/pH.h"
#include "src/Humidity/src/HumiditySensor.h"
#include "src/Vacuum/Vacuum.h"

// Contains all the StepperMotor and DCMotor objects.
#include "src/pinouts.h"
// Contains all the Protobuf data
#include "src/science.pb.h"

#define SCIENCE_COMMAND_ID 0x43
#define SCIENCE_DATA_ID 0x17

#define USE_SERIAL_MONITOR false

void scienceHandler(const uint8_t* data, int length);
BurtSerial serial(scienceHandler, Device::Device_SCIENCE);
BurtCan can(SCIENCE_COMMAND_ID, scienceHandler);

#define PH_PIN 14
#define METHANE_PIN 16
#define HUM_PIN 15
#define CO2_PIN 17

//measurements in steps, used for testing sequence
#define DIRT_CAROUSEL_INSERT_TESTS -31000
#define DIRT_CAROUSEL_POUR 0
#define DIRT_CAROUSEL_NEXT_SECTION -17000
#define DIRT_CAROUSEL_NEXT_TUBE -4300
#define DIRT_CAROUSEL_PICTURE 0

#define DIRT_LINEAR_POUR_OUTER_HOLES -500000
#define DIRT_LINEAR_POUR_INNER_HOLE -700000
#define DIRT_LINEAR_INSERT_TESTS -100000
#define DIRT_LINEAR_PICTURE -650000

#define SCIENCE_LINEAR_INSERT_TESTS 500000

//pump speed
#define PUMP_START -100

//delays in ms
#define DIRT_RELEASE_DELAY 200
#define BLOCK_DELAY 10
#define PUMP_DELAY 5000

#define R_0 945

MethaneSensor methanesensor = MethaneSensor(METHANE_PIN, R_0);
HumiditySensor humsensor = HumiditySensor(HUM_PIN);
CO2Sensor co2 = CO2Sensor();
pHSensor pH = pHSensor(PH_PIN, humsensor);

ScienceState state = ScienceState_STOP_COLLECTING;

int canSendInterval = 3000;
unsigned long nextSendTime;

int sample_number = 0;

void setup() {
	Serial.begin(9600);
  Serial.println("Initializing...");

  Serial.println("Initializing vacuum...");
  vacuum.setup();
  dirtRelease.setup();

  Serial.println("Initializing communications...");
  can.setup();
  nextSendTime = millis() + canSendInterval;

  Serial.println("Initializing stepper motors...");
  vacuumLinear.presetup();
  dirtLinear.presetup();
  scienceLinear.presetup();
  dirtCarousel.presetup();

	vacuumLinear.setup();
  dirtLinear.setup();
  scienceLinear.setup();
  dirtCarousel.setup();

  Serial.println("Calibrating motors...");
  vacuumLinear.calibrate();
  scienceLinear.calibrate();
  dirtLinear.calibrate();
  dirtCarousel.calibrate();

  Serial.println("Initializing pumps...");
  pump1.setup();
  pump2.setup();
  pump3.setup();
  pump4.setup();

  Serial.println("Initializing sensors...");
  humsensor.setup();
  co2.setup();
  pH.setup();

	Serial.println("Science Subsystem ready.");
}

void loop() {
  /* Real Rover code */
  vacuumLinear.update();
  dirtLinear.update();
  scienceLinear.update();
  dirtCarousel.update();

  can.update();
  sendData();

  if (USE_SERIAL_MONITOR) parseSerialCommand();
  else serial.update();
}

/* Temporary Serial Monitor interface for testing. */
void parseSerialCommand() {
  String input = Serial.readString();
  int delimiter = input.indexOf(" ");
  if (delimiter == -1) return;
  String part1 = input.substring(0, delimiter);
  String part2 = input.substring(delimiter + 1);
  String motor = part1;
  float distance = part2.toFloat();
  int speed = part2.toInt();

  // Execute the command
  if (motor == "vacuum-linear"){ 
    Serial.println("moving vacuum linear");
    vacuumLinear.moveBy(distance);  
  }
  else if (motor == "stop") stopEverything();
  else if (motor == "calibrate") calibrateEverything();
  else if (motor == "dirt-linear") dirtLinear.moveBy(distance); //dirtLinear.moveBy(distance);  
  else if (motor == "science-linear") scienceLinear.moveBy(distance); //scienceLinear.moveBy(distance);  
  else if (motor == "dirt-carousel") dirtCarousel.moveBy(distance); //dirtCarousel.moveBy(distance);  
  else if (motor == "vacuum_on") vacuum.enable();
  else if (motor == "vacuum_off") vacuum.disable();
  else if (motor == "dirt-release") dirtRelease.moveBy(distance); //go +49 to uncover hole, -49 to go back
  else if (motor == "science-test") test_sample(distance);
  else if (motor == "pour-dirt") pourDirt(distance);
  else if (motor == "pump1") {
    pump1.setSpeed(speed);
    delay(PUMP_DELAY);
    pump1.hardBrake();
  } else if (motor == "pump2") {
    pump2.setSpeed(speed);
    delay(PUMP_DELAY);
    pump2.hardBrake();
  } else if (motor == "pump3") {
    pump3.setSpeed(speed);
    delay(PUMP_DELAY);
    pump3.hardBrake();
  } else if (motor == "pump4") {
    Serial.println(speed);
    pump4.setSpeed(speed);
    delay(PUMP_DELAY);
    pump4.hardBrake();
  } else {
    Serial.println("Command not recognized: " + input);
    Serial.println("  Commands are of the form: motor-name distance/speed.");
    Serial.println("  For stepper motors: positive distance is away from the limit switch.");
    Serial.println("  For DC motors: speed is in the range [-100, 100]. Positive = clockwise.");
    Serial.println("  For example: vacuum-linear 20, would move the vacuum 20mm up.");
    Serial.println("  Example 2: auger 100 spins the auger at full speed.");
    Serial.println("");
  }
}

void scienceHandler(const uint8_t* data, int length) {
  ScienceCommand command = BurtProto::decode<ScienceCommand>(data, length, ScienceCommand_fields);
  // Individual motor control
  if (command.dirt_carousel != 0) dirtCarousel.moveBy(command.dirt_carousel);
  if (command.dirt_linear != 0) dirtLinear.moveBy(command.dirt_linear);
  if (command.science_linear != 0) scienceLinear.moveBy(command.science_linear);
  if (command.vacuum_linear != 0) vacuumLinear.moveBy(command.vacuum_linear);

  // Pumps
  pump1.handleCommand(command.pump1);
  pump2.handleCommand(command.pump2);
  pump3.handleCommand(command.pump3);
  pump4.handleCommand(command.pump4);
  vacuum.handleCommand(command.vacuum);
  dirtRelease.handleCommand(command.dirtRelease);

  // Commands
  if (command.stop) stopEverything();
  else if (command.calibrate) calibrateEverything();
  if (command.next_tube) dirtCarousel.moveBy(PI / 6); 
  if (command.next_section) dirtCarousel.moveBy(2 * PI / 3);
  if (command.sample != 0) sample_number = command.sample - 1;
  if (command.state == ScienceState_COLLECT_DATA) {
    if (state == ScienceState_STOP_COLLECTING) test_sample(sample_number);
    state = command.state;
  }
}

void calibrateEverything() {
  scienceLinear.calibrate(); block();
  dirtLinear.calibrate(); block();
  dirtCarousel.calibrate(); block();
  vacuumLinear.calibrate(); block();
}

void stopEverything() {
  dirtCarousel.stop();
  dirtLinear.stop();
  scienceLinear.stop();
  vacuumLinear.stop();
  vacuum.disable();
  pump1.setSpeed(0);
  pump2.setSpeed(0);
  pump3.setSpeed(0);
  pump4.setSpeed(0);
}

void sendData() {
  // Send generic data
  if (millis() < nextSendTime) return;
  ScienceData data = ScienceData_init_zero;
  data.sample = sample_number;
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.state = state;
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  // Send sensor data
  if (state != ScienceState_COLLECT_DATA) return;
  data = ScienceData_init_zero;
  data.methane = methanesensor.getMethanePPM();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.co2 = co2.getPercentage();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.humidity = humsensor.readHumidity();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.temperature = humsensor.readTemperature();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.pH = pH.sample_pH();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  nextSendTime = millis() + canSendInterval;
}

void block() {
  while (vacuumLinear.isMoving()) delay(BLOCK_DELAY);
  while (scienceLinear.isMoving()) delay(BLOCK_DELAY);
  while (dirtLinear.isMoving()) delay(BLOCK_DELAY);
  while (dirtCarousel.isMoving()) delay(BLOCK_DELAY);
}

void pourDirt(int ms) {
  dirtRelease.open();
  delay(ms);
  dirtRelease.close();
}

/// The vacuum should be lowered manually. This function sucks dirt to fill the canister, then
/// pours it into each hole of the dirt carousel, being careful to spill only a bit in each tube.
/// Then, drops the science tests into the tubes.
void test_sample(int sample) {
  // Move everything to the zero position
  calibrateEverything();

  // Align the dirt carousel for pouring into the first hole
  dirtCarousel.moveTo(DIRT_CAROUSEL_POUR); block();  
  for (int i = 0; i < sample; i++) {
    dirtCarousel.moveBy(DIRT_CAROUSEL_NEXT_SECTION); block();
  }
  dirtLinear.moveTo(DIRT_LINEAR_POUR_OUTER_HOLES); block();

  // Pour into each outer hole
  for (int outerHole = 0; outerHole < 4; outerHole++) {
    if (outerHole > 0) dirtCarousel.moveBy(DIRT_CAROUSEL_NEXT_TUBE); block();
    dirtRelease.open();
    delay(DIRT_RELEASE_DELAY);
    dirtRelease.close();
  }

  // Pour into the middle hole (aligned with the third hole)
  dirtCarousel.moveBy(-DIRT_CAROUSEL_NEXT_TUBE); block();
  dirtLinear.moveTo(DIRT_LINEAR_POUR_INNER_HOLE); block();
  dirtRelease.open();
  delay(DIRT_RELEASE_DELAY);
  dirtRelease.close();

  // Drop the science tests into the tubes
  dirtLinear.moveTo(DIRT_LINEAR_INSERT_TESTS); block();
  dirtCarousel.moveTo(DIRT_CAROUSEL_INSERT_TESTS); block();
  if (sample == 1)
  { dirtCarousel.moveBy(DIRT_CAROUSEL_NEXT_SECTION); 
    block();
  }
  else if (sample == 2) 
  { dirtCarousel.moveBy(-DIRT_CAROUSEL_NEXT_SECTION); 
    block();
  }
  scienceLinear.moveTo(SCIENCE_LINEAR_INSERT_TESTS); block();
  // TODO: Re-enable the below
  
  /*
  // Pour the test fluids into the tubes
  pump1.setSpeed(PUMP_START);
  pump1.setSpeed(PUMP_START);
  pump1.setSpeed(PUMP_START);
  pump1.setSpeed(PUMP_START);
  delay(PUMP_DELAY);
  pump1.hardBrake();
  pump1.hardBrake();
  pump1.hardBrake();
  pump1.hardBrake();
  */

  // Move the dirt carousel to its picture position:
  // - dirt linear as far back as possible
  // - dirt carousel so that the clear tube is in front of camera
  /*
  scienceLinear.moveTo(0); block();
  dirtCarousel.moveBy(-DIRT_CAROUSEL_NEXT_TUBE); block();
  dirtLinear.moveTo(DIRT_LINEAR_PICTURE); block();
  */
}
