#include "lidar.h"

LidarSensor::LidarSensor(){}
LidarSensor::~LidarSensor(){}

void LidarSensor::setup(){
    lidar.begin(0, true);
    lidar.configure(0);
}

void LidarSensor::update(){
    distance = lidar.distance();
}

float LidarSensor::getDistance(){
    return distance;
}