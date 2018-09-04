#include "garageDoor.h"
#include "transport.h"

const int CYCLETIME_DETERMINE_MOVEMENT = 5;//timeout value(seconds) for when to stop checking distance to door
const int DOOR_CLOSED_DISTANCE = 15;
const int DOOR_OPEN_DISTANCE = 5;
const String MQTT_TOPIC = "Elgvegen1/Garage/";

//declare doors
int triggerDistance_North = D0;
int echo_North = D1;
int movementSensor = D2;
int doorControl_North = D3;

int triggerDistance_South = D4;
int echo_South = D5;
int movementSensor_South = D6;
int doorControl_South = D7;


GarageDoor doorNorth;
GarageDoor doorSouth;




void setup() {
  Serial.begin(115000);
  pinMode(triggerDistance_North, OUTPUT);
  pinMode(echo_North, INPUT);
  digitalWrite(triggerDistance_North, LOW);
  digitalWrite(movementSensor, LOW);
  digitalWrite(doorControl_North, HIGH);

  attachInterrupt(digitalPinToInterrupt(movementSensor), movementDetected, CHANGE);


  //initialize doors
  doorNorth.setName("doorNorth");
  doorSouth.setName("doorSouth");

  determineState(doorNorth.getName());
  //  determineState(doorSouth.getName());

  //init Wifi and MQTT connection
  wifiConnect();
  mqttInit();
  mqttConnect();

}

void loop() {
  // put your main code here, to run repeatedly:

  while (WiFi.status() != WL_CONNECTED) {
    wifiConnect();

  }
  for (int i = 5; i > 0; i--) {
    Serial.print("Detect doorstatus in "); Serial.print(i);
    Serial.println("");
    delay(1000);
  }
    Serial.println(determineState(doorNorth.getName()));
  mqttPublish(MQTT_TOPIC + doorNorth.getName(), determineState(doorNorth.getName()));
  mqttClient.loop();
}

unsigned long  measureDistance(String door) {

  if (door == "doorNorth") {
    digitalWrite(triggerDistance_North, HIGH);
    delay(10);
    digitalWrite(triggerDistance_North, LOW);
    unsigned int duration = pulseIn(echo_North, HIGH);
    //    Serial.print("Distance for "+door); Serial.print(duration/58);
    return duration / 58;
  } else if (door == "doorSouth") {
    digitalWrite(triggerDistance_South, HIGH);
    delay(10);
    digitalWrite(triggerDistance_South, LOW);
    unsigned int duration = pulseIn(echo_South, HIGH);
    //    Serial.print("Distance for "+door); Serial.print(duration/58);
    return duration / 58;
  }


}

void movementDetected() {
  //TODO - send door states to broker
  String doorState_North = determineState("doorNorth");
  String doorState_South = determineState("doorSouth");
}

String determineState(String doorname) {
  Serial.println("**************************");
  int currentDoorDistance = measureDistance(doorname);
  delay(200);
  int lastDoorDistance = currentDoorDistance;
  currentDoorDistance = measureDistance(doorname);
  int deltaDoorDistance = lastDoorDistance - currentDoorDistance;


      Serial.print("DoorDistance "); Serial.println(currentDoorDistance);
      Serial.print("Delta"); Serial.println(deltaDoorDistance);

  if (currentDoorDistance > DOOR_CLOSED_DISTANCE && deltaDoorDistance == 0) {
    Serial.println("Door closed");

  } else if (deltaDoorDistance > 0) {
    return "Door opening";
  } else if (deltaDoorDistance < 0) {
    return "Door closing";
  }  else if (deltaDoorDistance == 0 && currentDoorDistance >= DOOR_CLOSED_DISTANCE) {
    return "Door closed";
  } else if (deltaDoorDistance == 0 && currentDoorDistance < DOOR_OPEN_DISTANCE) {
    return "Door open";
  } else if (deltaDoorDistance == 0 && currentDoorDistance > DOOR_OPEN_DISTANCE) {
    return "In between";
  } else return "oops, no state configured";


//  lastDoorDistance = currentDoorDistance;
//  currentDoorDistance = measureDistance(doorname);
//  deltaDoorDistance = lastDoorDistance - currentDoorDistance;

  delay(1000);

  Serial.println("**************************");

}





