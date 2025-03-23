#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define GC9A01A_BLACK       0x0000
#define GC9A01A_NAVY        0x000F
#define GC9A01A_DARKGREEN   0x03E0
#define GC9A01A_DARKCYAN    0x03EF
#define GC9A01A_MAROON      0x7800
#define GC9A01A_PURPLE      0x780F
#define GC9A01A_OLIVE       0x7BE0
#define GC9A01A_LIGHTGREY   0xC618
#define GC9A01A_DARKGREY    0x7BEF
#define GC9A01A_BLUE        0x001F
#define GC9A01A_GREEN       0x07E0
#define GC9A01A_CYAN        0x07FF
#define GC9A01A_RED         0xF800
#define GC9A01A_MAGENTA     0xF81F
#define GC9A01A_YELLOW      0xFFE0
#define GC9A01A_WHITE       0xFFFF
#define GC9A01A_ORANGE      0xFD20
#define GC9A01A_GREENYELLOW 0xAFE5
#define GC9A01A_PINK        0xF81F

// Display setup
#define TFT_CS    D2
#define TFT_RST   D0
#define TFT_DC    D1
#define TFT_BL    -1  // Set to -1 if backlight is connected directly to VCC
Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);

// WiFi settings
const char* ssid = "Tabish realme 8 5G";
const char* password = "Zaintab@123";

// Time setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000); // Update every 60 seconds

void setup() {
  // Initialize display
  tft.begin();
  tft.setRotation(0);  // Set rotation if needed
  tft.fillScreen(GC9A01A_BLACK);

  // Initialize Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Initialize NTP
  timeClient.begin();
}

void drawPlantsAndFlowers() {
  // Draw plants and flowers around the screen, moved closer to the center

  // Left plant (moved right and down)
  tft.fillCircle(40, 40, 10, GC9A01A_GREEN); // Leaf 1
  tft.fillCircle(55, 60, 8, GC9A01A_GREEN);  // Leaf 2
  tft.fillCircle(40, 80, 6, GC9A01A_GREEN);  // Leaf 3
  tft.drawLine(40, 40, 40, 100, GC9A01A_DARKGREEN); // Stem

  // Right plant (moved left and down)
  tft.fillCircle(200, 60, 10, GC9A01A_GREEN); // Leaf 1
  tft.fillCircle(185, 80, 8, GC9A01A_GREEN);  // Leaf 2
  tft.fillCircle(200, 100, 6, GC9A01A_GREEN);  // Leaf 3
  tft.drawLine(200, 60, 200, 120, GC9A01A_DARKGREEN); // Stem

  // Bottom-left plant (moved right and up)
  tft.fillCircle(60, 180, 12, GC9A01A_GREEN); // Leaf 1
  tft.fillCircle(75, 200, 9, GC9A01A_GREEN);  // Leaf 2
  tft.fillCircle(60, 220, 7, GC9A01A_GREEN);  // Leaf 3
  tft.drawLine(60, 180, 60, 240, GC9A01A_DARKGREEN); // Stem

  // Bottom-right plant (moved left and up)
  tft.fillCircle(180, 200, 12, GC9A01A_GREEN); // Leaf 1
  tft.fillCircle(165, 220, 9, GC9A01A_GREEN);  // Leaf 2
  tft.fillCircle(180, 240, 7, GC9A01A_GREEN);  // Leaf 3
  tft.drawLine(180, 200, 180, 260, GC9A01A_DARKGREEN); // Stem

  // Flowers (moved towards center)
  tft.fillCircle(70, 70, 6, GC9A01A_MAGENTA);  // Flower 1
  tft.fillCircle(180, 90, 6, GC9A01A_PINK);    // Flower 2
  tft.fillCircle(80, 210, 6, GC9A01A_ORANGE);  // Flower 3
  tft.fillCircle(160, 230, 6, GC9A01A_MAGENTA); // Flower 4
  tft.fillCircle(140, 40, 8, GC9A01A_PINK);    // Flower 5

  // Flower centers (moved accordingly)
  tft.fillCircle(70, 70, 2, GC9A01A_YELLOW);
  tft.fillCircle(180, 90, 2, GC9A01A_YELLOW);
  tft.fillCircle(80, 210, 2, GC9A01A_YELLOW);
  tft.fillCircle(160, 230, 2, GC9A01A_YELLOW);
  tft.fillCircle(140, 40, 3, GC9A01A_YELLOW);

  // Flower pot (moved up and centered)
  tft.fillRoundRect(90, 190, 60, 30, 5, GC9A01A_MAROON); // Pot body
  tft.fillRect(95, 215, 50, 25, GC9A01A_RED); // Pot base

  // Leaves in pot (moved accordingly)
  tft.fillCircle(105, 185, 10, GC9A01A_GREEN); // Leaf 1
  tft.fillCircle(135, 185, 10, GC9A01A_GREEN); // Leaf 2
  tft.fillCircle(120, 170, 10, GC9A01A_GREEN); // Leaf 3

  // Flowers in pot (moved accordingly)
  tft.fillCircle(105, 170, 5, GC9A01A_ORANGE);  // Flower 1
  tft.fillCircle(135, 170, 5, GC9A01A_MAGENTA); // Flower 2
  tft.fillCircle(120, 160, 5, GC9A01A_PINK);    // Flower 3

  // Flower centers in pot
  tft.fillCircle(105, 170, 2, GC9A01A_YELLOW);  // Flower 1 center
  tft.fillCircle(135, 170, 2, GC9A01A_YELLOW);  // Flower 2 center
  tft.fillCircle(120, 160, 2, GC9A01A_YELLOW);  // Flower 3 center
}

void drawClockFace(int cx, int cy, int radius) {
  // Draw the clock circle
  tft.drawCircle(cx, cy, radius, GC9A01A_WHITE);

  // Draw clock tick marks (12 hours)
  for (int i = 0; i < 12; i++) {
    float angle = i * 30 * PI / 180;
    int x1 = cx + (radius - 10) * cos(angle);
    int y1 = cy + (radius - 10) * sin(angle);
    int x2 = cx + radius * cos(angle);
    int y2 = cy + radius * sin(angle);
    tft.drawLine(x1, y1, x2, y2, GC9A01A_WHITE);
  }
}

void drawClockHands(int cx, int cy, int hour, int minute, int second) {
  // Convert hour to 12-hour format
  hour = hour % 12;

  // Draw hour hand
  float hourAngle = (hour + minute / 60.0) * 30 * PI / 180;
  int hourX = cx + 40 * cos(hourAngle - PI / 2);
  int hourY = cy + 40 * sin(hourAngle - PI / 2);
  tft.drawLine(cx, cy, hourX
