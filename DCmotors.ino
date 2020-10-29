#include <SparkFun_TB6612.h> //ROB 14451 

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
#define STBY 9 //will need to be changed for final board integration
const int motor_offset = 1;

//create motors
Motor Pump1 = Motor(Pump1_in1, Pump1_in2, Pump1_PWM, motor_offset, STBY);
Motor Pump2 = Motor(Pump2_in1, Pump2_in2, Pump2_PWM, motor_offset, STBY);
Motor Pump3 = Motor(Pump3_in1, Pump3_in2, Pump3_PWM, motor_offset, STBY);
Motor Pump4_and_5 = Motor(Pump4_and_5_in1, Pump4_and_5_in2, Pump4_and_5_PWM, motor_offset, STBY);
Motor linear = Motor(linear_in1, linear_in2, linear_PWM, motor_offset, STBY);
Motor DC_motor = Motor(DC_motor_in1, DC_motor_in2, DC_motor_PWM, motor_offset, STBY);

void setup()
{
 //empty
}


void loop()
{

   Pump1.drive(255,100); //placeholder code -- runs pump 1 at speed 255 for 100 ms then turns off and loops
   Pump1.brake();
   delay(1000);
   
   
}
