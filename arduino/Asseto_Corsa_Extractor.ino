#include <ArduinoJson.h>
#include <ArduinoJson.hpp>


#include <Joystick.h>


const int ledPin = 9;
const int upButtonPin = 21;
const int downButtonPin = 16;
const int jsonCapacity = 256;
int maxRpm = 0;
bool maxRpmSet = false;

const int holdDuration = 1000; // 3 seconds
unsigned long buttonPressedTime = 0;
bool upButtonPressed = false;


void setup()
{
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Joystick.begin();
}

void shortPress() {
  // Function called for short press
  //Serial.println("button pressed");
}

void heldForThreeSeconds() {
  // Function called when the button is held for more than 3 seconds
  Serial.println("Resetting max RPM...");
  maxRpmSet = false;
  analogWrite(ledPin, 255); // Turn on LED
}

void loop()
{
  // Read from serial port
  if (Serial.available() > 0) {

    String jsonString = Serial.readStringUntil('\n');
    
    // Allocate memory for the JSON object
    StaticJsonDocument<jsonCapacity> doc;
    
    // Deserialize the JSON object
    DeserializationError error = deserializeJson(doc, jsonString);
    
    // Check if deserialization was successful
    if (!error) {
      // Get the method from the JSON object
      String method = doc["method"];
      
      // Check if the method is "setup"
      if (method == "setup") {
        // Apply maxRpm value
        if (maxRpmSet == false) {
          analogWrite(ledPin, 0); // Turn off LED
          maxRpm = doc["maxRpm"];
          maxRpmSet = true;
          Serial.println("Set up max RPM of " + String(maxRpm));
        } 
      } 
      // Check if the method is "data"
      else if (method == "data" && maxRpmSet == true) {
        // Apply rpms value
        int rpms = doc["currentRpm"];
        // Turn on LEDs if rpms is high
        // Clever way to check if the recieved value is more than 0.95 of max rpm
        analogWrite(ledPin, rpms > maxRpm * 0.95 ? 255 : 0);
      }
    } else {
      // Failed to parse JSON
      Serial.println("Error parsing JSON");
    }
  }

    // Read the state of the down button
  int downButtonState = digitalRead(downButtonPin);

  // Check if the down button is pressed
  if (downButtonState == LOW) {
    // Serial.println("Down button pressed");
    Joystick.setButton(21, HIGH); // Set button 21 to pressed state
    // analogWrite(ledPin, 255);
  } else {
    Joystick.setButton(21, LOW); // Set button 21 to released state
    // analogWrite(ledPin, 0);
  }


  // Read the state of the up button
  int upButtonState = digitalRead(upButtonPin);

  if (upButtonState == LOW) {
    Joystick.setButton(20, HIGH);
    if (!upButtonPressed) {
      // Button was just pressed
      upButtonPressed = true;
      buttonPressedTime = millis(); // Record the time when the button was pressed
    }
  } else {
    Joystick.setButton(20, LOW);
    // Button is released
    if (upButtonPressed) {
      // Button was released after being pressed
      upButtonPressed = false;
      unsigned long elapsedTime = millis() - buttonPressedTime;
      if (elapsedTime >= holdDuration) {
        // Button was held for more than 3 seconds
        heldForThreeSeconds();
      } else {
        // Button was released before 3 seconds
        // shortPress();
      }
    }
  }

}
