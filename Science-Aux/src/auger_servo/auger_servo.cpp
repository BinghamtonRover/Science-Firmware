#include "auger_servo.h"

AugerServo::AugerServo(int servoPIN, int openPOS, int closePOS){
    this->servoPIN = servoPIN;
    this->openPOS = openPOS;
    this->closePOS = closePOS;
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


