/************************************************************************************
   GxEPD_SPI_TestExample : test example for e-Paper displays from Dalian Good Display Co., Ltd.: www.good-display.com

   based on Demo Example from Good Display, now available on http://www.good-display.com/download_list/downloadcategoryid=34&isMode=false.html

   Author : J-M Zingg

   modified by :

   Version : 2.0

   Support: limited, provided as example, no claim to be fit for serious use

   connection to the e-Paper display is through DESTM32-S2 connection board, available from Good Display

   DESTM32-S2 pinout (top, component side view):
       |-------------------------------------------------
       |  VCC  |o o| VCC 5V, not needed
       |  GND  |o o| GND
       |  3.3  |o o| 3.3V
       |  nc   |o o| nc
       |  nc   |o o| nc
       |  nc   |o o| nc
       |  MOSI |o o| CLK=SCK
       | SS=DC |o o| D/C=RS    // Slave Select = Device Connect |o o| Data/Command = Register Select
       |  RST  |o o| BUSY
       |  nc   |o o| BS, connect to GND
       |-------------------------------------------------
*/

const int SLEEP_LENGTH = 200;
// //50x50 pixels
// #define yt3_width 100
// #define yt3_height 61
// static unsigned char youtube[112] = { /* 0X00,0X01,0X20,0X00,0X1C,0X00, */

// };
// include library, include base class, make path known

// Include required libraries
#include <FS.h>
#include <GxEPD.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoJson.h> // Include the ArduinoJson library for parsing JSON

#define min(a,b) ((a)<(b)?(a):(b))

// Select the display class to use
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

// Define constants for ESP8266
#if defined(ESP8266)
GxIO_Class io(SPI, SS, D3, D4); // Configure your SPI pins as necessary
GxEPD_Class display(io);
#endif

// WiFi credentials
const char* ssid = "TabishTP-Link_B736";     
const char* password = "Zainabu@01";

// OpenWeatherMap API credentials
const char* api_key = "b9dd54b7c5ed9398dcf5681144f29846";    
const char* city_id = "2844988";

// Weather API URL
const char* weather_url = "http://api.openweathermap.org/data/2.5/weather?id=";

String cityName;
String temperature;
String windSpeed;
String currentTime;

void setup() {
    // Initialize Serial for debugging
    Serial.begin(115200);

    // Initialize display
    display.init();
    display.setRotation(0);

    // Initialize WiFi
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    wifiManager.autoConnect(ssid, password);

    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Failed to connect to WiFi");
        return;
    }
}

void loop() {
    // Request weather data
    WiFiClient client;
    if (!client.connect(weather_url, 80)) {
        Serial.println("Connection to weather API failed");
        return;
    }

    String url = String(weather_url) + city_id + "&appid=" + api_key + "&units=metric";
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: api.openweathermap.org\r\n" + 
                 "Connection: close\r\n\r\n");

    // Wait for the response
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 10000) {
            Serial.println("Client Timeout !");
            client.stop();
            return;
        }
    }

    // Parse JSON response
    String jsonResponse;
    while (client.available()) {
        jsonResponse += client.readString();
    }

    DynamicJsonDocument doc(1024);
    deserializeJson(doc, jsonResponse);
    cityName = doc["name"].as<String>();
    temperature = String(doc["main"]["temp"].as<float>()) + " °C";
    windSpeed = String(doc["wind"]["speed"].as<float>()) + " m/s";

    // Get the current time
    time_t now;
    struct tm* timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    currentTime = asctime(timeinfo);

    // Update the display
    showWeather();

    // Sleep for 60 minutes
    ESP.deepSleep(60 * 60 * 1000000); // Sleep for 60 minutes
}

void showWeather() {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeSans9pt7b);

    // Display City Name and Time
    display.setCursor(5, 20);
    display.println("City: " + cityName);
    display.setCursor(5, 40);
    display.println("Time: " + currentTime);

    // Display Weather Information
    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(5, 80);
    display.println("Temp: " + temperature);
    display.setCursor(5, 120);
    display.println("Wind: " + windSpeed);

    display.update();
}
