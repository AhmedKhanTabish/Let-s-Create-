#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// NTP Client Setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);  // Sync every minute

// Display setup
#define TFT_CS    D2
#define TFT_RST   D0
#define TFT_DC    D1
#define TFT_BL    -1  // Set to -1 if backlight is connected directly to VCC
Adafruit_GC9A01A tft = Adafruit_GC9A01A(TFT_CS, TFT_DC, TFT_RST);

// Custom colors for the display
#define GC9A01A_BLACK       0x0000
#define GC9A01A_NAVY        0x000F
#define GC9A01A_LIGHTGREY   0xC618
#define GC9A01A_DARKGREY    0x7BEF
#define GC9A01A_BLUE        0x001F
#define GC9A01A_WHITE       0xFFFF

// Example C array for a globe image (Replace this with your converted image array)
const uint16_t globe_image[] PROGMEM = {
  // Add the C array content generated from the image
};


// When accessing the array, use pgm_read_word to read from PROGMEM
void drawGlobe() {
  for (int16_t y = 0; y < 240; y++) {  // Use the correct image height
    for (int16_t x = 0; x < 240; x++) {  // Use the correct image width
      tft.drawPixel(x, y, pgm_read_word(&globe_image[y * 100 + x]));
    }
  }
}

// Function to draw the rotating dials with 12-hour format and 5-minute intervals
void drawDials(int hour, int minute, int second) {
  // Outer dial for hours (12 segments)
  for (int i = 0; i < 12; i++) {
    float angle = (i * PI / 6);  // Calculate angle for 12-hour segments
    int x = 120 + cos(angle) * 100;  // X coordinate for hour marker
    int y = 120 + sin(angle) * 100;  // Y coordinate for hour marker
    
    // Highlight the current hour
    if (i == hour % 12) {
      tft.fillCircle(x, y, 5, GC9A01A_WHITE);  // Highlighted marker
    } else {
      tft.fillCircle(x, y, 3, GC9A01A_DARKGREY);  // Regular marker
    }
  }

  // Inner dial for minutes (12 segments, each representing 5 minutes)
  for (int i = 0; i < 12; i++) {
    float angle = (i * PI / 6);  // Calculate angle for 12 segments (5 minutes each)
    int x = 120 + cos(angle) * 70;  // X coordinate for minute marker
    int y = 120 + sin(angle) * 70;  // Y coordinate for minute marker
    
    // Highlight the current minute segment
    if (i == minute / 5) {
      tft.fillCircle(x, y, 5, GC9A01A_LIGHTGREY);  // Highlighted marker
    } else {
      tft.fillCircle(x, y, 3, GC9A01A_DARKGREY);  // Regular marker
    }
  }
}


// Setup WiFi and display
void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Initialize the display
  tft.begin();
  tft.setRotation(3);  // Set display orientation
  tft.fillScreen(GC9A01A_BLACK);  // Clear screen

  // WiFi settings
  const char* ssid = "Tabish realme 8 5G";
  const char* password = "Zaintab@123";

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize the NTP client
  timeClient.begin();
}

// Main loop to display the time
void loop() {
  // Sync time with NTP
  timeClient.update();
  
  // Get the current date from NTP
  time_t rawTime = timeClient.getEpochTime();
  struct tm * timeInfo = localtime(&rawTime);

  // Extract time components
  int hour = timeInfo->tm_hour;
  int minute = timeInfo->tm_min;
  int second = timeInfo->tm_sec;

  // Clear screen, draw globe, and dials
  tft.fillScreen(GC9A01A_BLACK);  // Clear the screen
  drawGlobe();  // Draw the globe at the center
  drawDials(hour, minute, second);  // Draw the rotating dials

  // Delay for 1 second before updating again
  delay(60000);
}
