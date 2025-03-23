#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <U8g2lib.h>

// WiFi credentials
const char* ssid = "Tabish realme 8 5G";
const char* password = "Zaintab@123";

// OpenWeatherMap API settings
const String apiKey = "b9dd54b7c5ed9398dcf5681144f29846";
const String city = "Rosenheim";
const String countryCode = "DE";
float temperature;
int humidity;
String weatherCondition;

// NTP Client settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 2 * 3600, 60000);

// OLED display object
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 5, /* data=*/ 4);

// Name and Course of Study
const char* name = "Tabish Ahmed Khan";
const char* course = "Electrical&IT Engineer";

// Variables to control display states
int textPosition = 128;
int displayState = 0;
int buttonState = 0;
long lastWeatherUpdate = 0;
const long updateInterval = 600000; // 10 minutes

// Heartbeat sensor (KY-039) settings
const int heartbeatPin = A0; // KY-039 signal pin to analog pin A0
int heartbeat;
unsigned long lastPulseTime = 0;
unsigned long pulseInterval = 0;
int bpm = 0;

void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(10, 0);
  digitalWrite(9, 0);
  u8g2.begin();
  u8g2.enableUTF8Print();

  // Initialize the PRG button (GPIO0) as input with pull-up resistor
  pinMode(0, INPUT_PULLUP);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize NTP client
  Serial.println("Initializing NTP client...");
  timeClient.begin();

  // Fetch the weather data once after setup
  fetchWeatherData();
}

void loop() {
  // Check if the button is pressed
  buttonState = digitalRead(0);
  if (buttonState == LOW) {
    displayState = (displayState + 1) % 4; // Now cycles through 0, 1, 2, and 3
    delay(500);
  }

  // Update NTP client
  timeClient.update();

  // Check if it's time to refresh the weather data
  if (millis() - lastWeatherUpdate > updateInterval) {
    fetchWeatherData();
    lastWeatherUpdate = millis();
  }

  // Clear the display buffer
  u8g2.clearBuffer();

  // Choose what to display based on the current state
  if (displayState == 0) {
    displayNameAndCourse();
  } else if (displayState == 1) {
    displayWeather();
  } else if (displayState == 2) {
    displaySmartwatchTime();
  } else if (displayState == 3) {
    displayHeartbeat();
  }

  // Send the buffer to the display
  u8g2.sendBuffer();
  delay(50);
}

// Function to fetch weather data
void fetchWeatherData() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&units=metric&appid=" + apiKey;
    http.begin(client, url);

    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Weather Data: " + payload);

      // Parse JSON
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      weatherCondition = doc["weather"][0]["main"].as<String>();
      temperature = doc["main"]["temp"].as<float>();
      humidity = doc["main"]["humidity"].as<int>();
    } else {
      Serial.println("Failed to fetch weather data");
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

// Function to display the weather data
void displayWeather() {
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 10);
  u8g2.print("Weather: ");
  u8g2.print(weatherCondition);

  u8g2.setCursor(0, 20);
  u8g2.print("Temp: ");
  u8g2.print(temperature);
  u8g2.print(" C");

  u8g2.setCursor(0, 30);
  u8g2.print("Humidity: ");
  u8g2.print(humidity);
  u8g2.print(" %");
}

// Function to display time in smartwatch style (vertical)
void displaySmartwatchTime() {
  String formattedTime = getFormattedTime();
  u8g2.setFont(u8g2_font_logisoso16_tf);

  u8g2.setCursor(0, 20);
  u8g2.print(formattedTime.substring(0, 2));

  u8g2.setCursor(28, 20);
  u8g2.print(":");

  u8g2.setCursor(38, 20);
  u8g2.print(formattedTime.substring(3, 5));
}

// Function to display name and course of study
void displayNameAndCourse() {
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 15);
  u8g2.print(name);
  u8g2.setCursor(0, 30);
  u8g2.print(course);
}

// Function to get formatted time
String getFormattedTime() {
  unsigned long rawTime = timeClient.getEpochTime();
  time_t currentTime = (time_t)rawTime;
  struct tm* p_tm = localtime(&currentTime);

  char timeString[6];
  sprintf(timeString, "%02d:%02d", p_tm->tm_hour, p_tm->tm_min);
  return String(timeString);
}

// Function to display heartbeat
void displayHeartbeat() {
  u8g2.setFont(u8g2_font_ncenB08_tr);

  // Read the sensor
  int sensorValue = analogRead(heartbeatPin);
  if (sensorValue > 500 && (millis() - lastPulseTime) > 600) {
    pulseInterval = millis() - lastPulseTime;
    lastPulseTime = millis();
    bpm = 60000 / pulseInterval; // Calculate BPM
  }

  // Display BPM
  u8g2.setCursor(0, 30);
  u8g2.print(bpm);
}
