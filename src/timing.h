// Function declarations
void getTimezone();

// List of timezones, to be presented to users
const String timezoneNames[] = {
    "Eastern Time Zone (ET)",
    "Central Time Zone (CT)",
    "Mountain Time Zone (MT)",
    "Pacific Time Zone (PT)",
    "Alaska Time Zone (AKT)",
    "Hawaii-Aleutian Time Zone (HAT)",
    "Atlantic Time Zone (AT)",
};

// Corresponsing timezone values to be sent to API
const String timezoneVals[] = {
    "America/New_York",
    "America/Chicago",
    "America/Denver",
    "America/Los_Angeles",
    "America/Anchorage",
    "Pacific/Honolulu",
    "America/Puerto_Rico",
};

const int nTimezones = sizeof(timezoneNames) / sizeof(timezoneNames[0]);
String selectedTimezone = timezoneVals[0];  // Default value before selection

void getTimezone() {
    // Output prompt for timezone to Serial Monitor
    Serial.println("Please select a timezone: ");
    for(int i = 0; i < nTimezones; i++) {
        Serial.println(String(i+1) + ": " + timezoneNames[i]);
    }
    
    // Wait for valid user input
    int selection = 0;
    while(selection < 1 || selection > nTimezones) {
        if(Serial.available()) {
            selection = Serial.parseInt();
        }
    }
    Serial.println("You selected: " + timezoneNames[selection - 1] + "(" + timezoneVals[selection - 1] + ")");
    selectedTimezone = timezoneVals[selection - 1]; // Update global var
}