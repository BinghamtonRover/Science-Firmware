#include "subsurface.h"

// Removed servo pin hard code, put it in constructor
const int openPOS = 90;
const int closePOS = 150;

AugerServo::AugerServo(int servoPIN){
    this->servoPIN = servoPIN;
}

void AugerServo::setup(){
    servo.attach(servoPIN);
}
void AugerServo::open(){
    servo.write(openPOS);
}
void AugerServo::close(){
    servo.write(closePOS); 
}

void AugerServo::handleCommand(ScienceCommand command){
    if (command.lower_auger_servo == ServoState_SERVO_OPEN) open();
    else if (command.upper_auger_servo == ServoState_SERVO_CLOSE) close();
}

bool AugerServo::attached(){
    return servo.attached();
}