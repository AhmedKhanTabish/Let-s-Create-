#include <time.h> // Include time library
#include "LCD_Test.h"
#include "ImageData.h" // Include your bitmap image data here

UWORD *BlackImage; // Declare BlackImage globally

void DrawBitmap(const unsigned char *image) {
  for (int y = 0; y < LCD_1IN28_HEIGHT; y++) {
    for (int x = 0; x < LCD_1IN28_WIDTH; x++) {
      // Read the pixel color data
      uint16_t color = image[(y * LCD_1IN28_WIDTH + x) * 2] | (image[(y * LCD_1IN28_WIDTH + x) * 2 + 1] << 8);
      
      // Extract RGB components (assuming RGB565 format)
      uint8_t r = (color >> 11) & 0x1F;
      uint8_t g = (color >> 5) & 0x3F;
      uint8_t b = color & 0x1F;
      
      // Swap red and blue components to convert from RGB to BGR
      uint16_t correctedColor = (b << 11) | (g << 5) | r;
      
      // Draw the corrected color pixel on the display
      Paint_DrawPoint(x, y, correctedColor, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
    }
  }
}

void SetInitialTime(int year, int month, int day, int hour, int minute, int second) {
  struct tm newTime;
  newTime.tm_year = year - 1900; // tm_year is years since 1900
  newTime.tm_mon = month - 1;    // tm_mon is months since January (0-11)
  newTime.tm_mday = day;         // Day of the month (1-31)
  newTime.tm_hour = hour;        // Hours since midnight (0-23)
  newTime.tm_min = minute;       // Minutes after the hour (0-59)
  newTime.tm_sec = second;       // Seconds after the minute (0-59)
  newTime.tm_isdst = -1;         // Not setting daylight saving time info

  // Convert to time_t
  time_t initialTime = mktime(&newTime);

  // Set system time
  struct timeval tv;
  tv.tv_sec = initialTime;
  tv.tv_usec = 0; // No microseconds
  settimeofday(&tv, NULL);
}

void setup() {
  // put your setup code here, to run once:
  if(DEV_Module_Init() != 0)
    Serial.println("GPIO Init Fail!");
  else
    Serial.println("GPIO Init successful!");

  LCD_1IN28_Init(HORIZONTAL);
  DEV_SET_PWM(50);
  LCD_1IN28_Clear(WHITE);
  UDOUBLE Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
  
  BlackImage = (UWORD *)malloc(Imagesize); // Allocate memory globally
  if (BlackImage == NULL)
  {
      Serial.println("Failed to apply for black memory...");
      exit(0);
  }
  
  // Create a new image cache named IMAGE_RGB and fill it with white
  Paint_NewImage((UBYTE *)BlackImage, LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
  Paint_SetScale(65);
  Paint_Clear(WHITE);
  Paint_SetRotate(ROTATE_0);
  Paint_Clear(WHITE);

  // Load and draw the bitmap image as the background with color correction
  DrawBitmap(gImage_background);

  Paint_DrawPoint(50, 41, BLACK, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP); // 240 240
  Paint_DrawPoint(50, 46, BLACK, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
  Paint_DrawPoint(50, 51, BLACK, DOT_PIXEL_3X3, DOT_FILL_RIGHTUP);
  Paint_DrawPoint(50, 56, BLACK, DOT_PIXEL_4X4, DOT_FILL_RIGHTUP);
  Paint_DrawPoint(50, 61, BLACK, DOT_PIXEL_5X5, DOT_FILL_RIGHTUP);

  Paint_DrawLine(60, 40, 90, 70, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
  Paint_DrawLine(60, 70, 90, 40, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);

  Paint_DrawRectangle(60, 40, 90, 70, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(100, 40, 130, 70, BLUE, DOT_PIXEL_2X2, DRAW_FILL_FULL);

  Paint_DrawLine(135, 55, 165, 55, CYAN, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
  Paint_DrawLine(150, 40, 150, 70, CYAN, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);

  Paint_DrawCircle(150, 55, 15, GREEN, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawCircle(185, 55, 15, GREEN, DOT_PIXEL_1X1, DRAW_FILL_FULL);

  Paint_DrawNum(50, 80, 9.87654321, &Font20, 3, WHITE, BLACK);
  Paint_DrawString_EN(50, 100, "Created By", &Font20, 0x000f, 0xfff0);
  Paint_DrawString_EN(50, 120, "Tabish", &Font20, WHITE, BLUE);

  Paint_DrawString_EN(50, 161, "WaveShare", &Font16, RED, WHITE);

  // Refresh the picture in RAM to LCD
  LCD_1IN28_Display(BlackImage);
  DEV_Delay_ms(2000);

  // Set the initial time here
  SetInitialTime(2024, 7, 30, 1, 10, 0); // Set to desired initial time
}

void loop() {
  while (true) {
    time_t now;
    struct tm *timeinfo;
    char timeString[20];

    // Get the current time
    time(&now);
    timeinfo = localtime(&now);

    // Clear the screen
    Paint_Clear(WHITE);
    DrawBitmap(gImage_background); // Redraw the bitmap background with color correction

    // Draw the current time
    sprintf(timeString, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    Paint_DrawString_EN(50, 50, timeString, &Font24, BLACK, WHITE);

    // Update the display
    LCD_1IN28_Display(BlackImage);
    DEV_Delay_ms(1000); // Update every second
  }
}
