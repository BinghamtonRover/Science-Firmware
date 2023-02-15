#include "Vacuum_Servo.h"

vacuum_servo::vacuum_servo(int PIN) {
	pin = PIN;
};

vacuum_servo::open() {
	for (pos = 0; pos <= 180; pos += 1) { 
    myservo.write(pos);              
    delay(15); 
	}
};

vacuum_servo::close() {
	for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);              
    delay(15); 
  }
};