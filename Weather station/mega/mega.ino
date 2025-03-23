#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// TFT screen pins
#define TFT_CS  10
#define TFT_RST  8
#define TFT_DC   9

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(9600);

  // Initialize TFT screen
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1); // 0 = portrait, 1 = landscape

  // Fill screen with black
  tft.fillScreen(ST7735_BLACK);

  // Print a message to the screen
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.print("TFT Screen Test");

  // Draw a red rectangle
  tft.fillRect(10, 20, 50, 30, ST7735_RED);

  // Draw a blue circle
  tft.fillCircle(70, 50, 20, ST7735_BLUE);

  // Draw a green line
  tft.drawLine(10, 80, 100, 80, ST7735_GREEN);

  // Print a message to the serial monitor
  Serial.println("TFT screen test complete!");
}

void loop() {
  // Do nothing, just display the test screen
}