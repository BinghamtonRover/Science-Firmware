//user notes -- should calibrate the co2 sensor using sensor calibration code -- need to first let sensor read value from room then let it read from bag filled with exhaled air
//Pump4 and Pump5 have A1 & A2 tied to B1 & B2 (suspect they will run at same time)
//update calibration function to mirror what is done in the mg811 calibration example code on github 

//Arduino code for BURT 20202-2021 Science Subsystem written by Dillon Kane

#include <Wire.h>
#include <rocs.hpp>
#include <SparkFun_TB6612.h> //ROB 14451 
#include <DRV8834.h> //closest to MP6500
#include <MultiDriver.h>
#include <SyncDriver.h>
#include <Arduino.h>
#include <MG811.h>//library for co2 sensor
#include <dht.h> //library for DHT11 temp & humidity sensor

//pin definitions
#define Pump1_in1 15 
#define Pump1_in2 14 
#define Pump1_PWM 11 
#define Pump2_in1 18 
#define Pump2_in2 13 
#define Pump2_PWM 5 
#define Pump3_in1 19 
#define Pump3_in2 9 
#define Pump3_PWM 6 
#define Pump4_in1 41
#define Pump4_in2 30 
#define Pump4_PWM 7
#define Pump5_in1 41 //might be a problem -- tied pump5_in1 to pump4_in2 instead of pump4_in1 on PCB
#define Pump5_in2 30 //might be a problem -- tied pump5_in2 to pump4_in1 instead of pump4_in2 on PCB
#define Pump5_PWM 10 
#define linear_in1 37 
#define linear_in2 36 
#define linear_PWM 8 
#define DC_motor_in1 16
#define DC_motor_in2 17 
#define DC_motor_PWM 12 
#define STBY 40 //all ROB 14451's have same stby pin
const int motor_offset = 1;
#define MOTOR_STEPS 200 //steps per revolution -- 200 steps or 1.8 degrees/step
#define RPM 120 // Target RPM for cruise speed //need to find ideal value
#define MOTOR_ACCEL 2000 // Acceleration and deceleration values are always in FULL steps / s^2
#define MOTOR_DECEL 1000
#define MICROSTEPS 16 //microstepping can make motors more accurate but require extra pins -- if we decide to microstep we will have to solder extra wires
#define Dirt_DIR 35 //step 1 on pcb
#define Dirt_STEP 33 //step 1 on pcb
#define Dirt_MS1 PIN_UNCONNECTED //ms pins are used for microstepping, library shows that pins are defined as unconnected so may not have to change
#define Dirt_MS2 PIN_UNCONNECTED 
#define Auger_DIR 32 //step 2 on pcb
#define Auger_STEP 31 //step 2 on pcb
#define Auger_MS1 PIN_UNCONNECTED
#define Auger_MS2 PIN_UNCONNECTED
#define Dirt_to_Auger_DIR 27 //step 3 on pcb
#define Dirt_to_Auger_STEP 26 //step 3 on pcb
#define Dirt_to_Auger_MS1 PIN_UNCONNECTED
#define Dirt_to_Auger_MS2 PIN_UNCONNECTED

dht DHT; //create dht variable to be used when reading temperature & humidity values
#define pH 20           //pH meter Analog output to Arduino Analog Input 0
#define MQ4 23     //need to convert to Rs/ Ro values as seen in data sheet to plot against ppm
#define DHT11 22 //DHT only returns integer values      
#define Offset 0.00            //deviation compensate for pH
#define samplingInterval 20    //used in pH
#define printInterval 800       //used ih Ph
#define ArrayLenth  40    //used in pH -- times of collection
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex=0;

float v400 = 4.535;     // change 4.353 to be value from co2 open air reading (from calibration code)
float v40000 = 3.206;   // change 3.206 to be value from co2 exhaled air reading (from calibration code)
    
MG811 co2Sensor = 21; //define Co2 Sensor pin
int Co2Value, methaneValue; //global variables used to store & write final values of co2 and methane
float pHValue; //global variable used to store & write final value of pH
int temperature, humidity; //global variable to store temp & humidity values
const char* ROCS_NAME = "ScienceSubsystem";
uint8_t selector = 0;

