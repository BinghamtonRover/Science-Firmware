//#include "src/Firmware-Utilities-main/BURT_utils.h"
#include "src/utils/BURT_utils.h"
//temporary fix, ask Levi how to get original command to work 

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
CO2Sensor co2sensor = CO2Sensor(CO2_PIN);
pHSensor pH = pHSensor(PH_PIN);
HumiditySensor humsensor = HumiditySensor(HUM_PIN);

void setup() {
	Serial.begin(9600);
  Serial.println("Initializing...");
  
  can.setup();

  //setting up stepper motors
  vacuumLinear.presetup();
  dirtLinear.presetup();
  scienceLinear.presetup();
  dirtCarousel.presetup();

	vacuumLinear.setup();
  dirtLinear.setup();
  scienceLinear.setup();
  dirtCarousel.setup();
  delay(10);

  //vacuumLinear.calibrate();
  //dirtLinear.calibrate();
  //scienceLinear.calibrate();
  //dirtCarousel.calibrate();

	Serial.println("Stepper motors initialized.");

  //TODO: Determine which pump is which (labelled D-G on hardware)
	 pump1.setup();
	 pump2.setup();
	 pump3.setup();
	 pump4.setup();
	 Serial.println("DC motors initialized.");

	vacuum.setup();
	dirtRelease.setup();
	vacuum.setSpeed(0);
	Serial.println("Vacuum initialized.");

  Serial.println("Sensors initialized.");

	Serial.println("Science Subsystem ready.");
	
}

void loop() {
  /* Real Rover code */
  
  //needed for stepper motors
   vacuumLinear.update();
   dirtLinear.update();
   scienceLinear.update();
   dirtCarousel.update();
   delay(10);

  can.update();
  serial.update();
  pH.sample_pH();
  sendData();

	/* Temporary Serial Monitor interface */
	String input = Serial.readString();
	parseSerialCommand(input);
	delay(10);
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
	else if (motor == "vacuum") vacuum.setSpeed(speed);
	else if (motor == "dirt-release") dirtRelease.moveBy(distance); //go +49 to uncover hole, -49 to go back
  else if (motor == "dirt-release-open") dirtRelease.open(); //TEST OPEN AND CLOSE FUNCTIONS
  else if (motor == "dirt-release-close") dirtRelease.close(); 
	else if (motor == "pump1") { //NEED TO TEST PUMPS, some speeds may need to change signs in order to actually pump
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

//Will need to switch to TMC commands once we have tested them
void scienceHandler(const uint8_t* data, int length) {
  ScienceCommand command = BurtProto::decode<ScienceCommand>(data, length, ScienceCommand_fields);
  if(command.carousel_angle != 0) dirtCarousel.debugMoveBySteps(command.carousel_angle);
  if(command.carousel_linear_position != 0) dirtLinear.debugMoveBySteps(command.carousel_linear_position);
  if(command.test_linear_position != 0) scienceLinear.debugMoveBySteps(command.test_linear_position);
  if(command.vacuum_linear_position != 0) vacuumLinear.debugMoveBySteps(command.vacuum_linear_position);
  if(command.dirtRelease != 0) dirtRelease.moveBy(command.dirtRelease); //can we change this to use open/close functions based on what is pressed?
  if (command.pump1) { //double check these speeds and which pump is which (D-G on hardware)
  	pump1.setSpeed(100);
  	delay(1000); //switch this to pump delay defined at top once verified
  	pump1.setSpeed(0);
  }
  if (command.pump2) {
  	pump2.setSpeed(100);
  	delay(1000);
  	pump2.setSpeed(0);
  }
  if (command.pump3) {
  	pump3.setSpeed(-100);
  	delay(1000);
  	pump3.setSpeed(0);
  }
  if (command.pump4) {
  	pump4.setSpeed(-100);
  	delay(1000);
  	pump4.setSpeed(0); 
  }
  vacuum.setSpeed(command.vacuum_suck);
}

void sendData() {
  ScienceData data1, data2, data3, data4, data5;
  data1.methane = methanesensor.getMethanePPM();
  data1.methane = 0.25;
  data2.co2 = co2sensor.readPPM();
  data3.pH = pH.returnpH();
  data4.humidity = humsensor.readHumidity();
  data5.temperature = humsensor.readTemperature();
  can.send(SCIENCE_DATA_ID, &data1, ScienceData_fields);
  Serial.println(data1.methane);
  // can.send(SCIENCE_DATA_ID, &data2, ScienceData_fields);
  // can.send(SCIENCE_DATA_ID, &data3, ScienceData_fields);
  // can.send(SCIENCE_DATA_ID, &data4, ScienceData_fields);
  // can.send(SCIENCE_DATA_ID, &data5, ScienceData_fields);
}
