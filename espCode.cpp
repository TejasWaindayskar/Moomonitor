#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000
const int vibrationSensorPin = 15; // GPIO15 for vibration sensor

// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;

// Callback routine executed when a pulse is detected
void onBeatDetected() {
    Serial.println("Beat detected!");
}

// Network credentials
const char* ssid = "your-SSID";
const char* password = "your-PASSWORD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Variables for sensor readings
float heartRate = 0.0;
float spo2 = 0.0;

void setup() {
    Serial.begin(115200);
    Wire.begin(14, 2); // SDA on GPIO14, SCL on GPIO2 (ESP32-CAM I2C)

    // Initialize Pulse Oximeter
    Serial.println("Initializing MAX30100...");
    if (!pox.begin()) {
        Serial.println("Failed to initialize MAX30100!");
        while (true); // Halt if initialization fails
    }
    Serial.println("MAX30100 initialized successfully!");
    pox.setOnBeatDetectedCallback(onBeatDetected);

    // Set IR LED current
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Set up the vibration sensor pin
    pinMode(vibrationSensorPin, INPUT);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Web server route
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        String html = "<html><body>";
        html += "<h1>ESP32-CAM Sensor Data</h1>";
        html += "<p>Heart Rate: " + String(heartRate) + " bpm</p>";
        html += "<p>SpO2: " + String(spo2) + " %</p>";
        html += "</body></html>";
        request->send(200, "text/html", html);
    });

    // Start the web server
    server.begin();
}

void loop() {
    // Update MAX30100 readings
    pox.update();

    // Check if reporting period has elapsed
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        heartRate = pox.getHeartRate(); // Get heart rate
        spo2 = pox.getSpO2(); // Get SpO2

        Serial.print("Heart Rate: ");
        Serial.print(heartRate);
        Serial.println(" bpm");

        Serial.print("SpO2: ");
        Serial.print(spo2);
        Serial.println(" %");

        tsLastReport = millis();
    }

    // Vibration sensor check
    if (digitalRead(vibrationSensorPin)) {
        Serial.println("Animal moving...");
    } else {
        Serial.println("Stationary");
    }

    delay(1000);
}