//create motors
Sparkfun_Motor Pump1(Pump1_in1, Pump1_in2, Pump1_PWM, motor_offset, STBY);
Sparkfun_Motor Pump2(Pump2_in1, Pump2_in2, Pump2_PWM, motor_offset, STBY);
Sparkfun_Motor Pump3(Pump3_in1, Pump3_in2, Pump3_PWM, motor_offset, STBY);
Sparkfun_Motor Pump4(Pump4_in1, Pump4_in2, Pump4_PWM, motor_offset, STBY);
Sparkfun_Motor Pump5(Pump5_in1, Pump5_in2, Pump5_PWM, motor_offset, STBY);
Sparkfun_Motor linear(linear_in1, linear_in2, linear_PWM, motor_offset, STBY);
Sparkfun_Motor DC_Motor(DC_motor_in1, DC_motor_in2, DC_motor_PWM, motor_offset, STBY); //error is because it reads from stepper motor library instead of rob-14451 
DRV8834 Dirt_stepper(MOTOR_STEPS, Dirt_DIR, Dirt_STEP, Dirt_MS1, Dirt_MS2);
DRV8834 Auger_stepper(MOTOR_STEPS, Auger_DIR, Auger_STEP, Auger_MS1, Auger_MS2);
DRV8834 Dirt_to_Auger_stepper(MOTOR_STEPS, Dirt_to_Auger_DIR, Dirt_to_Auger_STEP, Dirt_to_Auger_MS1, Dirt_to_Auger_MS2);

void setup() {
  
    Serial.begin(115200);
    pinMode(MQ4, INPUT);
    pinMode(pH, INPUT);
    pinMode(DHT11, INPUT);
    Wire.begin();
    rocs::init(0x11, ROCS_NAME, strlen(ROCS_NAME));
    rocs::set_read_handler(read_handler);
    rocs::set_write_handler(write_handler);   
    Dirt_stepper.begin(RPM, MICROSTEPS); //rotate dirt carousel
    Dirt_stepper.enable();
    Auger_stepper.begin(RPM, MICROSTEPS); //lower auger head
    Auger_stepper.enable();
    Dirt_to_Auger_stepper.begin(RPM, MICROSTEPS); //move dirt carousel below tests or auger 
    Dirt_to_Auger_stepper.enable();
    
}

void loop() { 

  Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    selector = Wire.read(); // receive a byte as character
    Serial.print(selector);         // print the character
  }
  
    switch(selector){
      
      case 1: 
        calibrate_sensors();
        delay(100);
        break;
      case 2: 
        lower_auger();
        delay(100);
        break;
      case 3: 
        collect_dirt();
        delay(100);
        break;
      case 4: 
        raise_auger();
        delay(100);
        break;
      case 5: 
        move_dirt_to_auger();
        delay(100);
        break;
      case 6: 
        move_dirt_to_tests();
        delay(100);
        break;
      case 7: 
        distribute_liquids();
        delay(100);
        break;
      case 8: 
        collect_data();
        delay(100);
        break;
      case 9: 
        raise_tests();
        delay(100);
        break;
      case 10: 
        rotate_dirt();
        delay(100);
        break;  
      default:
        delay(10);
        break;  
    }   
   
}

void distribute_liquids(void){ //need to figure out for how long we need to run each pump to get desired amount of liquids ---------------------------------------------------------------
  
   Pump1.drive(255,1000); //runs pump 1 at speed 255 for 100 ms then turns off and loops
   Pump1.brake();
   delay(100);
   Pump2.drive(255,1000);
   Pump2.brake();
   delay(100);
   Pump3.drive(255,1000);
   Pump3.brake();
   delay(100);
   Pump4.drive(255,1000);
   Pump4.brake();
   delay(1000);
   Pump5.drive(255,1000);
   Pump5.brake();
   delay(100);
   
}

void lower_auger(void){ //want to run this function as long as a certain button is held

    Auger_stepper.rotate(360); //not sure if rotation is done with angle or time
    Auger_stepper.stop();
    
}

void raise_auger(void){ //want to run this function as long as a certain button is held

    Auger_stepper.rotate(-360); //not sure if rotation is done with angle or time
    Auger_stepper.stop();
    
}

void move_dirt_to_auger(void){ //need to calculate / measure how long to run this function so it always alligns

    Dirt_to_Auger_stepper.rotate(120); //not sure if rotation is done with angle or time
    Dirt_to_Auger_stepper.stop();
    
}

void move_dirt_to_tests(void){ //need to calculate / measure how long to run this function so it always alligns

    Dirt_to_Auger_stepper.rotate(-120); //not sure if rotation is done with angle or time
    Dirt_to_Auger_stepper.stop();

}

void rotate_dirt(void){ //want this rotation to be exactly 1/3rd (120 degrees)

    Dirt_stepper.rotate(120); //not sure if rotation is done with angle or time
    Dirt_stepper.stop();
    
}

void lower_tests(void){ //run for predetermined amount of time / distance

    linear.drive(255,1000);
    linear.brake();
    
}

void raise_tests(void){ //run for predetermined amount of time / distance

    linear.drive(-255,1000);
    linear.brake();
    
}

