#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>

// Pin Definitions
const int DHT_PIN = 8;      // DHT11 sensor connected to digital pin 8
const int WATER_PIN = A0;   // Water sensor connected to analog pin A0
const int GAS_PIN = A1;     // MQ-5 gas sensor connected to analog pin A1
const int MQ9_PIN = A2;     // MQ-9 gas sensor connected to analog pin A2
const int MQ3_PIN = A3;     // MQ-3 gas sensor connected to analog pin A3
const int FLAME_PIN = 9;    // KY-026 flame sensor connected to digital pin 9
const int SOIL_PIN = A4;    // Soil moisture sensor connected to analog pin A4
const int TFT_CS = 31;      // TFT LCD CS pin
const int TFT_RST = 32;     // TFT LCD RST pin
const int TFT_DC = 33;      // TFT LCD DC pin
const int TFT_MOSI = 34;    // TFT LCD MOSI pin
const int TFT_CLK = 35;     // TFT LCD SCK pin
const int TFT_MISO = 36;    // TFT LCD MISO pin
// const int RTC_SDA = 20;     // RTC module SDA pin
// const int RTC_SCL = 21;     // RTC module SCL pin

// Constants for DHT sensor
#define DHT_TYPE DHT11   // DHT 11 sensor type

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Initialize TFT display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// // Initialize RTC
// RTC_DS3231 rtc;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("Serial communication started");

  // Initialize DHT sensor
  dht.begin();
  delay(2000); // Delay to allow sensor to stabilize
  Serial.println("DHT sensor initialized");

  // Initialize TFT display
  tft.begin();
  tft.setRotation(1); // Adjust rotation if needed
  tft.fillScreen(ILI9341_BLACK);

  // Display initial messages
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(65, 115);
  tft.println("Weather Station");
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_MAGENTA);
  tft.setTextSize(2);
  tft.setCursor(30, 115);
  tft.println("Created By AgroVoithos");

  Serial.println("Initialization complete");
  delay(3000); // Show initial message for 3 seconds

  // // Initialize RTC
  // Wire.begin();
  // if (!rtc.begin()) {
  //   Serial.println("Couldn't find RTC");
  //   while (1);
  // }

  // // Set initial time and date (if necessary)
  // rtc.adjust(DateTime(2024, 8, 11, 3, 33, 0)); // Year, Month, Day, Hour, Minute, Second

  // Serial.println("RTC initialized");
}

void loop() {
  // Display "Loading Data..." while updating sensor values
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(60, 115); // Center "Loading Data..."
  tft.println("Loading Data...");

  // Read sensor values
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int waterValue = analogRead(WATER_PIN);
  bool waterDetected = (waterValue > 800);
  int gasValue = analogRead(GAS_PIN);
  int mq9Value = analogRead(MQ9_PIN);
  int mq3Value = analogRead(MQ3_PIN);
  bool flameDetected = digitalRead(FLAME_PIN) == LOW;
  int soilValue = analogRead(SOIL_PIN);

  // // Read time from RTC
  // DateTime now = rtc.now();
  // int hour = now.hour();
  // int minute = now.minute();
  // int second = now.second();
  // int day = now.day();
  // int month = now.month();
  // int year = now.year();

  // Display data on TFT LCD
  tft.fillScreen(ILI9341_BLACK);
  
  // Temperature and Humidity Section
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Temperature:");
  tft.setTextSize(1);
  tft.setCursor(10, 30);
  tft.print(temperature);
  tft.println(" C");
  
  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.println("Humidity:");
  tft.setTextSize(1);
  tft.setCursor(10, 70);
  tft.print(humidity);
  tft.println(" %");
  
  // Sensor Status Section
  tft.setTextSize(2);
  tft.setCursor(190, 10);
  tft.println("Water:");
  tft.setTextSize(1);
  tft.setCursor(190, 30);
  tft.println(waterDetected ? "Detected" : "Not Detected");
  
  tft.setTextSize(2);
  tft.setCursor(190, 50);
  tft.println("Methane:");
  tft.setTextSize(1);
  tft.setCursor(190, 70);
  tft.println(gasValue);
  
  tft.setTextSize(2);
  tft.setCursor(190, 90);
  tft.println("CO:");
  tft.setTextSize(1);
  tft.setCursor(190, 110);
  tft.println(mq9Value);
  
  tft.setTextSize(2);
  tft.setCursor(190, 130);
  tft.println("Alcohol:");
  tft.setTextSize(1);
  tft.setCursor(190, 150);
  tft.println(mq3Value);
  
  tft.setTextSize(2);
  tft.setCursor(10, 90);
  tft.println("Flame:");
  tft.setTextSize(1);
  tft.setCursor(10, 110);
  tft.println(flameDetected ? "Detected" : "Not Detected");
  
  tft.setTextSize(2);
  tft.setCursor(10, 130);
  tft.println("Soil Moisture:");
  tft.setTextSize(1);
  tft.setCursor(10, 150);
  tft.println(soilValue);

  // // Display Date and Time
  // tft.setTextSize(1);
  // tft.setCursor(10, 210);
  // tft.print("Date:");
  // tft.print(day);
  // tft.print("/");
  // tft.print(month);
  // tft.print("/");
  // tft.println(year);

  // tft.setCursor(190, 210);
  // tft.print("Time:");
  // tft.print(hour);
  // tft.print(":");
  // if (minute < 10) {
  //   tft.print("0");
  // }
  // tft.print(minute);
  // tft.print(":");
  // if (second < 10) {
  //   tft.print("0");
  // }
  // tft.println(second);

  // Display sensor data on Serial Monitor
  Serial.println("------------------------");
  Serial.println("Temperature: " + String(temperature) + " C");
  Serial.println("Humidity: " + String(humidity) + " %");
  Serial.println("Water Detected: " + String(waterDetected ? "Yes" : "No"));
  Serial.println("Methane: " + String(gasValue));
  Serial.println("Carbon Monoxide: " + String(mq9Value));
  Serial.println("Alcohol Vapor: " + String(mq3Value));
  Serial.println("Flame Detected: " + String(flameDetected ? "Yes" : "No"));
  Serial.println("Soil Moisture: " + String(soilValue));
  // Serial.println("Date: " + String(day) + "/" + String(month) + "/" + String(year));
  // Serial.println("Time: " + String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second));
  Serial.println("------------------------");

  delay(5000); // Update every 5 seconds (adjust as needed)
}
