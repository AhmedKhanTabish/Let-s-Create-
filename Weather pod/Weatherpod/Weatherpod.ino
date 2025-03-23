#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// Create an SSD1306 display object connected to I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WiFi credentials
const char* ssid = "TabishTP-Link_B736";     
const char* password = "Zainabu@01";

// OpenWeatherMap API credentials
const char* api_key = "b9dd54b7c5ed9398dcf5681144f29846";    
const char* city_id = "2844988";    

WiFiClient client;  // WiFi client object for HTTPClient

// NTP Client settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 2 * 3600, 60000); // Adjust timezone offset as necessary

// Bluetooth Serial
BluetoothSerial SerialBT;
const char* bt_password = "Tabish@01"; // Set your desired password
bool authenticated = false; // Track authentication status

// Reminder structure
struct Reminder {
  String time;
  String message;
};

Reminder reminders[5]; // Array to store reminders
int reminderCount = 0;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("TABISH_WeatherPod"); // Bluetooth device name
  
  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize NTP client
  Serial.println("Initializing NTP client...");
  timeClient.begin();
}

void loop() {
  // Update NTP client
  timeClient.update();

  // Get current time
  String formattedTime = getFormattedTime();
  Serial.println("Current time: " + formattedTime);

  // Update weather and display it
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String weather_url = "http://api.openweathermap.org/data/2.5/weather?id=" + String(city_id) + "&appid=" + String(api_key) + "&units=metric";
    http.begin(client, weather_url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Weather API response: " + payload);
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      // Extract temperature and weather description from JSON
      float temperature = doc["main"]["temp"];
      const char* weather_description = doc["weather"][0]["description"];
      String weather = String(weather_description) + " , " + String(temperature) + "C";

      // Display weather and time on OLED
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Bismillah");
      display.print("Weather:");
      display.println(weather);
      display.setCursor(0, 25);
      display.print("City: Rosenheim");
      display.setCursor(0, 40);
      display.println(formattedTime);

      Serial.println("Weather: " + weather);
    } else {
      Serial.println("Error getting weather data, HTTP code: " + String(httpCode));
    }
    http.end(); // Close HTTP connection
  } else {
    Serial.println("WiFi not connected");
  }

  // Display reminders
  display.setCursor(0, 50);
  for (int i = 0; i < reminderCount; i++) {
    display.print("R" + String(i+1) + ": " + reminders[i].message);
    display.setCursor(0, 50 + (i + 1) * 10);
  }

  display.display();

  // Check for Bluetooth data
  if (SerialBT.available()) {
    String received = SerialBT.readStringUntil('\n');
    Serial.println("Received via Bluetooth: " + received);
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
    Serial.println("Reminder added: " + message + " at " + time);
  } else {
    Serial.println("Reminder list full");
  }
}

// Function to delete a reminder
void deleteReminder(int index) {
  if (index < 0 || index >= reminderCount) return;
  Serial.println("Deleting reminder: " + reminders[index].message);
  for (int i = index; i < reminderCount - 1; i++) {
    reminders[i] = reminders[i + 1];
  }
  reminderCount--;
}

void processBluetoothCommand(String command) {
  // Check if the user is authenticated
  if (!authenticated) {
    if (command.equals(bt_password)) {
      authenticated = true;
      Serial.println("Authenticated successfully");
      SerialBT.println("Authenticated successfully");
    } else {
      Serial.println("Authentication failed");
      SerialBT.println("Authentication failed");
    }
    return;
  }

  // Process other commands only if authenticated
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

String getFormattedTime() {
  unsigned long rawTime = timeClient.getEpochTime();
  time_t currentTime = (time_t)rawTime; // Cast to time_t
  struct tm* p_tm = localtime(&currentTime); // Convert to local time

  // Format time as HH:MM:SS
  char timeString[9]; // Buffer for time string
  sprintf(timeString, "%02d:%02d:%02d", p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
  return String(timeString); // Return formatted time as String
}
