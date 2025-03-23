#include <Arduino_GFX_Library.h>

// Define the number of clocks
#define NUM_CLOCKS 4

// Define the number of watch faces
#define NUM_WATCH_FACES 4

// Define the number of cities for world clocks
#define NUM_CITIES 4

// Define the cities and their time zones
const char *cities[NUM_CITIES] = {"New York", "London", "Paris", "Tokyo"};
const int timeZones[NUM_CITIES] = {-5, 0, 1, 9};

// Define the watch faces
const uint8_t watchFaces[NUM_WATCH_FACES][10] = {
  {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09},
  {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19},
  {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29},
  {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}
};

// Create an array of clock objects
Clock clocks[NUM_CLOCKS];

// Initialize the display
Arduino_GC9A01 gfx = Arduino_GC9A01(7 /* RST */, 0 /* rotation */, true /* IPS */);

void setup() {
  // Initialize the display
  gfx.begin();
  gfx.fillScreen(BLACK);

  // Initialize the serial monitor
  Serial.begin(9600);

  // Set the time for each clock
  for (int i = 0; i < NUM_CLOCKS; i++) {
    clocks[i].setTime(i + 1, i * 15, i * 30);
    clocks[i].setWatchFace(i % NUM_WATCH_FACES);
    clocks[i].setCity(i % NUM_CITIES);
  }
}

void loop() {
  // Clear the display
  gfx.fillScreen(BLACK);

  // Update the time for each clock
  for (int i = 0; i < NUM_CLOCKS; i++) {
    int hours = clocks[i].getHours();
    int minutes = clocks[i].getMinutes();
    int seconds = clocks[i].getSeconds();

    // Update the watch face
    int watchFace = clocks[i].getWatchFace();

    // Update the city
    int cityIndex = clocks[i].getCity();

    // Draw the clock
    drawClock(gfx, i * 80 + 40, 40, 40, watchFaces[watchFace], cities[cityIndex], timeZones[cityIndex], hours, minutes, seconds);
  }

  // Wait for a second
  delay(1000);
}

void drawClock(Arduino_GC9A01& gfx, int x, int y, int r, const uint8_t *watchFace, const char *city, int timeZone, int hours, int minutes, int seconds) {
  // Draw the watch face
  gfx.drawBitmap(x, y, 80, 80, watchFace);

  // Draw the hour hand
  int hourHandLength = r * 3 / 4;
  int hourHandX = x + r / 2 + hourHandLength * cos(TWO_PI * (hours + minutes / 60.0) / 12.0);
  int hourHandY = y + r / 2 + hourHandLength * sin(TWO_PI * (hours + minutes / 60.0) / 12.0);
  gfx.drawLine(x + r / 2, y + r / 2, hourHandX, hourHandY, WHITE);

  // Draw the minute hand
  int minuteHandLength = r * 2 / 3;
  int minuteHandX = x + r / 2 + minuteHandLength * cos(TWO_PI * (minutes + seconds / 60.0) / 60.0);
  int minuteHandY = y + r / 2 + minuteHandLength * sin(TWO_PI * (minutes + seconds / 60.0) / 60.0);
  gfx.drawLine(x + r / 2, y + r / 2, minuteHandX, minuteHandY, WHITE);

  // Draw the second hand
  int secondHandLength = r - 4;
  int secondHandX = x + r / 2 + secondHandLength * cos(TWO_PI * seconds / 60.0);
  int secondHandY = y + r / 2 + secondHandLength * sin(TWO_PI * seconds / 60.0);
  gfx.drawLine(x + r / 2, y + r / 2, secondHandX, secondHandY, WHITE);

  // Draw the time zone text
  gfx.setTextColor(WHITE);
  gfx.setCursor(x + r / 2 - gfx.textWidth(city) / 2, y + r + 4);
  gfx.print(city);
  gfx.print(" (UTC ");
  gfx.print(timeZone);
  gfx.print(")");
}
