/*
  TODO
  - Send MQTT-message to nodeMCU
  -Trigger OpenDoor and CloseDoorcommands
  - Add openDoor and Closedoorcommands based on state
  - Add username and password on MQTT
*/


#include "garageDoor.h"
#include "transport.h"

const int CYCLETIME_DETERMINE_MOVEMENT = 5;//timeout value(seconds) for when to stop checking distance to door
const int DOOR_CLOSED_DISTANCE = 15;
const int DOOR_OPEN_DISTANCE = 5;
const String MQTT_TOPIC = "Elgvegen1/Garage/";
const int DOOR_TRIGGER_TIME = 200; //value in milliseconds


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
  pinMode(doorControl_North, OUTPUT);
  digitalWrite(triggerDistance_North, LOW);
  digitalWrite(movementSensor, LOW);
  digitalWrite(doorControl_North, LOW);

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
  if (!mqttClient.connected()) {
    Serial.println("MQTT not connected");
    mqttConnect();
  }
  mqttClient.loop();
  Serial.println(".");
  delay(1000);


  /*
    for (int i = 5; i > 0; i--) {
    Serial.print("Detect doorstatus in "); Serial.print(i);
    Serial.println("");
    delay(1000);
    }
    Serial.println(determineState(doorNorth.getName()));
    if (!mqttClient.connected()) {
    mqttConnect();
    }
    mqttPublish(MQTT_TOPIC + doorNorth.getName(), determineState(doorNorth.getName()));
    mqttClient.loop(); */
}

unsigned long  measureDistance(String door) {

  if (door == "Elgvegen1/Garage/doorNorth") {
    digitalWrite(triggerDistance_North, HIGH);
    delay(10);
    digitalWrite(triggerDistance_North, LOW);
    unsigned int duration = pulseIn(echo_North, HIGH);
    //    Serial.print("Distance for "+door); Serial.print(duration/58);
    return duration / 58;
  } else if (door == "Elgvegen1/Garage/doorSouth") {
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
  Serial.print("Delta: "); Serial.println(deltaDoorDistance);

  if (currentDoorDistance > DOOR_CLOSED_DISTANCE && deltaDoorDistance == 0) {
    Serial.println("Door closed");

  } else if (deltaDoorDistance > 0) {
    return "Door opening";
  } else if (deltaDoorDistance < 0) {
    return "Door closing";
  } else if (deltaDoorDistance == 0 && currentDoorDistance >= DOOR_CLOSED_DISTANCE) {
    return "Door closed";
  } else if (deltaDoorDistance == 0 && currentDoorDistance < DOOR_OPEN_DISTANCE) {
    return "Door open";
  } else if (deltaDoorDistance == 0 && currentDoorDistance > DOOR_OPEN_DISTANCE) {
    return "In between";
  } else return "oops, no state configured";


  //  lastDoorDistance = currentDoorDistance;
  //  currentDoorDistance = measureDistance(doorname);
  //  deltaDoorDistance = lastDoorDistance - currentDoorDistance;



  Serial.println("**************************");

}

void closeDoor(String doorName) {
  Serial.println(doorName + " closing");
}

void openDoor(String doorName) {
  Serial.println(doorName + " opening");
}

void determineMQTTCommand(String doorName, int command) {

  switch (command) {
    case 1:
      openDoor(doorName);
      break;
    case 2:
      closeDoor(doorName);
      break;
    case 3:
      determineState(doorName);
      break;
    case 4:
      triggerNorth();
      break;
    default: Serial.println("No valid command"); break;
  }

}

void callback(char* topic, byte * payload, unsigned int payloadLength) {
  String recievedCommand;
  //Debug MQTT recieve
  Serial.print(topic); Serial.println(" has following message: ");
  
  for (int i = 0; i < payloadLength; i++) {
    
    recievedCommand += (char)payload[i];
  }
  Serial.println("Command recieved: " +recievedCommand);
  determineMQTTCommand(topic, recievedCommand.toInt());
}

void triggerNorth(){
    digitalWrite(doorControl_North, HIGH);
    delay(DOOR_TRIGGER_TIME);
    digitalWrite(doorControl_North, LOW);
    Serial.println("North triggered");
  
}

void triggerSouth(){
    digitalWrite(doorControl_South, HIGH);
    delay(DOOR_TRIGGER_TIME);
    digitalWrite(doorControl_South, LOW);
}

