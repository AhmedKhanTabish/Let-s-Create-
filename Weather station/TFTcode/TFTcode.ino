#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <TFT_eSPI.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define SD_CS 10
#define TFT_CS 9
#define TFT_RST 8
#define TFT_RS 7

DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;  // Change to RTC_DS3231 for DS3231SN
TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT_eSPI class

void setup() {
  Serial.begin(9600);
  dht.begin();

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed, or not present");
    while (1);
  }

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  DateTime now = rtc.now();

  logDataToSD(now, temperature, humidity);
  displayDataOnTFT(temperature, humidity);

  delay(2000);
}

void logDataToSD(DateTime now, float temperature, float humidity) {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(now.year(), DEC);
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/');
    dataFile.print(now.day(), DEC);
    dataFile.print(' ');
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.print(", ");
    dataFile.print(temperature);
    dataFile.print(", ");
    dataFile.println(humidity);
    dataFile.close();
  } else {
    Serial.println("Error opening datalog.txt");
  }
}

void displayDataOnTFT(float temperature, float humidity) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.print("Temp: ");
  tft.print(temperature);
  tft.println(" C");
  tft.print("Hum: ");
  tft.print(humidity);
  tft.println(" %");
}
