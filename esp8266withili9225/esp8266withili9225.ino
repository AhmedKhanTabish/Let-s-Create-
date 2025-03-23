#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "SPI.h"
#include "TFT_22_ILI9225.h"
#include <../fonts/FreeSans12pt7b.h>
#include <../fonts/FreeSans24pt7b.h>

// WiFi credentials
const char* ssid = "Tabish realme 8 5G";
const char* password = "Zaintab@123";

// Time settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // Update every 60 seconds

// TFT display pins
#define TFT_RST 4   // D2
#define TFT_RS  5   // D1
#define TFT_CLK 14  // D5 SCK
#define TFT_SDI 13  // D7 MOSI
#define TFT_CS  15  // D8 SS
#define TFT_LED 2   // D4

#define TFT_BRIGHTNESS 200 // Initial brightness of TFT backlight

// Initialize TFT
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);

int16_t x=0, y=0, w, h;

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void drawHeart(int16_t x, int16_t y, uint16_t color) {
  tft.drawCircle(x - 5, y, 5, color);
  tft.drawCircle(x + 5, y, 5, color);
  tft.fillTriangle(x - 10, y, x + 10, y, x, y + 15, color);
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
  timeClient.begin();

  tft.begin();
}

void loop() {
  tft.clear(); // Clear the screen at the start of each loop

  // Draw multiple hearts at different positions
  drawHeart(30, 50, COLOR_RED);
  drawHeart(60, 50, COLOR_RED);
  drawHeart(90, 50, COLOR_RED);
  drawHeart(120, 50, COLOR_RED);
  drawHeart(150, 50, COLOR_RED);

  // Display the time
  timeClient.update();
  String s1 = "Time:";
  tft.setGFXFont(&FreeSans24pt7b);
  tft.getGFXTextExtent(s1, x, y, &w, &h);
  y = h + 20; // Set y position to string height + 20 pixels down
  tft.drawGFXText(x, y, s1, COLOR_BLUE);

  // Get and display formatted time
  String formattedTime = timeClient.getFormattedTime().substring(0, 5); // HH:MM
  y += h + 10; // Set y position to string height plus shift down 10 pixels
  tft.getGFXTextExtent(formattedTime, x, y, &w, &h);
  tft.drawGFXText(x + 10, y, formattedTime, COLOR_GREEN);

  delay(60000); // Update every minute
}

