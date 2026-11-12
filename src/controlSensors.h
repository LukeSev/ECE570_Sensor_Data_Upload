// Function Declarations
void setupControlSensors();
int readButtonState();
int readTiltSwitchState();

// Define Pins for Tilt Switch and Button
const int buttonPin = D1; // Pin connected to the button
const int tiltSwitchPin = D2; // Pin connected to the tilt switch

// Look for HIGH-->LOW transition
int prevButtonState = HIGH;
int prevTiltSwitchState = HIGH;

void setupControlSensors() {
  pinMode(buttonPin, INPUT_PULLUP);     // Set button pin as input with internal pull-up resistor
  pinMode(tiltSwitchPin, INPUT_PULLUP); // Set tilt switch pin as input with internal pull-up resistor

  prevButtonState = digitalRead(buttonPin); // Get initial button state
  prevTiltSwitchState = digitalRead(tiltSwitchPin); // Get initial tilt switch state
}

// Read button state, returns HIGH if not pressed, LOW if pressed
int readButtonState() {
  return digitalRead(buttonPin); 
}

// Read tilt switch state, returns HIGH if not tilted, LOW if tilted
int readTiltSwitchState() {
  return digitalRead(tiltSwitchPin); // Read the state of the tilt switch (HIGH or LOW)
}