void collect_dirt(void){ //spin the auger head to collect dirt (want to run while button held)

    DC_Motor.drive(255, 1000);
    DC_Motor.brake();
    
}

void dispense_dirt(void){ //spin the auger the opposite direction (want to run while button held)

    DC_Motor.drive(-255, 1000);
    DC_Motor.brake();
    
}

int collect_data(void)
  {
    int sensorThres = 400; //need to figure out what value to give this
    int methaneValue = analogRead(MQ4); //read MQ4 sensor ///need to figure out singificance of this value and convert it to useful data
    int Co2Value = co2Sensor.read();  //read co2 sensor (value in ppm)
    int checkDHT = DHT.read11(DHT11); //used to check if DHT sensor is operating correctly
    pHValue = find_pH(); //call the pH function to calculate the average pH
    switch (checkDHT){ //checks DHT sensor is working ok, prints status
          case DHTLIB_OK:  
          Serial.print("OK,\n"); 
          break;
          case DHTLIB_ERROR_CHECKSUM: 
          Serial.print("Checksum error,\n"); 
          break;
          case DHTLIB_ERROR_TIMEOUT: 
          Serial.print("Time out error,\n"); 
          break;
          default: 
          Serial.print("Unknown error,\n"); 
          break;
    }
    delay(500);
    print_results(pHValue, Co2Value, methaneValue, sensorThres); //send results to serial monitor
  }

float find_pH(void)
  {
     
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float pHValue,voltage;
  if(millis()-samplingTime > samplingInterval)
    {
        pHArray[pHArrayIndex++]=analogRead(pH); //pH sensor actually seems to measure the voltage
        if(pHArrayIndex==ArrayLenth)pHArrayIndex=0; 
        voltage = avergearray(pHArray, ArrayLenth)*5.0/1024; //avg of voltage array is used to calculate the pH
        pHValue = 3.5*voltage+Offset;
        samplingTime=millis();
    }
  if(millis() - printTime > printInterval)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
    {
      Serial.print("Voltage:");
          Serial.print(voltage,2);
          Serial.print("    pH value: ");
      Serial.println(pHValue,2);
          printTime=millis();
    }
 }
 
double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0)
    {
      Serial.println("Error number for the array to avraging!/n");
      return 0;
    }
  if(number<5)
     {   //less than 5, calculated directly statistics
      for(i=0;i<number;i++)
        {amount+=arr[i];}
      
     avg = amount/number;
     return avg;
    
     }
     
     else
       {
      if(arr[0]<arr[1])
        {min = arr[0];max=arr[1];}
      else
      {min=arr[1];max=arr[0];}
      
      for(i=2;i<number;i++){
        if(arr[i]<min){
          amount+=min;        //arr<min
          min=arr[i];
        }else {
          if(arr[i]>max){
            amount+=max;    //arr>max
            max=arr[i];
          }else{
            amount+=arr[i]; //min<=arr<=max
          }
        }//if
      }//for
      avg = (double)amount/(number-2);
    }//if
    
  return avg;
  
  }
  
void print_results(float pHValue, int Co2Value, int methaneValue, int sensorThres) //can likely use this function to send data to I2C
  {
    Serial.print("pH Value is ");
    Serial.print(pHValue);
    Serial.print("\n");
    Serial.print("Co2 Value is ");
    Serial.print(Co2Value);
    Serial.print(" ppm");
    Serial.print("\n");
    Serial.print("methane Value is ");
    Serial.print(methaneValue);
    Serial.print(" ppm");
      if (methaneValue > sensorThres) //likely want to do more complex math here to allow for an accepted average range without telling user value is high or low
        {Serial.print(" (relatively high)");}
      else
        {Serial.print(" (relatively low)");}
    Serial.print("\n");  
    Serial.print("Humidity is ");        
    Serial.print(DHT.humidity, 1); 
    Serial.print(" %"); //not sure if this is directly reported in % from sensor
    Serial.print("\n");
    Serial.print("Temperature is ");
    Serial.println(DHT.temperature, 1);
    Serial.print(" degrees celcius");
    Serial.print("\n");
    
}

void calibrate_sensors(void){ //figure out if other sensors need calibration

    co2Sensor.calibrate(); //calibrate Co2 Sensor  (should be done in a separate function -- check other sensors to see if they need calibration too)
    co2Sensor.begin(v400, v40000); // set the reference values for co2 sensor in the library  (this stays here (only remove above line)
    Serial.print("Co2 Sensor calibrated \n");
    
}

void write_handler(uint8_t reg, uint8_t val) {
    switch (reg) {
        case 1:
            selector = val;
            break;
    }
}

uint8_t read_handler(uint8_t reg) {
    switch (reg) {
        case 1:
            return selector;
    }

    return 0;
}
