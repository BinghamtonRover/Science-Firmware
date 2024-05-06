#include <Servo.h>


//#include <BURT_utils.h>
#include "src/utils/BURT_utils.h"

// Hardware code
#include "src/CO2/CO2Sensor.h"

#include "pinouts.h"
#include "src/science.pb.h"

#define SCIENCE_COMMAND_ID 0x43
#define SCIENCE_DATA_ID 0x17

#define USE_SERIAL_MONITOR false

void scienceHandler(const uint8_t* data, int length);
void sendData();
void shutdown() { }
BurtSerial serial(Device::Device_SCIENCE, scienceHandler, shutdown);
BurtCan<Can3> can(SCIENCE_COMMAND_ID, Device::Device_SCIENCE, scienceHandler, shutdown);
BurtTimer dataTimer(250, sendData);

#define CO2_PIN 17

//measurements in steps, used for testing sequence
#define DIRT_CAROUSEL_INSERT_TESTS -31000
#define DIRT_CAROUSEL_POUR 0
#define DIRT_CAROUSEL_NEXT_SECTION -17000
#define DIRT_CAROUSEL_NEXT_TUBE 4600
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

#define FUNNEL_OPEN 35
#define FUNNEL_CLOSE 90

CO2Sensor co2 = CO2Sensor(17);

ScienceState state = ScienceState_STOP_COLLECTING;

int sample_number = 0;

Servo servo1;
void block();
void stopEverything();
void test_sample();
void calibrateEverything();

void setup() {
	Serial.begin(9600);
  Serial.println("Initializing...");

  Serial.println("Initializing communications...");
  can.setup();

  Serial.println("Initializing stepper motors...");
  vacuumLinear.presetup();
  dirtLinear.presetup();
  scoopArmMotor.presetup();
  dirtCarousel.presetup();

	vacuumLinear.setup();
  dirtLinear.setup();
  scoopArmMotor.setup();
  dirtCarousel.setup();

  // Serial.println("Calibrating motors...");
  // vacuumLinear.calibrate();
  // scoopArmMotor.calibrate();
  // dirtLinear.calibrate();
  // dirtCarousel.calibrate();

  Serial.println("Initializing other hardware...");
  scooper.setup();
  pumps.setup();
  servo1.attach(SERVO1);
  servo1.write(FUNNEL_CLOSE);

  Serial.println("Initializing sensors...");
  co2.setup();

	Serial.println("Science Subsystem ready.");
}

