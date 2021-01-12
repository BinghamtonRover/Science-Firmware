#include <SparkFun_TB6612.h> //ROB 14451 
#include <DRV8834.h> //closest to MP6500
#include <MultiDriver.h>
#include <SyncDriver.h>
#include <Arduino.h>

//pin definitions
#define Pump1_in1 2 //will need to be changed for final board integration
#define Pump1_in2 4 //will need to be changed for final board integration
#define Pump1_PWM 5 //will need to be changed for final board integration
#define Pump2_in1 7//will need to be changed for final board integration
#define Pump2_in2 8 //will need to be changed for final board integration
#define Pump2_PWM 6 //will need to be changed for final board integration
#define Pump3_in1 2 //will need to be changed for final board integration
#define Pump3_in2 4 //will need to be changed for final board integration
#define Pump3_PWM 5 //will need to be changed for final board integration
#define Pump4_and_5_in1 7//will need to be changed for final board integration
#define Pump4_and_5_in2 8 //will need to be changed for final board integration
#define Pump4_and_5_PWM 6 //will need to be changed for final board integration
#define linear_in1 2 //will need to be changed for final board integration
#define linear_in2 4 //will need to be changed for final board integration
#define linear_PWM 5 //will need to be changed for final board integration
#define DC_motor_in1 7//will need to be changed for final board integration
#define DC_motor_in2 8 //will need to be changed for final board integration
#define DC_motor_PWM 6 //will need to be changed for final board integration
#define STBY1 9 //will need to be changed for final board integration ------------------------------- top 2 rob 14451's share stby pin
#define STBY2 9 //will need to be changed for final board integration ------------------------------- bottom 2 rob 14451's share stby pin
const int motor_offset = 1;
#define MOTOR_STEPS 200 //steps per revolution -- 200 steps or 1.8 degrees/step
#define RPM 120 // Target RPM for cruise speed //need to find ideal value
// Acceleration and deceleration values are always in FULL steps / s^2
#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000
#define MICROSTEPS 16 //microstepping can make motors more accurate but require extra pins -- if we decide to microstep we will have to solder extra wires
#define Dirt_DIR 8 //will need to redefine based on board connections
#define Dirt_STEP 9 //will need to redefine based on board connections
#define Dirt_MS1 10 //will need to redefine based on board connections
#define Dirt_MS2 PIN_UNCONNECTED //will need to redefine based on board connections //ms pins are used for microstepping -- dont have to wire so I will look into ommiting from code -- library shows that pins are defined as unconnected so may not have to change
#define Auger_DIR 8 //will need to redefine based on board connections
#define Auger_STEP 9 //will need to redefine based on board connections
#define Auger_MS1 10 //will need to redefine based on board connections
#define Auger_MS2 11 //will need to redefine based on board connections //ms pins are used for microstepping -- dont have to wire so I will look into ommiting from code
#define Dirt_to_Auger_DIR 8 //will need to redefine based on board connections
#define Dirt_to_Auger_STEP 9 //will need to redefine based on board connections
#define Dirt_to_Auger_MS1 10 //will need to redefine based on board connections
#define Dirt_to_Auger_MS2 11 //will need to redefine based on board connections //ms pins are used for microstepping -- dont have to wire so I will look into ommiting from code

//create motors
Sparkfun_Motor Pump1(Pump1_in1, Pump1_in2, Pump1_PWM, motor_offset, STBY1);
Sparkfun_Motor Pump2(Pump2_in1, Pump2_in2, Pump2_PWM, motor_offset, STBY1);
Sparkfun_Motor Pump3(Pump3_in1, Pump3_in2, Pump3_PWM, motor_offset, STBY1);
Sparkfun_Motor linear(linear_in1, linear_in2, linear_PWM, motor_offset, STBY1);
Sparkfun_Motor Pump4_and_5(Pump4_and_5_in1, Pump4_and_5_in2, Pump4_and_5_PWM, motor_offset, STBY2);
Sparkfun_Motor DC_Motor(DC_motor_in1, DC_motor_in2, DC_motor_PWM, motor_offset, STBY2); //error is because it reads from stepper motor library instead of rob-14451 
DRV8834 Dirt_stepper(MOTOR_STEPS, Dirt_DIR, Dirt_STEP, Dirt_MS1, Dirt_MS2);
DRV8834 Auger_stepper(MOTOR_STEPS, Auger_DIR, Auger_STEP, Auger_MS1, Auger_MS2);
DRV8834 Dirt_to_Auger_stepper(MOTOR_STEPS, Dirt_to_Auger_DIR, Dirt_to_Auger_STEP, Dirt_to_Auger_MS1, Dirt_to_Auger_MS2);

void setup() {
  
    Serial.begin(115200);
    Dirt_stepper.begin(RPM, MICROSTEPS); //rotate dirt carousel
    Dirt_stepper.enable();
    Auger_stepper.begin(RPM, MICROSTEPS); //lower auger head
    Auger_stepper.enable();
    Dirt_to_Auger_stepper.begin(RPM, MICROSTEPS); //move dirt carousel below tests or auger 
    Dirt_to_Auger_stepper.enable();
    
}

void loop() { //for now this code just calls each function but we want each function to be called when a button on the remote is pressed 


    lower_auger();
    delay(1000);
    raise_auger();
    delay(1000);
    move_dirt_to_auger();
    delay(1000);
    move_dirt_to_tests();
    delay(1000);
    distribute_liquids();
    delay(1000);
    rotate_dirt();
    delay(1000);
   
   
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
   Pump4_and_5.drive(255,1000);
   Pump4_and_5.brake();
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
