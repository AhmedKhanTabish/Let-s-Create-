#include <Adafruit_NeoPixel.h>

#define PIN            4  // Digital pin connected to the Neopixels
#define NUMPIXELS      7  // Number of Neopixels in your array

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();  // Initialize the Neopixel library
}

void loop() {
  // Define an array of colors (format: RGB)
  uint32_t colors[NUMPIXELS] = {
    // pixels.Color(255, 0, 0),     // Red
    // pixels.Color(255, 165, 0),   // Orange
    // pixels.Color(255, 255, 0),   // Yellow
    // pixels.Color(0, 255, 0),     // Green
    // pixels.Color(0, 0, 255),     // Blue
    // pixels.Color(75, 0, 130),    // Indigo
    pixels.Color(238, 130, 238),  // Violet
    pixels.Color(238, 130, 238),  // Violet
    pixels.Color(238, 130, 238),  // Violet
    pixels.Color(238, 130, 238),  // Violet
    pixels.Color(238, 130, 238),  // Violet
    pixels.Color(238, 130, 238),  // Violet
    pixels.Color(238, 130, 238)  // Violet
  };

  // Display each color on each Neopixel
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, colors[i]);
  }
  pixels.show();  // Display the colors

  delay(10000);  // Pause for a second
}
