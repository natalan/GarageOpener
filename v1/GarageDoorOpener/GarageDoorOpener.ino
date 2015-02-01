//*****************************************************************************
/// @file
/// @brief
///   Garage door opener for two doors
//*****************************************************************************
#include <SoftwareSerial.h>   
#include <SmartThings.h>


#define PIN_THING_RX    3
#define PIN_THING_TX    2


SmartThingsCallout_t messageCallout;    // call out function forward decalaration
SmartThings smartthing(PIN_THING_RX, PIN_THING_TX, messageCallout);  // constructor


int ledPin = 13;
int relayPin1 = 7;
int relayPin2 = 8;
bool isDebugEnabled;    // enable or disable debug in this example

void setup() {
  // setup default state of global variables
  isDebugEnabled = true;
  
  // setup hardware pins 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(relayPin1, OUTPUT); 
  pinMode(relayPin2, OUTPUT); 
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);

  if (isDebugEnabled) {
    Serial.begin(9600); // setup serial with a baud rate of 9600
    Serial.println("finished setup.."); 
  }
}


void loop() {
  // run smartthing logic
  smartthing.run();
}

void openDoor(int door) {
  String data;
  digitalWrite(ledPin, LOW);
  digitalWrite(door, LOW);
  smartthing.shieldSetLED(0, 1, 0);
  delay(2000);
  digitalWrite(ledPin, HIGH);
  digitalWrite(door, HIGH);
  smartthing.shieldSetLED(0, 0, 0);
  data = (door == 7) ? "open1" : "open2";

  if (isDebugEnabled) {
    Serial.println("Open called. Sending to the cloud:");
    Serial.println(data);
  }
  smartthing.send(data);
}

void closeDoor(int door) {
  String data;
  digitalWrite(ledPin, LOW);
  digitalWrite(door, LOW);
  smartthing.shieldSetLED(0, 0, 1);
  delay(2000);
  digitalWrite(ledPin, HIGH);
  digitalWrite(door, HIGH);
  smartthing.shieldSetLED(0, 0, 0);

  data = (door == 7) ? "close1" : "close2";
  if (isDebugEnabled) {
    Serial.println("Close called. Sending to the cloud:");
    Serial.println(data);
  }
  
  smartthing.send(data);
}


void messageCallout(String message) {
  if (isDebugEnabled) {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

  if (message.equals("open1")) {
    openDoor(relayPin1);
  } else if (message.equals("open2")) {
    openDoor(relayPin2);
  } else if (message.equals("close1")) {
    closeDoor(relayPin1);
  } else if (message.equals("close2")) {
    closeDoor(relayPin2);
  } else {
    Serial.println("Unknown message. Ignored.");
  }
}
