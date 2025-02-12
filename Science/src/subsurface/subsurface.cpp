#include "subsurface.h"

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