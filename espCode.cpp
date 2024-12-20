#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000
#define VIBRATION_PIN 15  // GPIO15 for vibration sensor

// WiFi credentials
const char* ssid = "Tejas";  // Replace with your WiFi name
const char* password = "1234abcd";  // Replace with your WiFi password

// ThingSpeak API details
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "RGQU89W5D3J8XDV6";  // Replace with your ThingSpeak API key

PulseOximeter pox;
uint32_t tsLastReport = 0;
float heartRate = 0.0;
float spo2 = 0.0;

// Callback for pulse detection
void onBeatDetected() {
    Serial.println("Beat detected!");
}

void setup() {
    Serial.begin(115200);

    // Initialize Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Initialize MAX30100
    Wire.begin(14, 2);  // SDA on GPIO14, SCL on GPIO2
    Serial.println("Initializing MAX30100...");
    if (!pox.begin()) {
        Serial.println("Failed to initialize MAX30100! Halting...");
        while (true);
    }
    Serial.println("MAX30100 initialized.");
    pox.setOnBeatDetectedCallback(onBeatDetected);

    // Set IR LED current
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Vibration sensor pin setup
    pinMode(VIBRATION_PIN, INPUT);
}

void loop() {
    // Update MAX30100
    pox.update();

    // Check if reporting period has elapsed
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        // Get readings
        heartRate = pox.getHeartRate();
        spo2 = pox.getSpO2();
        bool isMoving = digitalRead(VIBRATION_PIN);

        // Print sensor data
        Serial.print("Heart Rate: ");
        Serial.print(heartRate);
        Serial.println(" bpm");

        Serial.print("SpO2: ");
        Serial.print(spo2);
        Serial.println(" %");

        Serial.print("Vibration: ");
        Serial.println(isMoving ? "Moving" : "Stationary");

        // Send data to ThingSpeak
        sendToThingSpeak(heartRate, spo2, isMoving);

        tsLastReport = millis();
    }

    delay(500);
}

void sendToThingSpeak(float heartRate, float spo2, bool isMoving) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(server) + "?api_key=" + apiKey +
                     "&field1=" + String(heartRate) +
                     "&field2=" + String(spo2) +
                     "&field3=" + String(isMoving);
        http.begin(url);

        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            Serial.print("ThingSpeak response: ");
            Serial.println(httpResponseCode);
        } else {
            Serial.print("Error connecting to ThingSpeak: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    } else {
        Serial.println("WiFi not connected. Skipping ThingSpeak update.");
    }
}
