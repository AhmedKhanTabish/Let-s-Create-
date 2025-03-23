#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <GxEPD2_BW.h>

// Define DHT sensor
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Define Water Sensor
#define WATERSENSORPIN 3

// Define MQ-5 Sensor
#define MQ5PIN A0

// Define RTC
RTC_DS1307 rtc;

// Define SD card
#define CSPIN 4

// E-Paper display setup
GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(SS, 8, 9, 7));

void setup() {
  Serial.begin(9600);

  // Initialize DHT sensor
  dht.begin();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Initialize SD card
  if (!SD.begin(CSPIN)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  
  // Initialize E-Paper display
  display.init();
  
  // Initialize Water Sensor
  pinMode(WATERSENSORPIN, INPUT);

  // Display setup message
  display.clearScreen();
  display.setCursor(0, 0);
  display.print(F("Weather Station Setup Complete"));
}

void loop() {
  // Read sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  bool waterDetected = digitalRead(WATERSENSORPIN);
  int gasLevel = analogRead(MQ5PIN);
  DateTime now = rtc.now();
  
  // Display data on E-Paper
  display.clearScreen();
  display.setCursor(0, 0);
  display.print(F("Temp: ")); display.println(temperature, 1); display.print(F(" C"));
  display.print(F("Humidity: ")); display.println(humidity, 1); display.print(F(" %"));
  display.print(F("Water: ")); display.println(waterDetected ? F("Yes") : F("No"));
  display.print(F("Gas Level: ")); display.println(gasLevel);
  display.print(F("Time: "));
  display.print(now.hour()); display.print(F(":"));
  display.print(now.minute()); display.print(F(":"));
  display.println(now.second());
  display.refresh();
  
  // Log data to SD card
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(now.timestamp());
    dataFile.print(F(", "));
    dataFile.print(temperature);
    dataFile.print(F(", "));
    dataFile.print(humidity);
    dataFile.print(F(", "));
    dataFile.print(waterDetected);
    dataFile.print(F(", "));
    dataFile.println(gasLevel);
    dataFile.close();
  } else {
    Serial.println(F("Error opening datalog.txt"));
  }
  
  delay(60000); // Log data every minute
}
