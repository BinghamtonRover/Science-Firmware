#include "pinouts.h"
#include "src/utils/BURT_utils.h"
#include "src/science.pb.h"

#define SCIENCE_COMMAND_ID 0x43
#define SCIENCE_DATA_ID 0x17

#define USE_SERIAL_MONITOR false

const Version version {
  major: 1,
  minor: 0,
};

void scienceHandler(const uint8_t* data, int length);
void sendData();
void shutdown() { }
BurtSerial serial(Device::Device_SCIENCE, scienceHandler, shutdown);
BurtCan<Can3> can(SCIENCE_COMMAND_ID, Device::Device_SCIENCE, scienceHandler, shutdown);
BurtTimer dataTimer(250, sendData);

ScienceState state = ScienceState_STOP_COLLECTING;

int sample_number = 0;
  
void stopEverything() {
  motors.stop();
  pumps.turnOff();
}

void setup() {
	Serial.begin(9600);
  Serial.println("Initializing...");

  Serial.println("Initializing communications...");
  can.setup();

  Serial.println("Initializing hardware...");
  motors.setup();
  motors.calibrate();
  scooper.setup();
  pumps.setup();
  carousel.setup();

  Serial.println("Initializing sensors...");
  co2.setup();
  tempHumidity.setup();

	Serial.println("Science Subsystem ready.");
}

void loop() {
  motors.update();
  can.update();
  serial.update();
  dataTimer.update();
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

  // Execute the command
  if (motor == "stop") stopEverything();
  else if (motor == "dirt-linear") motors.dirtLinear.moveBy(distance); //dirtLinear.moveBy(distance);  
  else if (motor == "science-linear") motors.scooperArm.moveBy(distance); //scoopArmMotor.moveBy(distance);  
  else if (motor == "dirt-carousel") motors.dirtCarousel.moveBy(distance); //dirtCarousel.moveBy(distance);  
  else if (motor == "pump") {
    pumps.fillTubes();
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
  if (command.has_version && command.version.major != version.major) return;

  // Control specific hardware
  motors.handleCommand(command);
  pumps.handleCommand(command);
  scooper.handleCommand(command);
  carousel.handleCommand(command);

  // General commands  
  if (command.stop) stopEverything();
  else if (command.calibrate) motors.calibrate();
  if (command.sample != 0) sample_number = command.sample - 1;
  switch (command.state) {
    case ScienceState_SCIENCE_STATE_UNDEFINED: break;
    case ScienceState_COLLECT_DATA: 
      if (state == ScienceState_STOP_COLLECTING) test_sample(sample_number);
      state = command.state;
      break;
    case ScienceState_STOP_COLLECTING: 
      state = command.state;
      break;
  }
}

void sendData() {
  ScienceData data = ScienceData_init_zero;
  data.sample = sample_number;
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  serial.send(ScienceData_fields, &data, 8);

  data = ScienceData_init_zero;
  data.state = state;
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  serial.send(ScienceData_fields, &data, 8);

  data = ScienceData_init_zero;
  data.version = version;
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  serial.send(ScienceData_fields, &data, 8);

  if (state != ScienceState_COLLECT_DATA) return;

  data = ScienceData_init_zero;
  data.co2 = co2.read();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  serial.send(ScienceData_fields, &data, 8);

  data = ScienceData_init_zero;
  data.humidity = tempHumidity.getHumidity();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  serial.send(ScienceData_fields, &data, 8);

  data = ScienceData_init_zero;
  data.temperature = tempHumidity.getTemperature();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
  serial.send(ScienceData_fields, &data, 8);
}

void test_sample(int sample) {
  motors.calibrate();
  carousel.goToSection(sample);
  carousel.fillSection();
  carousel.goToTests();
  pumps.fillTubes();
  carousel.goToPicture();
}
