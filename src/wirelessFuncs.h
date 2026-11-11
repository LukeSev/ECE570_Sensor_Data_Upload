#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <cmath>
#include <ArduinoJson.h>

#include <private_credentials.h>
#include <controlSensors.h>
#include <dataSensors.h>
#include <timing.h>

/*
;-----------------------------------------------------------------------------
; Main Program:
;-----------------------------------------------------------------------------
*/

// Function Declarations
void wifiConnect();
void sendData(String nodeName, String date, float distance, int lightRaw, bool lightOn);
void checkSwitches();
String getDateFromTimezone(String timezone);
String parseTimeAPIResponse(String jsonResponse);

// WiFi information
String SSID = HOME_SSID;
String PW = HOME_PW;

WiFiClient client;

String uploadURL = "https://lucasloughner.site/upload_sensor_data_lightdistance.php?";
String uploadData = "";

void wifiConnect() {
  WiFi.begin(SSID, PW);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Signal Strength (RSSI): %ld dBm\n", WiFi.RSSI());
}

void sendData(String nodeName, String date, float distance, int lightRaw, bool lightOn) {
  if(WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // Disable SSL certificate verification
    HTTPClient https;

    String fullURL = uploadURL + "node_name=" + nodeName + "&time_received=" + date + "&distance=" + String(distance) + "&light_raw=" + String(lightRaw) + "&light_on=" + String(lightOn);
    Serial.println("Sending data: " + fullURL);
    
    if(https.begin(client, fullURL)) {
        int httpCode = https.GET();

        Serial.println("Response Code: <--- " + String(httpCode));
        while(httpCode <= 0) {
          Serial.println("Error on HTTP request, retrying in 30s...");
          delay(30000); // Wait for 30 seconds before retrying
          httpCode = https.GET();
          Serial.println("Response Code: <--- " + String(httpCode));
        }
        String payload = https.getString();
        Serial.println("Response Payload: <--- " + payload);

        // if(httpCode > 0) {
        //     String payload = https.getString();
        //     Serial.println("Response Payload: <--- " + payload);
        // } else {
        //     Serial.println("Error on HTTP request");
        // }
        https.end();
    }

  } else {
    Serial.println("WiFi not connected");
  }
}

void checkSwitches() {
  int buttonState = readButtonState();
  int tiltSwitchState = readTiltSwitchState();
  bool upload = false;
  String nodeName = "invalid";

  // Only upload data when switch goes HIGH --> LOW (new trigger)
  if(buttonState == LOW && prevButtonState == HIGH) {
    Serial.println("Button Pressed");
    upload = true;
    nodeName = "node_1";
  } else if(tiltSwitchState == LOW && prevTiltSwitchState == HIGH) {
    Serial.println("Tilt Switch Activated");
    upload = true;
    nodeName = "node_2";
  }

  if(upload) {
    float distance = round(readDistanceCm())*100.0f/100.0f; // Round to 2 decimal places
    int lightRaw = readLightLevel();
    bool lightOn = (lightRaw < LIGHT_ONOFF_THRESHOLD);  // Lower value means brighter environment
    String date = getDateFromTimezone(selectedTimezone);

    sendData(nodeName, date, distance, lightRaw, lightOn);
  }
  prevButtonState = buttonState;
  prevTiltSwitchState = tiltSwitchState;
}

// Get current date in dateTime format using TimeAPI
String getDateFromTimezone(String timezone) {
  if(WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure(); // Disable SSL certificate verification
    HTTPClient https;

    String apiURL = "https://timeapi.io/api/Time/current/zone?timeZone=" + timezone;
    if(https.begin(client, apiURL)) {
        int httpCode = https.GET();

        Serial.println("Response Code: <--- " + String(httpCode));
        if(httpCode > 0) {
          // Successful response, parse JSON
          String payload = https.getString();
          return parseTimeAPIResponse(payload);
        } else {
            Serial.println("Error on HTTP request");
        }
        https.end();
    }

  } else {
    Serial.println("WiFi not connected");
  }
  return "2025-12-08%2012:00:32"; // Placeholder date
}

String parseTimeAPIResponse(String jsonResponse) {
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonResponse);
  String dateTimeRaw = doc["dateTime"];
  size_t pos = dateTimeRaw.indexOf('.');
  String dateTimeFixed = dateTimeRaw.substring(0, pos);
  pos = dateTimeFixed.indexOf('T');
  dateTimeFixed.replace("T", "%20");
  // Serial.println("FIXED: " + dateTimeFixed);
  return dateTimeFixed;
}