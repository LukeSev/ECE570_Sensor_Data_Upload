/*
;-----------------------------------------------------------------------------
; Title: Upload ESP Sensor Data
;-----------------------------------------------------------------------------
; Program Detail:
------------------------------------------------------------------------------
; Purpose: Take measurements from Proximity Sensor and Light Sensor, and upload to remote database over WiFi
; Inputs: Push Button, Tilt Switch, Proximity Sensor, Light Sensor
; Outputs: Distance and Light Level/Light Status uploaded to remote database
; Date: 2026-11-2
; Compiler: GCC
; Author: Lucas Loughner
; Versions:
;     1.0 - Initial Version
;-----------------------------------------------------------------------------
; File Dependencies:
; wirelessFuncs.h
; controlSensors.h
; dataSensors.h
; timing.h
;-----------------------------------------------------------------------------
*/

#include <Arduino.h>
#include <wirelessFuncs.h>

#define READ_DELAY 250 // Delay between switch checks in milliseconds


void setup() {
  Serial.begin(9600);
  setupControlSensors();
  setupDataSensors();
  wifiConnect();
  getTimezone();
  delay(READ_DELAY); // Sleep for 1 second
}

void loop() {
  checkSwitches();
  delay(READ_DELAY); // Sleep for 1 second
}