void loop() {
  /* Real Rover code */
  vacuumLinear.update();
  dirtLinear.update();
  scoopArmMotor.update();
  dirtCarousel.update();

  can.update();
  serial.update();
  sendData();
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
  // int speed = part2.toInt();

  // Execute the command
  if (motor == "vacuum-linear"){ 
    Serial.println("moving vacuum linear");
    vacuumLinear.moveBy(distance); 
  }
  else if (motor == "stop") stopEverything();
  // else if (motor == "calibrate") calibrateEverything();
  else if (motor == "dirt-linear") dirtLinear.moveBy(distance); //dirtLinear.moveBy(distance);  
  else if (motor == "science-linear") scoopArmMotor.moveBy(distance); //scoopArmMotor.moveBy(distance);  
  else if (motor == "dirt-carousel") dirtCarousel.moveBy(distance); //dirtCarousel.moveBy(distance);  
  // else if (motor == "dirt-release") dirtRelease.moveBy(distance); //go +49 to uncover hole, -49 to go back
  // else if (motor == "science-test") test_sample(distance);
  // else if (motor == "pour-dirt") pourDirt(distance);
  else if (motor == "pinch") servo1.write(distance);
  else if (motor == "pump") {
    pumps.turnOn();
    delay(PUMP_DELAY);
    pumps.turnOff();
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
  if (command.science_linear != 0) scoopArmMotor.moveBy(command.science_linear);
  // if (command.dirt_linear != 0) dirtLinear.moveBy(command.dirt_linear);
  // if (command.vacuum_linear != 0) vacuumLinear.moveBy(command.vacuum_linear);

  // Pumps
  pumps.handleCommand(command);

  // Dirt release
  if (command.dirtRelease == DirtReleaseState::DirtReleaseState_OPEN_DIRT) {
    servo1.write(FUNNEL_OPEN);
  } else if (command.dirtRelease == DirtReleaseState::DirtReleaseState_CLOSE_DIRT) {
    servo1.write(FUNNEL_CLOSE);
  }
  // Scooper
  if (command.pump2 == PumpState::PumpState_PUMP_ON) {
    scooper.open();
  } else if (command.pump2 == PumpState::PumpState_PUMP_OFF) {
    scooper.close();
  }

  // Commands
  if (command.stop) stopEverything();
  // else if (command.calibrate) calibrateEverything();
  // if (command.next_tube) dirtCarousel.moveBy(PI / 6); 
  // if (command.next_section) dirtCarousel.moveBy(2 * PI / 3);
  // if (command.sample != 0) sample_number = command.sample - 1;
  // if (command.state == ScienceState_COLLECT_DATA) {
    // if (state == ScienceState_STOP_COLLECTING) test_sample(sample_number);
    // state = command.state;
  // }
}

void calibrateEverything() {
  scoopArmMotor.calibrate(); block();
  dirtLinear.calibrate(); block();
  dirtCarousel.calibrate(); block();
  vacuumLinear.calibrate(); block();
}

void stopEverything() {
  dirtCarousel.stop();
  dirtLinear.stop();
  scoopArmMotor.stop();
  vacuumLinear.stop();
  pumps.turnOff();
}

void sendData() {
  // Send generic data
  ScienceData data;
  // ScienceData data = ScienceData_init_zero;
  // data.sample = sample_number;
  // can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  // data = ScienceData_init_zero;
  // data.state = state;
  // can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.co2 = co2.read();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  serial.send(ScienceData_fields, &data, 8);

  data = ScienceData_init_zero;
  // data.humidity = read_humidity();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  serial.send(ScienceData_fields, &data, 8);

  data = ScienceData_init_zero;
  // data.temperature = read_temperature();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  serial.send(ScienceData_fields, &data, 8);
}

void block() {
  while (vacuumLinear.isMoving()) delay(BLOCK_DELAY);
  while (scoopArmMotor.isMoving()) delay(BLOCK_DELAY);
  while (dirtLinear.isMoving()) delay(BLOCK_DELAY);
  while (dirtCarousel.isMoving()) delay(BLOCK_DELAY);
}

// void pourDirt(int ms) {
//   dirtRelease.open();
//   delay(ms);
//   dirtRelease.close();
// }

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
  // for (int outerHole = 0; outerHole < 4; outerHole++) {
  //   if (outerHole > 0) dirtCarousel.moveBy(DIRT_CAROUSEL_NEXT_TUBE); block();
  //   // dirtRelease.open();
  //   delay(DIRT_RELEASE_DELAY);
  //   // dirtRelease.close();
  // }

  // Pour into the middle hole (aligned with the third hole)
  dirtCarousel.moveBy(-DIRT_CAROUSEL_NEXT_TUBE); block();
  dirtLinear.moveTo(DIRT_LINEAR_POUR_INNER_HOLE); block();
  // dirtRelease.open();
  delay(DIRT_RELEASE_DELAY);
  // dirtRelease.close();

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
  scoopArmMotor.moveTo(SCIENCE_LINEAR_INSERT_TESTS); block();
  // TODO: Re-enable the below
  
  /*
  // Pour the test fluids into the tubes
  pumps.turnOn();
  delay(PUMP_DELAY);
  pumps.turnOff();
  */

  // Move the dirt carousel to its picture position:
  // - dirt linear as far back as possible
  // - dirt carousel so that the clear tube is in front of camera
  /*
  scoopArmMotor.moveTo(0); block();
  dirtCarousel.moveBy(-DIRT_CAROUSEL_NEXT_TUBE); block();
  dirtLinear.moveTo(DIRT_LINEAR_PICTURE); block();
  */
}
