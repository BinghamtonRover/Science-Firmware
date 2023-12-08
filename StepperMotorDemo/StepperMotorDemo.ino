#include "src/utils/BURT_utils.h"

#include "src/pinouts.h"
#include "src/science.pb.h"

#define BLOCK_DELAY 10

void setup() {
  Serial.begin(9600);
  Serial.println("init...");

  Serial.println("init stepper motor...");
  stepperLinear1.presetup();
  stepperLinear2.presetup();

  stepperLinear1.setup();
  stepperLinear2.setup();

  stepperLinear1.moveBySteps(2000000);
  stepperLinear2.moveBySteps(2000000);

}

void block() {
  // while (stepperLinear1.isMoving()) delay(BLOCK_DELAY);
  // while (stepperLinear2.isMoving()) delay(BLOCK_DELAY);
}
 
void loop() {
  stepperLinear1.update();
  // Serial.print("sg result: ");
  // Serial.println(stepperLinear1.didStall());
  stepperLinear2.update();
  // Serial.print("did stepper 2 stall: ");
  // Serial.println(stepperLinear2.didStall());
 
  // Serial.println("moving forward...");
  // stepperLinear1.moveBySteps(2000000);
  // stepperLinear2.moveBySteps(2000000);
  // block();
  // delay(1000);
  // Serial.println("moving backwards...");
  // stepperLinear1.moveBySteps(-2000000);
  // stepperLinear2.moveBySteps(-2000000);
  // block();
  // delay(1000);
}