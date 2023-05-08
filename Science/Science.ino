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

#define VACUUM_FAST_SPEED 50
#define VACUUM_DIG_DELAY 1000 // ms
#define VACUUM_DROP_DELAY 1000  // ms

#define VACUUM_LINEAR_MAX_HEIGHT -370 //Changed from 215 mm, Double check this
#define DIRT_LINEAR_OUTER_RADIUS 35  // mm
#define DIRT_LINEAR_INNER_RADIUS 35  // mm
#define DIRT_LINEAR_TEST_OFFSET 10   // mm
#define SCIENCE_LINEAR_MAX_HEIGHT 35  // mm

#define SENSORS_READ_DELAY 1000  // ms

#define PUMP_SPEED -100 //must be negative to pump, positive blows bubbles
#define PUMP_DELAY 1000  // 2000  // ms DETERMINE ACTUAL DESIRED VALUE

#define SCIENCE_COMMAND_ID 0x43
#define SCIENCE_DATA_ID 0x17

void scienceHandler(const uint8_t* data, int length);
BurtSerial serial(scienceHandler, Device::Device_SCIENCE);
BurtCan can(SCIENCE_COMMAND_ID, scienceHandler);

#define PH_PIN 14
#define METHANE_PIN 16
#define HUM_PIN 15
#define CO2_PIN 17

#define R_0 945

MethaneSensor methanesensor = MethaneSensor(METHANE_PIN, R_0);
HumiditySensor humsensor = HumiditySensor(HUM_PIN);
CO2Sensor co2 = CO2Sensor();
pHSensor pH = pHSensor(PH_PIN);

int canSendInterval = 200;
unsigned long nextSendTime;

void setup() {
	Serial.begin(9600);
  Serial.println("Initializing...");

  vacuum.setup();
  dirtRelease.setup();

  can.setup();
  nextSendTime = millis() + canSendInterval;

  //setting up stepper motors
  vacuumLinear.presetup();
  dirtLinear.presetup();
  scienceLinear.presetup();
  dirtCarousel.presetup();

	vacuumLinear.setup();
  dirtLinear.setup();
  scienceLinear.setup();
  dirtCarousel.setup();

  //vacuumLinear.calibrate();
  //dirtLinear.calibrate();
  //scienceLinear.calibrate();
  //dirtCarousel.calibrate();

	Serial.println("Stepper motors initialized.");

  // //TODO: Determine which pump is which (labelled D-G on hardware)
	 pump1.setup();
	 pump2.setup();
	 pump3.setup();
	 pump4.setup();
	 Serial.println("DC motors initialized.");

	// Serial.println("Vacuum initialized.");

  humsensor.setup();
  co2.setup();
  pH.setup();
  Serial.println("Sensors initialized.");

	Serial.println("Science Subsystem ready.");
}

void loop() {
  /* Real Rover code */
  vacuumLinear.update();
  dirtLinear.update();
  scienceLinear.update();
  dirtCarousel.update();

  can.update();
  serial.update();
  sendData();

  /* Temporary Serial Monitor interface */
  // String input = Serial.readString();
  // parseSerialCommand(input);
  // delay(10);
}

