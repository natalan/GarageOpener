//*****************************************************************************
#include <SoftwareSerial.h>   //TODO need to set due to some weird wire language linker, should we absorb this whole library into smartthings
#include <SmartThings.h>

//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
#define PIN_LED                         13

//Smartthings Shield Assignments
#define PIN_THING_RX                    3
#define PIN_THING_TX                    2

//Garage Door relay and contact assignments
#define PIN_RIGHT                       4
#define PIN_RIGHT_CONTACT               5
#define PIN_LEFT                        6
#define PIN_LEFT_CONTACT                7

//House Door Assignments
#define PIN_GYM_DOOR_CONTACT            8
#define PIN_GARAGE_DOOR_CONTACT         9
#define PIN_OFFICE_DOOR_CONTACT         10
#define PIN_LAUNDRY_DOOR_CONTACT        11

#define OPEN           HIGH
#define CLOSED         LOW
#define PUSH_DELAY     1000  // milliseconds to keep the garage door button "pushed"
#define UPDATE_DELAY   5000  //millseconds between update to SmartThings Cloud

//*****************************************************************************
// Global Variables   | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
SmartThingsCallout_t messageCallout;    // call out function forward decalaration
SmartThings smartthing(PIN_THING_RX, PIN_THING_TX, messageCallout);  // constructor

bool  leftClosed, rightClosed;
bool  garageClosed, gymClosed, laundryClosed, officeClosed;

bool isDebugEnabled;   // enable or disable debug
int stateLED;          // state to track last set value of LED
int stateNetwork;      // state of the network 
unsigned long timeLastUpdate;
unsigned long timeSinceLastUpdate;  
//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
// push left garage door button
void pushLeft() {
  if (leftClosed) {
    smartthing.send("leftDoor opening");
    Serial.println("leftDoor opening");
  } else {
    smartthing.send("leftDoor closing");
    Serial.println("leftDoor closing");
  }

  timeLastUpdate = millis();
  smartthing.shieldSetLED(0, 0, 2); // blue
  digitalWrite(PIN_LEFT,LOW);
  delay(PUSH_DELAY);
  digitalWrite(PIN_LEFT,HIGH);
  smartthing.shieldSetLED(0, 0, 0); // off
}

// push right garage door button
void pushRight() {
  if (rightClosed) {
    smartthing.send("rightDoor opening");
    Serial.println("rightDoor opening");
  } else {
    smartthing.send("rightDoor closing");
    Serial.println("rightDoor closing");
  }  
  
  timeLastUpdate = millis();
  smartthing.shieldSetLED(0, 0, 2); // blue
  digitalWrite(PIN_RIGHT,LOW);
  delay(PUSH_DELAY);
  digitalWrite(PIN_RIGHT,HIGH);
  smartthing.shieldSetLED(0, 0, 0); // off
}

bool isClosed(int pin) {
  return (digitalRead(pin) == CLOSED);
}

void updateDoorState() {
  if (millis() > timeLastUpdate) {
    timeSinceLastUpdate = millis() - timeLastUpdate;
  } else {
    timeLastUpdate = millis();
    timeSinceLastUpdate = 0;
  }

  if ((leftClosed != isClosed(PIN_LEFT_CONTACT)) || timeSinceLastUpdate >= UPDATE_DELAY) {
    leftClosed = isClosed(PIN_LEFT_CONTACT);
    
    if (leftClosed) {
      smartthing.send("leftDoor closed");
      Serial.println("leftDoor closed");
    } else {
      smartthing.send("leftDoor open");
      Serial.println("leftDoor open");
    }
  }
  if ((rightClosed != isClosed(PIN_RIGHT_CONTACT)) || timeSinceLastUpdate >= UPDATE_DELAY) {
    rightClosed = isClosed(PIN_RIGHT_CONTACT);
    if(rightClosed) {
      smartthing.send("rightDoor closed");
      Serial.println("rightDoor closed");
    } else {
      smartthing.send("rightDoor open");
      Serial.println("rightDoor open");
    }
  }
 
  if ((officeClosed != isClosed(PIN_OFFICE_DOOR_CONTACT)) || timeSinceLastUpdate >= UPDATE_DELAY) {
    officeClosed = isClosed(PIN_OFFICE_DOOR_CONTACT);
    if(officeClosed) {
      smartthing.send("officeDoor closed");
      Serial.println("officeDoor closed");
    } else {
      smartthing.send("officeDoor open");
      Serial.println("officeDoor open");
    }
  }

  if ((gymClosed != isClosed(PIN_GYM_DOOR_CONTACT)) || timeSinceLastUpdate >= UPDATE_DELAY) {
    gymClosed = isClosed(PIN_GYM_DOOR_CONTACT);
    if(gymClosed)
    {
      smartthing.send("gymDoor closed");
      Serial.println("gymDoor closed");
    } else {
      smartthing.send("gymDoor open");
      Serial.println("gymDoor open");
    }
  }
 
  if ((laundryClosed != isClosed(PIN_LAUNDRY_DOOR_CONTACT)) || timeSinceLastUpdate >= UPDATE_DELAY)
  {
    laundryClosed = isClosed(PIN_LAUNDRY_DOOR_CONTACT);
    if(laundryClosed)
    {
      smartthing.send("laundryDoor closed");
      Serial.println("laundryDoor closed");
    } else {
      smartthing.send("laundryDoor open");
      Serial.println("laundryDoor open");
    }
  }
  
  if ((garageClosed != isClosed(PIN_GARAGE_DOOR_CONTACT)) || timeSinceLastUpdate >= UPDATE_DELAY)
  {
    garageClosed = isClosed(PIN_GARAGE_DOOR_CONTACT);
    if(garageClosed)
    {
      smartthing.send("garageDoor closed");
      Serial.println("garageDoor closed");
    } else {
      smartthing.send("garageDoor open");
      Serial.println("garageDoor open");
    }
  } 
  if (timeSinceLastUpdate >= UPDATE_DELAY)
  {
    timeLastUpdate = millis();
  }
  
}

