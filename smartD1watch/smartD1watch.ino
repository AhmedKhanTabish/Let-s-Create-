#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <Fonts/FreeMono9pt7b.h>  // Add a smaller font

// Wi-Fi credentials
const char* ssid = "Tabish realme 8 5G";
const char* password = "Zaintab@123";

// NTP server details
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);  // Adjust time offset (e.g., 3600 for GMT+1)

// OLED display parameters
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 48
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) ; // Don't proceed, loop forever
  }

  // Clear the display
  display.clearDisplay();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");

  // Initialize the NTP client
  timeClient.begin();
  
  // Set the custom font
  display.setFont(&FreeMono9pt7b);  // Use the smaller font
}

void loop() {
  timeClient.update(); // Get the latest time from NTP

  // Format the date and time
  String formattedTime = timeClient.getFormattedTime();

  // Clear the previous display content
  display.clearDisplay();
  
  // Set text color
  display.setTextColor(SSD1306_WHITE);

  // Calculate the width of the formatted time string to center it
  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  display.getTextBounds(formattedTime, 0, 0, &x1, &y1, &textWidth, &textHeight);

  // Set the cursor to horizontally center the text
  int16_t xPos = (SCREEN_WIDTH - textWidth) / 2;
  display.setCursor(xPos, 25); // Adjust vertical position as needed
  
  // Display time
  display.println(formattedTime);  // Display time as HH:MM:SS
  
  // Show the display buffer on the screen
  display.display();

  // Wait for a second before updating the time
  delay(1000);
}
