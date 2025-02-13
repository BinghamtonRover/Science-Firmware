#include "subsurface.h"
#include "../science.pb.h"

const int ServoPIN = 25;
const int openPOS = 90;
const int closePOS = 150;

void SubSurfaceBox::setup(){
    subSurfaceServo.attach(ServoPIN);
}
void SubSurfaceBox::open(){
    subSurfaceServo.write(openPOS);
}
void SubSurfaceBox::close(){
    subSurfaceServo.write(closePOS); 
}

void SubSurfaceBox::handleCommand(ScienceCommand command){
    if (command.subsurface == ServoState_SERVO_OPEN) SubSurfaceBox.open();
    else if (command.subsuface == ServoState_SERVO_CLOSE) SubSurfaceBox.close();
}