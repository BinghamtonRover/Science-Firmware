//arduino code for remotely testing the science subsystem 

#include <SoftwareSerial.h>

#define VRx 14 //A0
#define VRy 15 //A1
#define A_button 7
#define B_button 8
#define Y_button 9
#define X_button 10
SoftwareSerial BTSerial(6,4);

int joystick = 80;
int state, readstate, xPosition, yPosition, mapX, mapY, A_state, B_state, Y_state, X_state = 0;

void setup() {

  BTSerial.begin(9600);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(A_button, INPUT);
  pinMode(B_button, INPUT);
  pinMode(Y_button, INPUT);
  pinMode(X_button, INPUT);
  Serial.begin(9600);

}

void loop() {

   if(BTSerial.available() > 0){ 
      readstate = BTSerial.read();
      Serial.println(readstate);
   }
   
   readposition();
   delay(10);
   updatestate();
   delay(10);
   printstatus();
   delay(10);
   
}

void readposition(){

  xPosition = analogRead(VRy);
  yPosition = analogRead(VRx);
  mapX = map(xPosition, 0, 1023, -512, 512);
  mapY = map(yPosition, 0, 1023, -512, 512);
  A_state = digitalRead(A_button);
  B_state = digitalRead(B_button);
  Y_state = digitalRead(Y_button);
  X_state = digitalRead(X_button);
  
}

void updatestate(){

  if (mapY >= joystick & Y_state == 1){
    BTSerial.print('W');
    Serial.println("up and Y");
  }
  if (mapY <= -joystick & Y_state == 1){
    BTSerial.print('Z');
    Serial.println("down and Y");
  }
  else if (mapY >= joystick){ //up
    BTSerial.print('U');
    Serial.println("up");
  } 
  else if (mapY <= -joystick){ //down
    BTSerial.print('D');
    Serial.println("down");
  }
  else if (mapX >= joystick){ //right
    BTSerial.print('R');
    Serial.println("right");
  } 
  else if (mapX <= -joystick ){ //left
    BTSerial.print('L');
    Serial.println("left");
  }
  else if (A_state == 1){
    BTSerial.print('A');
    Serial.println("A");
  }
  else if (B_state == 1){
    BTSerial.print('B');
    Serial.println("B");
  }
  else if (Y_state == 1){
    BTSerial.print('Y');
    Serial.println("Y");
  }
  else if (X_state == 1){
    BTSerial.print('X');
    Serial.println("X");
  }
  else { //idle
    BTSerial.print('N');
    Serial.println("idle");
  }
  
}

void printstatus(){

  Serial.print("X: ");
  Serial.print(mapX);
  Serial.print(" | Y: ");
  Serial.print(mapY);
  Serial.print("\n");
  Serial.print("A button: ");
  Serial.print(A_state);
  Serial.print("\n");
  Serial.print("B button: ");
  Serial.print(B_state);
  Serial.print("\n");
  Serial.print("Y button: ");
  Serial.print(Y_state);
  Serial.print("\n");
  Serial.print("X button: "); 
  Serial.print(X_state);
  Serial.print("\n");
  
}
