#include "pinouts.h"
#include "src/utils/BURT_utils.h"
#include "src/science.pb.h"
#include "src/version.pb.h"

#define SCIENCE_COMMAND_ID 0x43
#define SCIENCE_DATA_ID 0x17

#define USE_SERIAL_MONITOR false

Version version = {major: 1, minor: 1};
// Note to posterity: there's some bs in the tmc lib as of 5/29/26 at 1:09am
// dont remove unless the ability to configure the hold current of an internal pos stepper is added
// if it has been, set it to 0
// ouchie ouchie hot stepper
void scienceHandler(const uint8_t* data, int length);
void sendData();
void shutdown() { }

BurtSerial serial(Device::Device_SCIENCE, scienceHandler, ScienceData_fields, ScienceData_size);
BurtTimer dataTimer(250, sendData);

ScienceState state = ScienceState_STOP_COLLECTING;

int sample_number = 0;
  
void stopEverything() {
  augerMotor.stop();
  linearSlider.stop();
}

// void sampleISR() {
//   currentSensor.updateFromISR(analogRead(23));
// }

void setup() {
	Serial.begin(9600);
  
  dataTimer.setup();
  Serial.println("Initializing...");

  Serial.println("Initializing communications...");

  Serial.println("Initializing sensors...");
  tempHumidity.setup();

  upper_servo.setup();
  lower_servo.setup();
  
  //lidar.setup();

  currentSensor.begin();

  //Timer1.initialize(1000);
  //Timer1.attachInterrupt(sampleISR);

  Serial.println("Initializing hardware...");
  augerMotor.preSetup();
  augerMotor.setup();
  
  linearSlider.preSetup();
  linearSlider.setup();
  
	Serial.println("Science Auxiliary Subsystem ready.");
}

void loop(){
  augerMotor.update();
  linearSlider.update();
  //lidar.update();
  serial.update();
  dataTimer.update();
}

/* Temporary Serial Monitor interface for testing. 
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
*/

void scienceHandler(const uint8_t* data, int length) {
  ScienceCommand command = BurtProto::decode<ScienceCommand>(data, length, ScienceCommand_fields);

  // Control specific hardware
  if (command.stop) augerMotor.setMotorRps(0);
  if (command.auger.speed_rpm != 0){
    augerMotor.setMotorRps(command.auger.speed_rpm);
  }
  if(command.auger.speed_rpm == 0.0){
    augerMotor.setMotorRps(0);
  }
  if (command.linear_slider != 0){
    linearSlider.moveBy(command.linear_slider);
  }
  
  upper_servo.handleCommand(command.auger.upper_servo);
  lower_servo.handleCommand(command.auger.lower_servo);

  // General commands
  if (command.stop) {}//stopEverything();
  else if (command.calibrate){
    augerMotor.calibrate();
    linearSlider.calibrate();
  }
}

void sendData() {

  ScienceData data = ScienceData_init_zero;
  data.sample = sample_number;

  data.state = state;

  data.humidity = tempHumidity.getHumidity();

  data.temperature = tempHumidity.getTemperature();

  data.has_auger = true;
  //data.auger.distance_to_ground_cm = lidar.getDistance();

  data.auger.current = currentSensor.getFilteredCurrent();
  serial.send(&data);

}
