#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <DHT.h>
#include<ArduinoJson.h>
#define DHTPIN 15         
#define DHTTYPE DHT11     
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000
const int sensorPin = 25;
// Create a PulseOximeter object
PulseOximeter pox;

// Time at which the last beat occurred
uint32_t tsLastReport = 0;

// Callback routine is executed when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}
// Network credentials
const char* ssid = "student-OptiPlex-3000";
const char* password = "1234abcd";

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Variables to store sensor readings
float temperature = 0.0;
float humidity = 0.0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // SDA on GPIO21, SCL on GPIO22

  // Initialize DHT sensor
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Define the web server route
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html = "<html><body>";
    // html += "<h1>Sensor Data</h1>";
    html += "<p>Temperature: " + String(temperature) + " &deg;C</p>";
    // html += "<p>Humidity: " + String(humidity) + " %</p>";
    // html += "<p>Heart Rate: " + String(pox.getHeartRate()) + " bpm</p>";
    // html += "<p>SpO2: " + String(pox.getSpO2()) + " %</p>";
    html += "</body></html>";
    request->send(200, "text/html", html);
});
  //Pulse Oximeter Setup

    // Serial.print("Initializing pulse oximeter..");

    // // Initialize sensor
    // if (!pox.begin()) {
    //     Serial.println("FAILED");
    //     for(;;){
    //       Serial.println("EXIT");}
    // } else {
    //     Serial.println("SUCCESS");
    // }

  // Configure sensor to use 7.6mA for LED drive
//   pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

//     // Register a callback routine
//     pox.setOnBeatDetectedCallback(onBeatDetected);
  
//    pinMode(sensorPin, INPUT); //vibration sensor
// 
}

void loop() {
  // Update sensor readings
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Check if the readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the sensor readings to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  // Wait before the next reading
  delay(2000);

  // Pulse-Oximeter code
      // Read from the sensor
  //   pox.update();

  //   // Grab the updated heart rate and SpO2 levels
  //   if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
  //       Serial.print("Heart rate:");
  //       Serial.print(pox.getHeartRate());
  //       Serial.print("bpm / SpO2:");
  //       Serial.print(pox.getSpO2());
  //       Serial.println("%");

  //       tsLastReport = millis();
  //   }
   
  //   if (digitalRead(sensorPin)) {               // Check if there is any vibration detected by the sensor
  //   Serial.println("Animal moving....");
  //   } 
  // else {
  //   Serial.println("Stationary");  // Print "..." other wise
  // }

  // Add a delay to avoid flooding the serial monitor
  delay(1000);
}
