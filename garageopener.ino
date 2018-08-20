#include "garageDoor.h"

const int CYCLETIME_DETERMINE_MOVEMENT = 3;//timeout value(seconds) for when to stop checking distance to door
const int DOOR_CLOSED_DISTANCE = 10; 
const int DOOR_OPEN_DISTANCE = 5;

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

  doorNorth.setName("doorNorth");
  doorSouth.setName("doorSouth");

  determineDoorState(doorNorth.getName());
  determineDoorState(doorSouth.getName());
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



unsigned long  measureDistance(String door) {

  if (door == "doorNorth") {
    digitalWrite(triggerDistance_North, HIGH);
    delay(10);
    digitalWrite(triggerDistance_North, LOW);
    unsigned int duration = pulseIn(echo_North, HIGH);
    Serial.println("Distance for "+door);
    return duration / 58;
  } else if (door == "doorSouth") {
    return -1;
  }


}

void movementDetected() {
  
}

void determineDoorState(String doorname) {
  int doorDistance = measureDistance(doorname); 
  int deltaDoorDistance = doorDistance;
  
  for(int i=0;i<CYCLETIME_DETERMINE_MOVEMENT; i++){
    if(doorDistance > DOOR_CLOSED_DISTANCE && deltaDoorDistance == 0){
      Serial.println("Door closed");
    }else if(deltaDoorDistance > 0){
      Serial.println("Door opening");
    }else if(deltaDoorDistance < 0){
      Serial.println("Door closing");
    }  else if(deltaDoorDistance > 0){
      Serial.println("Door opening");
    }else if(doorDistance < DOOR_OPEN_DISTANCE){
      Serial.println("Door open");
    }else if(deltaDoorDistance == 0 && doorDistance > DOOR_OPEN_DISTANCE){
      Serial.println("Door open, but not closed");
    }
  }
  
  
  if(doorname == "doorNorth"){
    Serial.println("Doorstate for door " + doorname);
  }else if(doorname == "doorSouth"){
    Serial.println("Doorstate for door " + doorname);
  }

}





