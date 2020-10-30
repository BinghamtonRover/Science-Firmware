#include <DRV8834.h> //closest to MP6500
#include <MultiDriver.h>
#include <SyncDriver.h>
#include <Arduino.h>


#define MOTOR_STEPS 200 //steps per revolution -- 200 steps or 1.8 degrees/step
#define RPM 120 // Target RPM for cruise speed //need to find ideal value

// Acceleration and deceleration values are always in FULL steps / s^2
#define MOTOR_ACCEL 2000
#define MOTOR_DECEL 1000

#define MICROSTEPS 16 //microstepping can make motors more accurate but require extra pins -- if we decide to microstep we will have to solder extra wires

//declare pin constants
#define Dirt_DIR 8 //will need to redefine based on board connections
#define Dirt_STEP 9 //will need to redefine based on board connections
#define Dirt_MS1 10 //will need to redefine based on board connections
#define Dirt_MS2 11 //will need to redefine based on board connections //ms pins are used for microstepping -- dont have to wire so I will look into ommiting from code
#define Auger_DIR 8 //will need to redefine based on board connections
#define Auger_STEP 9 //will need to redefine based on board connections
#define Auger_MS1 10 //will need to redefine based on board connections
#define Auger_MS2 11 //will need to redefine based on board connections //ms pins are used for microstepping -- dont have to wire so I will look into ommiting from code
#define Dirt_to_Auger_DIR 8 //will need to redefine based on board connections
#define Dirt_to_Auger_STEP 9 //will need to redefine based on board connections
#define Dirt_to_Auger_MS1 10 //will need to redefine based on board connections
#define Dirt_to_Auger_MS2 11 //will need to redefine based on board connections //ms pins are used for microstepping -- dont have to wire so I will look into ommiting from code
//create motors
DRV8834 Dirt_stepper(MOTOR_STEPS, Dirt_DIR, Dirt_STEP, Dirt_MS1, Dirt_MS2);
DRV8834 Auger_stepper(MOTOR_STEPS, Auger_DIR, Auger_STEP, Auger_MS1, Auger_MS2);
DRV8834 Dirt_to_Auger_stepper(MOTOR_STEPS, Dirt_to_Auger_DIR, Dirt_to_Auger_STEP, Dirt_to_Auger_MS1, Dirt_to_Auger_MS2);

void setup() {
  
    Serial.begin(115200);
    Dirt_stepper.begin(RPM, MICROSTEPS);
    Dirt_stepper.enable();
    Auger_stepper.begin(RPM, MICROSTEPS);
    Auger_stepper.enable();
    Dirt_to_Auger_stepper.begin(RPM, MICROSTEPS);
    Dirt_to_Auger_stepper.enable();
    
}

void loop() { 

        //placeholder code used for testing functionality
    Dirt_stepper.rotate(3);
    Dirt_stepper.stop();

    delay(5000);
}
