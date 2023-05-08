#include "src/utils/BURT_utils.h"
#include "src/science.pb.h"
#include "pinouts.h"

#define DIRT_LINEAR_OUTER_RADIUS 35  // mm
#define DIRT_LINEAR_INNER_RADIUS 35  // mm
#define DIRT_LINEAR_TEST_OFFSET 10   // mm

#define SCIENCE_COMMAND_ID 0x43
#define SCIENCE_DATA_ID 0x17

#define USE_SERIAL_MONITOR false

void scienceHandler(const uint8_t* data, int length);
BurtSerial serial(scienceHandler, Device::Device_SCIENCE);
BurtCan can(SCIENCE_COMMAND_ID, scienceHandler);

int CAN_SEND_INTERVAL = 200;
unsigned long nextSendTime;

void setup() {
	Serial.begin(9600);
  Serial.println("Initializing...");

  Serial.println("Initializing DC motors...");
  vacuum.setup();
  dirtRelease.setup();
  pump1.setup();
  pump2.setup();
  pump3.setup();
  pump4.setup();

  //setting up stepper motors
  Serial.println("Initializing stepper motors...");
  vacuumLinear.presetup();
  dirtLinear.presetup();
  scienceLinear.presetup();
  dirtCarousel.presetup();

	vacuumLinear.setup();
  dirtLinear.setup();
  scienceLinear.setup();
  dirtCarousel.setup();

  Serial.println("  Calibrating stepper motors...");
  vacuumLinear.calibrate();
  dirtLinear.calibrate();
  scienceLinear.calibrate();
  dirtCarousel.calibrate();

  Serial.println("Initializing communications...");
  can.setup();
  nextSendTime = millis() + CAN_SEND_INTERVAL;

  Serial.println("Initializing sensors...");
  humiditySensor.setup();
  co2Sensor.setup();
  pHSensor.setup();
  methaneSensor.setup();

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
  if (USE_SERIAL_MONITOR) parseSerialMonitor();
  else serial.update();
}

/* Temporary Serial Monitor interface for testing. */
void parseSerialMonitor() {
  String input = Serial.readString();
  if (input == "") return;

  int delimiter = input.indexOf(" ");
  if (delimiter == -1) return;
  String motor = input.substring(0, delimiter);
  String arg = input.substring(delimiter + 1);
  float distance = arg.toFloat();
  int speed = arg.toInt();

  // Execute the command
  if (motor == "vacuum-linear") vacuumLinear.debugMoveBySteps(distance);  
  else if (motor == "dirt-linear") dirtLinear.debugMoveBySteps(distance); //dirtLinear.moveBy(distance);  
  else if (motor == "science-linear") scienceLinear.debugMoveBySteps(distance); //scienceLinear.moveBy(distance);  
  else if (motor == "dirt-carousel") dirtCarousel.debugMoveBySteps(distance); //dirtCarousel.moveBy(distance);  
  else if (motor == "vacuum_on") vacuum.enable();
  else if (motor == "vacuum_off") vacuum.disable();
  else if (motor == "dirt-release") dirtRelease.moveBy(distance); //go +49 to uncover hole, -49 to go back
  else if (motor == "pump1") {
    pump1.setSpeed(speed);
    delay(1000);
    pump1.hardBrake();
  } else if (motor == "pump2") {
    pump2.setSpeed(speed);
    delay(1000);
    pump2.hardBrake();
  } else if (motor == "pump3") {
    pump3.setSpeed(speed);
    delay(1000);
    pump3.hardBrake();
  } else if (motor == "pump4") {
    pump4.setSpeed(speed);
    delay(1000);
    pump4.hardBrake();
  } else {
    Serial.println("Command not recognized: " + input);
  }
}

void updatePump(DcMotor pump, PumpState state) {
  switch (state) {
    case PumpState::PumpState_PUMP_STATE_UNDEFINED: return;
    case PumpState::PumpState_PUMP_ON: pump.setSpeed(-1); break;
    case PumpState::PumpState_PUMP_OFF: pump.setSpeed(0); break;
  }
}

//Will need to switch to TMC commands once we have tested them
void scienceHandler(const uint8_t* data, int length) {
  ScienceCommand command = BurtProto::decode<ScienceCommand>(data, length, ScienceCommand_fields);

  // Stepper motors
  dirtCarousel.debugMoveBySteps(command.dirt_carousel);
  dirtLinear.debugMoveBySteps(command.dirt_linear);
  scienceLinear.debugMoveBySteps(command.science_linear);
  vacuumLinear.debugMoveBySteps(command.vacuum_linear);

  // Vacuum
  if (command.vacuum == PumpState_PUMP_ON) vacuum.enable();
  else if (command.vacuum == PumpState_PUMP_OFF) vacuum.disable();
  if (command.dirtRelease == DirtReleaseState_OPEN_DIRT) dirtRelease.open();
  else if(command.dirtRelease == DirtReleaseState_CLOSE_DIRT) dirtRelease.close();

  // Pumps
  updatePump(pump1, command.pump1);
  updatePump(pump2, command.pump2);
  updatePump(pump3, command.pump3);
  updatePump(pump4, command.pump4);

  // General commands
  if (command.stop) {
    dirtCarousel.stop();
    dirtLinear.stop();
    scienceLinear.stop();
    vacuumLinear.stop();
    vacuum.disable();
    pump1.hardBrake();
    pump2.hardBrake();
    pump3.hardBrake();
    pump4.hardBrake();
  } else if (command.calibrate) {
    dirtCarousel.calibrate();
    dirtLinear.calibrate();
    scienceLinear.calibrate();
    vacuumLinear.calibrate();
  }
  if (command.next_tube) dirtCarousel.moveBy(PI / 6); 
  if (command.next_section) dirtCarousel.moveBy(2 * PI / 3);
}

void sendData() {
  float temperature = humiditySensor.readTemperature();
  if (millis() < nextSendTime) return;
  ScienceData data = ScienceData_init_zero;
  data.methane = methaneSensor.read();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.co2 = co2Sensor.read();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.humidity = humiditySensor.readHumidity();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.temperature = temperature;
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  data = ScienceData_init_zero;
  data.pH = pHSensor.read(temperature);
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);

  nextSendTime = millis() + CAN_SEND_INTERVAL;
}
