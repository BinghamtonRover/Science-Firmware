#include <DFRobot_SHT3x.h>


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

#define USE_SERIAL_MONITOR true

void scienceHandler(const uint8_t* data, int length);
void shutdown() { }
BurtSerial serial(Device::Device_SCIENCE, scienceHandler, shutdown);
BurtCan<Can3> can(SCIENCE_COMMAND_ID, Device::Device_SCIENCE, scienceHandler, shutdown);

DFRobot_SHT3x sht3x(&Wire,/*address=*/0x44,/*RST=*/4);


#define PH_PIN 14
#define METHANE_PIN 16
#define HUM_PIN 15
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

// MethaneSensor methanesensor = MethaneSensor(METHANE_PIN, R_0);
// HumiditySensor humsensor = HumiditySensor(HUM_PIN);
CO2Sensor co2 = CO2Sensor(17);
// pHSensor pH = pHSensor(PH_PIN, humsensor);

ScienceState state = ScienceState_STOP_COLLECTING;

int canSendInterval = 3000;
unsigned long nextSendTime;

int sample_number = 0;

Servo servo1, servo2;
void block();
void stopEverything();
void test_sample();
void calibrateEverything();

void setupDF() {
  while (sht3x.begin() != 0) {
    Serial.println("Failed to Initialize the chip, please confirm the wire connection");
    delay(1000);
  }
  Serial.print("Chip serial number");
  Serial.println(sht3x.readSerialNumber());
  if(!sht3x.softReset()){
    Serial.println("Failed to Initialize the chip....");
  }
  Serial.println("Set Temp/Humidity");
}

