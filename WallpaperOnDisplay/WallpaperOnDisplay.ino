#include "SPI.h"
#include "TFT_22_ILI9225.h"

#define TFT_RST 9
#define TFT_RS  8
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 0   // 0 if wired to +5V directly

#define TFT_BRIGHTNESS 200 // Initial brightness of TFT backlight (optional)

TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED, TFT_BRIGHTNESS);

// Include the C array from tux.c
extern const uint8_t tux_map[];
extern const int tux_width;
extern const int tux_height;

void setup() {
  tft.begin();
  Serial.begin(9600);
  
  tft.setOrientation(0);
  tft.clear();
  
  // Display the image from the C array
  displayImageFromArray();
}

void loop() {
  // Nothing here for a one-time display
}

void displayImageFromArray() {
  int imageWidth = tux_width;
  int imageHeight = tux_height;

  for (int y = 0; y < imageHeight; y++) {
    for (int x = 0; x < imageWidth; x++) {
      // Extract RGB565 color from tux_map array
      int index = (y * imageWidth + x) * 2;
      uint16_t color = tux_map[index] << 8 | tux_map[index + 1];
      tft.drawPixel(x, y, color);
    }
  }
}
