/**
 *  @filename   :   epd4in2-demo.ino
 *  @brief      :   4.2inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 4 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SPI.h>
#include "epd4in2.h"
#include "imagedata.h"
#include "epdpaint.h"
#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_EPD.h>

#define COLORED     0
#define UNCOLORED   1

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Define Water Sensor
#define WATERSENSORPIN 3

// Define MQ-5 Sensor
#define MQ5PIN A0

// Define RTC
RTC_DS1307 rtc;

// // Define SD card
// #define CSPIN 4

// E-Paper display setup
#define EPD_CS     10
#define EPD_DC     9
#define EPD_RESET  8
#define EPD_BUSY   7
Adafruit_IL91874 display(176, 264, EPD_DC, EPD_RESET, EPD_CS, EPD_BUSY);

// Data logging file
File dataFile;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Epd epd;
  Serial.begin(9600);

  // Initialize DHT sensor
  dht.begin();

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // // Initialize SD card
  // if (!SD.begin(CSPIN)) {
  //   Serial.println("SD card initialization failed!");
  //   return;
  // }
  
  // Initialize E-Paper display
  display.begin();
  display.setRotation(1);
  display.setTextWrap(false);
  
  // Initialize Water Sensor
  pinMode(WATERSENSORPIN, INPUT);

  // Display setup message
  display.fillScreen(EPD_WHITE);
  display.setCursor(0, 0);
  display.setTextColor(EPD_BLACK);
  display.setTextSize(1);
  display.print(F("Weather Station Setup Complete"));
  display.display();
  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }
	//Serial.print(UNCOLORED);
  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to 
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
  // unsigned char image[1500];
  // Paint paint(image, 400, 28);    //width should be the multiple of 8 

  // paint.Clear(UNCOLORED);
  // paint.DrawStringAt(0, 0, "e-Paper Demo", &Font24, COLORED);
  // epd.SetPartialWindow(paint.GetImage(), 100, 40, paint.GetWidth(), paint.GetHeight());

  // paint.Clear(COLORED);
  // paint.DrawStringAt(100, 2, "Hello world", &Font24, UNCOLORED);
  // epd.SetPartialWindow(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());
  
  // paint.SetWidth(64);
  // paint.SetHeight(64);

  // paint.Clear(UNCOLORED);
  // paint.DrawRectangle(0, 0, 40, 50, COLORED);
  // paint.DrawLine(0, 0, 40, 50, COLORED);
  // paint.DrawLine(40, 0, 0, 50, COLORED);
  // epd.SetPartialWindow(paint.GetImage(), 72, 120, paint.GetWidth(), paint.GetHeight());
  
  // paint.Clear(UNCOLORED);
  // paint.DrawCircle(32, 32, 30, COLORED);
  // epd.SetPartialWindow(paint.GetImage(), 200, 120, paint.GetWidth(), paint.GetHeight());

  // paint.Clear(UNCOLORED);
  // paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  // epd.SetPartialWindow(paint.GetImage(), 72, 200, paint.GetWidth(), paint.GetHeight());

  // paint.Clear(UNCOLORED);
  // paint.DrawFilledCircle(32, 32, 30, COLORED);
  // epd.SetPartialWindow(paint.GetImage(), 200, 200, paint.GetWidth(), paint.GetHeight());

  /* This displays the data from the SRAM in e-Paper module */
  epd.DisplayFrame();

  /* This displays an image */
  //epd.DisplayFrame(IMAGE_BUTTERFLY);

  
  
  // epd.Init_4Gray();
  // epd.ClearFrame();
  // epd.Set_4GrayDisplay(gImage_4in2_4Gray1, 100, 100,  200,150);
  // delay(2000);

  /* Deep sleep */
  epd.ClearFrame();
  epd.DisplayFrame();
  epd.Sleep();
}

void loop() {
  // Read sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  bool waterDetected = digitalRead(WATERSENSORPIN);
  int gasLevel = analogRead(MQ5PIN);
  DateTime now = rtc.now();
  
  // Display data on E-Paper
  display.fillScreen(EPD_WHITE);
  display.setCursor(0, 0);
  display.setTextColor(EPD_BLACK);
  display.setTextSize(1);
  display.print(F("Temp: ")); display.print(temperature, 1); display.println(F(" C"));
  display.print(F("Humidity: ")); display.print(humidity, 1); display.println(F(" %"));
  display.print(F("Water: ")); display.println(waterDetected ? F("Yes") : F("No"));
  display.print(F("Gas Level: ")); display.println(gasLevel);
  display.print(F("Time: "));
  display.print(now.hour()); display.print(F(":"));
  display.print(now.minute()); display.print(F(":"));
  display.println(now.second());
  display.display();
  
  // // Log data to SD card
  // dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if (dataFile) {
  //   dataFile.print(now.timestamp());
  //   dataFile.print(F(", "));
  //   dataFile.print(temperature);
  //   dataFile.print(F(", "));
  //   dataFile.print(humidity);
  //   dataFile.print(F(", "));
  //   dataFile.print(waterDetected);
  //   dataFile.print(F(", "));
  //   dataFile.println(gasLevel);
  //   dataFile.close();
  // } else {
  //   Serial.println(F("Error opening datalog.txt"));
  // }
  
  delay(60000);
}
