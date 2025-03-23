#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <BluetoothSerial.h>

// OLED display configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi credentials
const char* ssid = "TabishTP-Link_B736";
const char* password = "Zainabu@01";

// Timezone offset in seconds (e.g., 5 hours * 3600 seconds/hour for UTC+5)
const long utcOffsetInSeconds = 2 * 3600;

// NTP client configuration
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, 60000);

// Bluetooth Serial
BluetoothSerial SerialBT;

// Reminder structure
struct Reminder {
  String time;
  String message;
};

Reminder reminders[5]; // Array to store reminders, reduced size for memory optimization
int reminderCount = 0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("TABISH_Calendar"); // Bluetooth device name

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize NTP client
  timeClient.begin();

  // Clear display buffer
  display.clearDisplay();
}

void loop() {
  // Update NTP client
  timeClient.update();

  // Get current time
  String formattedTime = timeClient.getFormattedTime();

  // Display time on OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Time:");
  display.setCursor(0, 20);
  display.print(formattedTime);

  // Display reminders
  display.setTextSize(1);
  display.setCursor(0, 40);
  for (int i = 0; i < reminderCount; i++) {
    display.print("R" + String(i+1) + ": " + reminders[i].message);
    display.setCursor(0, 50 + i*10);
  }

  display.display();

  // Check for Bluetooth data
  if (SerialBT.available()) {
    String received = SerialBT.readStringUntil('\n');
    Serial.println("Received: " + received);
    processBluetoothCommand(received);
  }

  // Check for reminders to delete
  checkReminders(formattedTime);

  delay(1000);
}

// Function to add a reminder
void addReminder(String time, String message) {
  if (reminderCount < 5) {
    reminders[reminderCount].time = time;
    reminders[reminderCount].message = message;
    reminderCount++;
  } else {
    Serial.println("Reminder list full");
  }
}

// Function to delete a reminder
void deleteReminder(int index) {
  if (index < 0 || index >= reminderCount) return;
  for (int i = index; i < reminderCount - 1; i++) {
    reminders[i] = reminders[i + 1];
  }
  reminderCount--;
}

void processBluetoothCommand(String command) {
  // Expected format: "REMINDER:hh:mm:ss:message"
  if (command.startsWith("REMINDER:")) {
    String time = command.substring(9, 17);
    String message = command.substring(18);
    addReminder(time, message);
    Serial.println("Reminder added: " + message);
  }
}

void checkReminders(String currentTime) {
  for (int i = 0; i < reminderCount; i++) {
    if (currentTime >= reminders[i].time) {
      Serial.println("Deleting reminder: " + reminders[i].message);
      deleteReminder(i);
      i--; // Adjust index after deletion
    }
  }
}