void setup() {
	Serial.begin(9600);
  Serial.println("Initializing...");

  Serial.println("Initializing pumps...");
  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  pinMode(PUMP3, OUTPUT);
  pinMode(PUMP4, OUTPUT);
  pinMode(PUMP5, OUTPUT);
  pinMode(PUMP6, OUTPUT);
  pinMode(PUMP7, OUTPUT);
  digitalWrite(PUMP1, HIGH);
  digitalWrite(PUMP2, HIGH);
  digitalWrite(PUMP3, HIGH);
  digitalWrite(PUMP4, HIGH);
  digitalWrite(PUMP5, HIGH);
  digitalWrite(PUMP6, HIGH);
  digitalWrite(PUMP7, HIGH);

  // Serial.println("Initializing communications...");
  // can.setup();
  // nextSendTime = millis() + canSendInterval;

  Serial.println("Initializing stepper motors...");
  vacuumLinear.presetup();
  dirtLinear.presetup();
  scienceLinear.presetup();
  dirtCarousel.presetup();

	vacuumLinear.setup();
  dirtLinear.setup();
  scienceLinear.setup();
  dirtCarousel.setup();

  // Serial.println("Calibrating motors...");
  // vacuumLinear.calibrate();
  // scienceLinear.calibrate();
  // dirtLinear.calibrate();
  // dirtCarousel.calibrate();

  Serial.println("Initializing servos...");
  servo1.attach(SERVO1);
  servo2.attach(SERVO2);
  servo2.write(70);
  servo1.write(30);
  // delay(1000);
  // servo2.write(180);

  Serial.println("Initializing sensors...");
  // co2.setup();
  // setupDF();

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

float read_temperature() {
  return sht3x.getTemperatureC();
}

float read_humidity() {
  return sht3x.getHumidityRH();
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
  else if (motor == "science-linear") scienceLinear.moveBy(distance); //scienceLinear.moveBy(distance);  
  else if (motor == "dirt-carousel") dirtCarousel.moveBy(distance); //dirtCarousel.moveBy(distance);  
  // else if (motor == "dirt-release") dirtRelease.moveBy(distance); //go +49 to uncover hole, -49 to go back
  // else if (motor == "science-test") test_sample(distance);
  // else if (motor == "pour-dirt") pourDirt(distance);
  else if (motor == "dirt-release") servo2.write(distance);
  else if (motor == "pinch") servo1.write(distance);
  else if (motor == "pump") {
    digitalWrite(PUMP1, LOW);
    digitalWrite(PUMP2, LOW);
    digitalWrite(PUMP3, LOW);
    digitalWrite(PUMP4, LOW);
    digitalWrite(PUMP5, LOW);
    digitalWrite(PUMP6, LOW);
    digitalWrite(PUMP7, LOW);
    delay(PUMP_DELAY);
    digitalWrite(PUMP1, HIGH);
    digitalWrite(PUMP2, HIGH);
    digitalWrite(PUMP3, HIGH);
    digitalWrite(PUMP4, HIGH);
    digitalWrite(PUMP5, HIGH);
    digitalWrite(PUMP6, HIGH);
    digitalWrite(PUMP7, HIGH);
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

void handlePumpCommand(int pin, PumpState state) {
  if (state == PumpState::PumpState_PUMP_ON) {
    digitalWrite(pin, LOW);
  } else if (state == PumpState::PumpState_PUMP_OFF) {
    digitalWrite(pin, HIGH);
  }
}

void scienceHandler(const uint8_t* data, int length) {
  Serial.println("Got data");
  ScienceCommand command = BurtProto::decode<ScienceCommand>(data, length, ScienceCommand_fields);
  // Individual motor control
  if (command.dirt_carousel != 0) dirtCarousel.moveBy(command.dirt_carousel);
  if (command.science_linear != 0) scienceLinear.moveBy(command.science_linear);
  // if (command.dirt_linear != 0) dirtLinear.moveBy(command.dirt_linear);
  // if (command.vacuum_linear != 0) vacuumLinear.moveBy(command.vacuum_linear);

  // Pumps
  handlePumpCommand(PUMP1, command.pump1);
  handlePumpCommand(PUMP2, command.pump1);
  handlePumpCommand(PUMP3, command.pump1);
  handlePumpCommand(PUMP4, command.pump1);
  handlePumpCommand(PUMP5, command.pump1);
  handlePumpCommand(PUMP6, command.pump1);
  handlePumpCommand(PUMP7, command.pump1);

  // Dirt release
  if (command.dirtRelease == DirtReleaseState::DirtReleaseState_OPEN_DIRT) {
    servo1.write(30);
  } else if (command.dirtRelease == DirtReleaseState::DirtReleaseState_CLOSE_DIRT) {
    servo1.write(180);
  }
  // Scooper
  if (command.pump2 == PumpState::PumpState_PUMP_ON) {
    servo2.write(0);
  } else if (command.pump2 == PumpState::PumpState_PUMP_OFF) {
    servo2.write(70);
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
  // vacuum.disable();
  // pump1.setSpeed(0);
  // pump2.setSpeed(0);
  // pump3.setSpeed(0);
  // pump4.setSpeed(0);
}

void sendData() {
  // Send generic data
  // if (millis() < nextSendTime) return;
  ScienceData data;
  // ScienceData data = ScienceData_init_zero;
  // data.sample = sample_number;
  // can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  // data = ScienceData_init_zero;
  // data.state = state;
  // can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  // // Send sensor data
  // if (state != ScienceState_COLLECT_DATA) return;
  // data = ScienceData_init_zero;
  // data.methane = methanesensor.getMethanePPM();
  // can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.co2 = co2.read();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  // serial.send(ScienceData_fields, &data, 8);
  // Serial.print("CO2: ");
  // Serial.println(data.humidity);

  data = ScienceData_init_zero;
  data.humidity = read_humidity();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  // serial.send(ScienceData_fields, &data, 8);
  // Serial.print("Humidity: ");
  // Serial.println(data.humidity);

  data = ScienceData_init_zero;
  data.temperature = read_temperature();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  // serial.send(ScienceData_fields, &data, 8);

  // data = ScienceData_init_zero;
  // data.pH = pH.sample_pH();
  // can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  nextSendTime = millis() + canSendInterval;
}

void block() {
  while (vacuumLinear.isMoving()) delay(BLOCK_DELAY);
  while (scienceLinear.isMoving()) delay(BLOCK_DELAY);
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
