#include "lidar.h"

LidarSensor::LidarSensor(){}
LidarSensor::~LidarSensor(){}

void setup(){
    lidar.begin(0, true);
    lidar.configure(0);
}

void update(){
    distance = lidar.distance();
}

float getDistance(){
    return distance;
}