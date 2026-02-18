#include "auger_servo.h"

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

bool AugerServo::attached(){
    return servo.attached();
}

void AugerServo::handleCommand(ServoState servo_state){
    if (servo_state == ServoState_SERVO_OPEN) open();
    else if (servo_state == ServoState_SERVO_CLOSE) close();
}


