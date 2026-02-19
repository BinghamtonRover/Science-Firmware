#pragma once
#include <Arduino.h>
#include "src/LIDARLite.h"

class LidarSensor {
    private:
        LIDARLite lidar;
        float distance = 0;

    public:
        LidarSensor();
        ~LidarSensor();
        
        void setup();
        void update();
        float getDistance();
};