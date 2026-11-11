#include <HCSR04.h>

// Function Declarations
void setupDataSensors();
float readDistanceCm();
int readLightLevel();

// GPIO pins
const int trigPin = D5;
const int echoPin = D6;
const int photoresistorPin = A0;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

// Raw value threshold to determine if light is considered ON/OFF
// May need to change based on environment
// Below threshold = ON, Above threshold = OFF
#define LIGHT_ONOFF_THRESHOLD 700   

void setupDataSensors() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(photoresistorPin, INPUT);
}

float readDistanceCm() {
    long duration;
    
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculate the distance in cm
    return duration * SOUND_VELOCITY/2;
}

int readLightLevel() {
    return analogRead(photoresistorPin);
}