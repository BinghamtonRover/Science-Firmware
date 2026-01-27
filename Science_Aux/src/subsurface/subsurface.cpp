#include "subsurface.h"

// Removed servo pin hard code, put it in constructor
const int openPOS = 90;
const int closePOS = 150;

void SubSurfaceBox::setup(){
    subSurfaceServo.attach(servoPIN);
}
void SubSurfaceBox::open(){
    subSurfaceServo.write(openPOS);
}
void SubSurfaceBox::close(){
    subSurfaceServo.write(closePOS); 
}

SubSurfaceBox::SubSurfaceBox(int servoPIN){
    this->servoPIN = servoPIN;
}

void SubSurfaceBox::handleCommand(ScienceCommand command){
    if (command.subsurface_motor == ServoState_SERVO_OPEN) open();
    else if (command.subsurface_motor == ServoState_SERVO_CLOSE) close();
}