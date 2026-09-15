/*
 * LED Timeline Animation with Tween Library
 * 
 * Made by: Massimo Banzi & Jose Chavarria
 * Date: February 2026
 * Institution: Copenhagen Institute of Interaction Design
 * Course: Tangible User Interface
 * Library:
 * Tween by Hideaki Tai - https://github.com/hideakitai/Tween
 */

#include <Tween.h>

// Create a timeline to coordinate multiple animations
Tween::Timeline timeline;

// Variables to store LED brightness values (0-255)
float redLedVal = 0.f;
const int redLedPin = 5;

float blueLedVal = 0.f;
const int blueLedPin = 6;

// Button setup
const int buttonPin = 0;
bool buttonState = false;
bool prevButtonState = false;  // Track previous state to detect button press

void setup() {
  Serial.begin(9600);
  
  // Configure pins
  pinMode(buttonPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  delay(2000);  // Wait 2 seconds before starting

  // ========================================
  // BLUE LED TIMELINE (Linear - No Easing)
  // ========================================
  timeline.add(blueLedVal)           // Add blueLedVal to the timeline
    .init(0)                         // Initialize at 0 (LED off)
    .then(255, 500)                 // Fade to 255 over 500ms (0.5 second)
    .hold(200)                       // Stay at 255 for 200ms
    .then(0, 3000)                   // Fade to 0 over 3000ms (3 seconds)
    .hold(1500)                      // Stay at 0 for 1500ms
    .then(255, 3500)                 // Fade to 255 over 3500ms (3.5 seconds)
    .hold(1500)                      // Stay at 255 for 1500ms
    .then(0, 1000);                  // Fade to 0 over 1000ms

  // ========================================
  // RED LED TIMELINE (With Easing Functions)
  // ========================================
  timeline.add(redLedVal)                    // Add redLedVal to the timeline
    .init(0)                                 // Initialize at 0 (LED off)
    .then<Ease::ElasticOut>(255, 2000)       // Elastic bounce to 255 over 2 seconds
    .hold(500)                               // Hold at 255 for 500ms
    .then<Ease::SineIn>(0, 2000)             // Smooth sine fade to 0 over 2 seconds
    .hold(1000)                              // Hold at 0 for 1 second
    .then<Ease::BounceOut>(255, 1000)        // Bouncing fade to 255 over 1 second
    .hold(500)                               // Hold at 255 for 500ms
    .then<Ease::ExpoIn>(0, 2000);            // Exponential fade to 0 over 2 seconds
}

void loop() {
  // Read current button state
  buttonState = digitalRead(buttonPin);
  
  // Check if button was just pressed (transition from LOW to HIGH)
  if (buttonState == true && prevButtonState == false) {
    timeline.restart();  // Reset timeline to beginning
    timeline.start();    // Start the animation
  }
  
  // Update the timeline (this calculates new LED values)
  timeline.update();

  // Write the calculated values to the LED pins
  analogWrite(redLedPin, redLedVal);
  analogWrite(blueLedPin, blueLedVal);
  
  // Store current button state for next loop iteration
  prevButtonState = buttonState;
}