/* Temporary Serial Monitor interface for testing. */
void parseSerialCommand(String input) {
  // Accept the command as-is
  // if (input == "calibrate") return calibrate();
  // else if (input  == "dig") return dig();
  // else if (input == "test") return testSamples();
  // Parse the command
  int delimiter = input.indexOf(" ");
  if (delimiter == -1) return;
  String part1 = input.substring(0, delimiter);
  String part2 = input.substring(delimiter + 1);
  String motor = part1;
  float distance = part2.toFloat();
  int speed = part2.toInt();

  // Execute the command
  if (motor == "vacuum-linear") {
    Serial.println("Moving vacuum linear");
    vacuumLinear.debugMoveBySteps(distance);
  }
  
  else if (motor == "dirt-linear") dirtLinear.debugMoveBySteps(distance); //dirtLinear.moveBy(distance);  
  else if (motor == "science-linear") scienceLinear.debugMoveBySteps(distance); //scienceLinear.moveBy(distance);  
  else if (motor == "dirt-carousel") dirtCarousel.debugMoveBySteps(distance); //dirtCarousel.moveBy(distance);  
  else if (motor == "vacuum_on") vacuum.enable();
  else if (motor == "vacuum_off") vacuum.disable();
  else if (motor == "dirt-release") dirtRelease.moveBy(distance); //go +49 to uncover hole, -49 to go back
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
  }
  else {
    Serial.println("Command not recognized: " + input);
    Serial.println("  Commands are of the form: motor-name distance/speed.");
    Serial.println("  For stepper motors: positive distance is away from the limit switch.");
    Serial.println("  For DC motors: speed is in the range [-100, 100]. Positive = clockwise.");
    Serial.println("  For example: vacuum-linear 20, would move the vacuum 20mm up.");
    Serial.println("  Example 2: auger 100 spins the auger at full speed.");
    Serial.println("");
  }
}

void updatePump(DCMotor pump, PumpState state) {
  switch (state) {
    case PumpState::PumpState_PUMP_STATE_UNDEFINED: return;
    case PumpState::PumpState_PUMP_ON: pump.setSpeed(-100); break;
    case PumpState::PumpState_PUMP_OFF: pump.setSpeed(0); break;
  }
}

//Will need to switch to TMC commands once we have tested them
void scienceHandler(const uint8_t* data, int length) {
  ScienceCommand command = BurtProto::decode<ScienceCommand>(data, length, ScienceCommand_fields);
  // Individual motor control
  dirtCarousel.debugMoveBySteps(command.dirt_carousel);
  dirtLinear.debugMoveBySteps(command.dirt_linear);
  scienceLinear.debugMoveBySteps(command.science_linear);
  vacuumLinear.debugMoveBySteps(command.vacuum_linear);

  // We get multiple packets without vacuum data, so [vacuumPower] will be 0. This will happen even
  // when the user is driving the vacuum, but we don't want to set the vacuum on and off all the 
  // time. So we'll ignore [vacuumPower] unless [set_vacuum] is true.
  if (command.vacuum == PumpState_PUMP_ON) vacuum.enable();
  else if (command.vacuum == PumpState_PUMP_OFF) vacuum.disable();
  if (command.dirtRelease == DirtReleaseState_OPEN_DIRT) dirtRelease.open();
  else if(command.dirtRelease == DirtReleaseState_CLOSE_DIRT) dirtRelease.close();

  // Pumps
  updatePump(pump1, command.pump1);
  updatePump(pump2, command.pump2);
  updatePump(pump3, command.pump3);
  updatePump(pump4, command.pump4);

  // Commands
  if (command.stop) {
    dirtCarousel.stop();
    dirtLinear.stop();
    scienceLinear.stop();
    vacuumLinear.stop();
    vacuum.disable();
    pump1.setSpeed(0);
    pump2.setSpeed(0);
    pump3.setSpeed(0);
    pump4.setSpeed(0);
  } else if (command.calibrate) {
    // dirtCarousel.calibrate();
    // dirtLinear.calibrate();
    // scienceLinear.calibrate();
    // vacuumLinear.calibrate();
  }
  if (command.next_tube) dirtCarousel.moveBy(PI / 6); 
  if (command.next_section) dirtCarousel.moveBy(2 * PI / 3);
}

void sendData() {
  float temperature = humsensor.readTemperature();
  if (millis() < nextSendTime) return;
  ScienceData data = ScienceData_init_zero;
  data.methane = methanesensor.getMethanePPM();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.co2 = co2.getPercentage();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.humidity = humsensor.readHumidity();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.temperature = temperature;
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.pH = pH.read(temperature);
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  nextSendTime = millis() + canSendInterval;
}
