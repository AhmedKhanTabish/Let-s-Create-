
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WiFi credentials
const char* ssid = "TabishTP-Link_B736";
const char* password = "Zainabu@01";

// OLED display settings
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// Create an SSD1306 display object connected to I2C (SDA, SCL)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Set text size and color
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Display a splash screen message
  display.setCursor(0, 10);
  display.println(F("Connecting"));
  display.display();

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(F("."));
  }
  Serial.println("");
  display.println(F("WiFi connected"));
  display.println(F("IP address: "));
  display.println(WiFi.localIP());
  display.println(F("Hello, World!"));

  // Clear the display buffer
  display.clearDisplay();
  
  // Display "Hello, World!" message
  // display.setCursor(0, 10);
  
  // display.display();
}

void loop() {
  // Nothing needed here
}