//*****************************************************************************
void setNetworkStateLED() {
  SmartThingsNetworkState_t tempState = smartthing.shieldGetLastNetworkState();
  if (tempState != stateNetwork) {
    switch (tempState) {
      case STATE_NO_NETWORK:
        if (isDebugEnabled) Serial.println("NO_NETWORK");
        smartthing.shieldSetLED(2, 0, 0); // red
        break;
      case STATE_JOINING:
        if (isDebugEnabled) Serial.println("JOINING");
        smartthing.shieldSetLED(2, 0, 0); // red
        break;
      case STATE_JOINED:
        if (isDebugEnabled) Serial.println("JOINED");
        smartthing.shieldSetLED(0, 0, 2); // blue
        break;
      case STATE_JOINED_NOPARENT:
        if (isDebugEnabled) Serial.println("JOINED_NOPARENT");
        smartthing.shieldSetLED(2, 0, 2); // purple
        break;
      case STATE_LEAVING:
        if (isDebugEnabled) Serial.println("LEAVING");
        smartthing.shieldSetLED(2, 0, 0); // red
        break;
      default:
      case STATE_UNKNOWN:
        if (isDebugEnabled) Serial.println("UNKNOWN");
        smartthing.shieldSetLED(0, 2, 0); // green
        break;
    }
    stateNetwork = tempState; 
  }
}

//*****************************************************************************
// API Functions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void setup() {
  // setup default state of global variables
  isDebugEnabled = true;
  stateLED = 0;                 // matches state of hardware pin set below
  stateNetwork = STATE_JOINED;  // set to joined to keep state off if off

  // setup hardware pins 
  pinMode(PIN_LED, OUTPUT);     // define PIN_LED as an output
  pinMode(PIN_RIGHT, OUTPUT);
  pinMode(PIN_LEFT, OUTPUT);
  digitalWrite(PIN_RIGHT, HIGH);
  digitalWrite(PIN_LEFT, HIGH);
  digitalWrite(PIN_LED, LOW);   // set value to LOW (off) to match stateLED=0

  pinMode(PIN_LEFT_CONTACT, INPUT_PULLUP);
  pinMode(PIN_RIGHT_CONTACT, INPUT_PULLUP);

  pinMode(PIN_OFFICE_DOOR_CONTACT, INPUT_PULLUP);
  pinMode(PIN_GYM_DOOR_CONTACT, INPUT_PULLUP);
  pinMode(PIN_LAUNDRY_DOOR_CONTACT, INPUT_PULLUP);
  pinMode(PIN_GARAGE_DOOR_CONTACT, INPUT_PULLUP);

  if (isDebugEnabled) { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("setup..");  // print out 'setup..' on start
  }

  // Get the Current State of the Doors
  Serial.println("Getting Door State...");
  if (isClosed(PIN_LEFT_CONTACT)) {
    leftClosed = true;
    smartthing.send("leftDoor closed");
    Serial.println("leftDoor closed");
  } else {
    leftClosed = false;
    smartthing.send("leftDoor open");
    Serial.println("leftDoor open");
  }

  delay(1000);

  if (isClosed(PIN_RIGHT_CONTACT)) {
    rightClosed = true;
    smartthing.send("rightDoor closed");
    Serial.println("rightDoor closed");
  } else {
    rightClosed = false;
    smartthing.send("rightDoor open");
    Serial.println("rightDoor open");
  }

  delay(1000);

  if (isClosed(PIN_OFFICE_DOOR_CONTACT)) {
    officeClosed = true;
    smartthing.send("officeDoor closed");
    Serial.println("officeDoor closed");
  } else {
    officeClosed = false;
    smartthing.send("officeDoor open");
    Serial.println("officeDoor open");
  }
  
  delay(1000);

  if (isClosed(PIN_GYM_DOOR_CONTACT)) {
    gymClosed = true;
    smartthing.send("gymDoor closed");
    Serial.println("gymDoor closed");
  } else {
    gymClosed = false;
    smartthing.send("gymDoor open");
    Serial.println("gymDoor open");
  }
  
  delay(1000);

  if (isClosed(PIN_LAUNDRY_DOOR_CONTACT)) {
    laundryClosed = true;
    smartthing.send("laundryDoor closed");
    Serial.println("laundryDoor closed");
  } else {
    laundryClosed = false;
    smartthing.send("laundryDoor open");
    Serial.println("laundryDoor open");
  }
  
  delay(1000);

  if (isClosed(PIN_GARAGE_DOOR_CONTACT)) {
    garageClosed = true;
    smartthing.send("garageDoor closed");
    Serial.println("garageDoor closed");
  } else {
    garageClosed = false;
    smartthing.send("garageDoor open");
    Serial.println("garageDoor open");
  }  
 
    timeLastUpdate = millis();
    timeSinceLastUpdate = 0;
}

//*****************************************************************************
void loop() {
  // run smartthing logic
  smartthing.run();

  // Check the open/closed state of the doors
  updateDoorState();

  // Code left here to help debut network connections
  setNetworkStateLED();
}

//*****************************************************************************
void messageCallout(String message) {
  // if debug is enabled print out the received message
  if (isDebugEnabled) {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

  if (message.equals("pushLeft")) {
    pushLeft();
  }
  else if (message.equals("pushRight")) {
    pushRight();
  }
}
