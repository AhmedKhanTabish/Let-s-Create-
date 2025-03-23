#include <TFT_22_ILI9225.h> // Ensure this matches the library you're using
#include <SPI.h>           // Ensure SPI is included if needed
// Define pins according to your setup
#define TFT_RST   8   // You can use any GPIO pin for reset
#define TFT_RS    9   // Any GPIO pin for data/command
#define TFT_CS    10  // Hardware SPI chip select pin (use D10)
#define TFT_CLK   13  // Hardware SPI clock pin (SCK, D13 on Nano)
#define TFT_SDI   11  // Hardware SPI data in (MOSI, D11 on Nano)
#define BUTTON_PIN 3  // Update to a free GPIO pin for your button
// Create TFT object
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK);
// Define pages of text
const char* pages[] = {
"The Midnight Raid\n\n"
  "Page 1\n\n"
  "The ship \"Sea Serpent\" drifted under the moonlit sky. Ahmed, a young teen with curly hair and a hat, tinkered with a compass. Nahla, with long blonde hair wrapped in a scarf, gazed at the horizon. The peaceful night shattered with the lookout’s cry: “Pirates!”",

  "Page 2\n\n"
  "Pirates in black kaftans emerged from the mist. Their leader, Captain Grim, roared, “Surrender your treasure!” The crew scattered. Ahmed and Nahla, exchanging worried glances, took cover behind crates.",

  "Page 3\n\n"
  "Nahla created a distraction by tossing barrels overboard. Ahmed slipped below deck and retrieved weapons from the armory. Armed, they regrouped on deck, ready to defend their ship.",

  "Page 4\n\n"
  "Ahmed and Nahla fought bravely. Ahmed’s sword clashed with pirates, while Nahla’s agility kept them at bay. They advanced towards Captain Grim. With a precise strike, Nahla injured the captain.",

  "Page 5\n\n"
  "The pirates, leader defeated, surrendered. The \"Sea Serpent\" was safe. Ahmed and Nahla stood together, proud and relieved. As the sun rose, they sailed onward, ready for new adventures."
};
const int numPages = sizeof(pages) / sizeof(pages[0]);
int currentPage = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Button pin with internal pull-up resistor
  
  tft.begin();
  tft.setOrientation(1); // Landscape mode
  tft.clear(); // Clear screen
  
  // Display the first page
  displayPage(currentPage);
}
void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) { // Button pressed
    delay(200); // Debounce delay
    currentPage = (currentPage + 1) % numPages; // Cycle through pages
    displayPage(currentPage);
  }
}
void displayPage(int page) {
  if (page < 0 || page >= numPages) return;
  tft.clear(); // Clear screen
  tft.setColor(255, 255, 255);     // Set text color to white (RGB: 255, 255, 255)
  tft.drawText(10, 20, pages[page]); // Display page content at position (10, 20)
}
