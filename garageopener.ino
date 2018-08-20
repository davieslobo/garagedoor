#include "garageDoor.h"

const int CYCLETIME_DETERMINE_MOVEMENT = 5;//timeout value for when to stop checking distance to door


int triggerDistance_North = D0;
int echo_North = D1;
int movementSensor_North = D2;
int doorControl_North = D3;

//TODO Declare southdoor

//declare doors
GarageDoor doorNorth;
GarageDoor doorSouth;

void setup() {
  Serial.begin(115000);
  pinMode(triggerDistance_North, OUTPUT);
  pinMode(echo_North, INPUT);
  digitalWrite(triggerDistance_North, LOW);
  digitalWrite(movementSensor_North, LOW);
  digitalWrite(doorControl_North, HIGH);

  attachInterrupt(digitalPinToInterrupt(movementSensor_North), movementDetected, CHANGE);

  doorNorth.setName('doorNorth');
  doorSouth.setName('doorSouth');

}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 5; i > 0; i--) {
    Serial.print("Detect doorstatus in "); Serial.print(i);
    Serial.println("");
    delay(1000);
  }
  Serial.println(measureDistance(doorNorth.getName()));

}



unsigned long  measureDistance(char door) {

  if (door == 'doorNorth') {
    digitalWrite(triggerDistance_North, HIGH);
    delay(10);
    digitalWrite(triggerDistance_North, LOW);
    unsigned int duration = pulseIn(echo_North, HIGH);
    Serial.println("heyhey");
    return duration / 58;
  } else if (door == 'doorSouth') {
    return -1;
  }


}

void movementDetected() {
}

void determineDoorState(char door) {
  if(door = 'doorNorth'){
    Serial.println("yeaah");
  }

}





