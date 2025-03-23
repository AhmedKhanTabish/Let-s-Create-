#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// Network credentials
const char* ssid = "TabishTP-Link_B736";
const char* password = "Zainabu@01";

// UV Sensor connected to ADC pin (A0 / GP26)
const int uvSensorPin = 26;

// Create a WiFi server on port 80
WiFiServer server(80);

// BMP280 sensor object
Adafruit_BMP280 bmp; // I2C

void setup() {
  Serial.begin(115200);

  // Initialize the UV sensor pin
  analogReadResolution(12);  // Set resolution to 12 bits for Pico W
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();

  // Initialize BMP280 sensor
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

float getUVIndex() {
  int rawValue = analogRead(uvSensorPin);   // Read raw analog value from UV sensor
  Serial.println(rawValue);
  float voltage = rawValue * (3.3 / 1023);  // Convert to voltage (for 12-bit resolution)
  Serial.println(voltage);
  float uvIndex = 0;
  if (voltage <= 1){
    uvIndex = (1.0 - voltage) / 0.1;  // Calculate UV index (approx 1V = 1 UV index)
    Serial.println(uvIndex);
  }
  else {
    uvIndex = (voltage - 1.0) / 0.1;
    Serial.println(uvIndex);
  }
  return uvIndex;
}

String getUVLevelDescription(float uvIndex) {
  if (uvIndex < 3.0) return "Safe (Low)";
  else if (uvIndex < 6.0) return "Moderate (Medium)";
  else if (uvIndex < 8.0) return "Harmful (High)";
  else return "Very Harmful (Extreme)";
}

void handleClient() {
  // Check if a client has connected
  WiFiClient client = server.accept();
  if (!client) return;

  // Wait until the client sends a request
  Serial.println("New Client Connected");
  while (!client.available()) delay(1);

  // Get the UV index and its description
  float uvIndex = getUVIndex();
  String uvLevelDescription = getUVLevelDescription(uvIndex);

  // Get temperature and pressure data from BMP280
  float temperature = bmp.readTemperature();  // Temperature in Celsius
  float pressure = bmp.readPressure() / 100.0F;  // Pressure in hPa (hPa = hPa / 100)

  // Set up UV level class based on the index range
  String uvClass;
  if (uvIndex < 3.0) uvClass = "safe";
  else if (uvIndex < 6.0) uvClass = "moderate";
  else if (uvIndex < 8.0) uvClass = "harmful";
  else uvClass = "very-harmful";

  // HTML response content
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>UV Light & Weather Detector</title>";
  html += "<style>";
  html += "body {font-family: Arial; text-align: center;}";
  html += ".level {font-size: 1.2em; color: #007bff;}";
  html += ".safe {color: green;}";
  html += ".moderate {color: orange;}";
  html += ".harmful {color: red;}";
  html += ".very-harmful {color: darkred;}";
  html += "</style></head>";
  html += "<body>";
  html += "<h1>UV Light & Weather Data</h1>";
  html += "<p class='level'>UV Index: <strong>" + String(uvIndex, 2) + "</strong></p>";
  html += "<p class='level " + uvClass + "'>" + uvLevelDescription + "</p>";
  html += "<h2>UV Index Scale</h2>";
  html += "<ul>";
  html += "<li><span class='safe'>0-2: Safe (Low)</span></li>";
  html += "<li><span class='moderate'>3-5: Moderate (Medium)</span></li>";
  html += "<li><span class='harmful'>6-7: Harmful (High)</span></li>";
  html += "<li><span class='very-harmful'>8+: Very Harmful (Extreme)</span></li>";
  html += "</ul>";

  // Add BMP280 data to the HTML
  html += "<h2>Weather Data</h2>";
  html += "<p>Temperature: <strong>" + String(temperature, 1) + " &deg;C</strong></p>";
  html += "<p>Pressure: <strong>" + String(pressure, 1) + " hPa</strong></p>";

  html += "</body></html>";

  // Send HTTP response to client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println(html);
  client.println();

  // Close the connection
  client.stop();
  Serial.println("Client Disconnected");
}

void loop() {
  // Handle incoming client requests
  handleClient();
}
