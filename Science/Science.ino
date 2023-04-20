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
#define PUMP_DELAY 10000  // 2000  // ms

#define SCIENCE_COMMAND_ID 0x43
#define SCIENCE_DATA_ID 0x17

BurtSerial serial(scienceHandler);
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

	vacuumLinear.setup();
	dirtLinear.setup();
	scienceLinear.setup();
	dirtCarousel.setup();
	Serial.println("Stepper motors initialized.");

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
  can.update();
  //serial.update();
  serial.update();
  pH.sample_pH();
  // sendData();

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
    vacuumLinear.moveBy(distance);
  }
	else if (motor == "temp") {  // changes the PWM delay of the given motor
		vacuumLinearConfig.pwmDelay = speed;
		Serial.println(vacuumLinearConfig.pwmDelay);
	}
	else if (motor == "dirt-linear") dirtLinear.moveBy(distance);
	else if (motor == "science-linear") scienceLinear.moveBy(distance);
	else if (motor == "dirt-carousel") dirtCarousel.moveBy(distance);
	else if (motor == "vacuum") vacuum.setSpeed(speed);
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

void scienceHandler(const uint8_t* data, int length) {
  ScienceCommand command = BurtProto::decode<ScienceCommand>(data, length, ScienceCommand_fields);
  if(command.spin_carousel_tube) dirtCarousel.nextTube();
  if(command.spin_carousel_section) dirtCarousel.nextSection();
  if(command.carousel_angle != 0) dirtCarousel.moveBy(command.carousel_angle);
  if(command.carousel_linear_position != 0) dirtLinear.moveBy(command.carousel_linear_position);
  if(command.test_linear_position != 0) scienceLinear.moveBy(command.test_linear_position);
  if(command.vacuum_linear_position != 0) vacuumLinear.moveBy(command.vacuum_linear_position);
  if(command.dirtRelease != 0) dirtRelease.moveBy(command.dirtRelease);
  if (command.pump1) {
  	pump1.setSpeed(100);
  	delay(1000);
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
  ScienceData data;
  data.methane = methanesensor.getMethanePPM();
  data.co2 = co2sensor.readPPM();
  data.pH = pH.returnpH();
  data.humidity = humsensor.readHumidity();
  data.temperature = humsensor.readTemperature();
  can.send(SCIENCE_DATA_ID, &data, ScienceData_fields);
}
