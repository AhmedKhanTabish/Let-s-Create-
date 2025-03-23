#include <TFT_eSPI.h>

#define TFT_CS 9
#define TFT_RST 8
#define TFT_RS 7

TFT_eSPI tft = TFT_eSPI();  // Create an instance of the TFT_eSPI class

void setup() {
  Serial.begin(9600);
  
  // Initialize TFT screen
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK); // Try filling with black initially
  
  Serial.println("Setup complete");
}

void loop() {
  Serial.println("Loop iteration");
  
  // Display something on TFT screen
  tft.fillScreen(TFT_BLACK); // Try filling with a color to verify display
  
  delay(1000);
}

