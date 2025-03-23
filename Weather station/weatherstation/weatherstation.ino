#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>

// Define pins for sensors and modules
#define DHTPIN 8
#define DHTTYPE DHT22
#define WATER_SENSOR_PIN A0
#define GAS_SENSOR_PIN A1

// #define TFT_CS A2
// #define TFT_RST A3
// #define TFT_DC A4
// #define TFT_MOSI A5
// #define TFT_SCK A6
// #define TFT_BACKLIGHT A7

DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);
Adafruit_ST7735 tft = Adafruit_ST7735(A2, A4, A5, A6, A3);
void setup() {
  Serial.begin(9600);

  // Initialize pin A7 as an output and set it high
  pinMode(A7, OUTPUT);
  digitalWrite(A7, HIGH);

  // Initialize DHT22 sensor
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

  // Initialize TFT screen
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);

}

void loop() {
  // Read sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int waterLevel = analogRead(WATER_SENSOR_PIN);
  int gasLevel = analogRead(GAS_SENSOR_PIN);

  DateTime now = rtc.now();
  String dateTime = now.timestamp();

  // Display data on TFT screen
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.println("Weather Station");
  tft.setTextSize(1);
  tft.print("Temp: ");
  tft.print(temperature);
  tft.println(" C");

  tft.print("Humidity: ");
  tft.print(humidity);
  tft.println(" %");

  tft.print("Water Level: ");
  tft.print(waterLevel);
  tft.println();

  tft.print("Gas Level: ");
  tft.print(gasLevel);
  tft.println();

  tft.print("Time: ");
  tft.println(dateTime);

  delay(10000); // Delay between readings
}
