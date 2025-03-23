#include <time.h> // Include time library 
#include "LCD_Test.h"
#include "ImageData.h" // Include your bitmap image data here

UWORD *BlackImage; // Declare BlackImage globally

// Array of love quotes
const char *quotes[] = {
  "Love is a bond stronger than words can hold.",
  "True love grows where hearts meet without expectations.",
  "In love, silence often speaks louder than declarations.",
  "Love creates warmth even in the coldest of hearts.",
  "A gentle touch can ignite a lifetime of love.",
  "Love sees beauty where others see imperfection.",
  "Two hearts in sync make an unbreakable melody.",
  "Love blooms when kindness meets understanding.",
  "Even shadows feel brighter when love stands beside them.",
  "Love's light shines in the darkest of moments.",
  "True love whispers, never demands or shouts.",
  "The language of love is spoken without words."
};

const int quoteCount = sizeof(quotes) / sizeof(quotes[0]);
int currentQuoteIndex = 0;

void DrawBitmap(const unsigned char *image) {
  for (int y = 0; y < LCD_1IN28_HEIGHT; y++) {
    for (int x = 0; x < LCD_1IN28_WIDTH; x++) {
      uint16_t color = image[(y * LCD_1IN28_WIDTH + x) * 2] | (image[(y * LCD_1IN28_WIDTH + x) * 2 + 1] << 8);
      uint8_t r = (color >> 11) & 0x1F;
      uint8_t g = (color >> 5) & 0x3F;
      uint8_t b = color & 0x1F;
      uint16_t correctedColor = (b << 11) | (g << 5) | r;
      Paint_DrawPoint(x, y, correctedColor, DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
    }
  }
}

void SetInitialTime(int year, int month, int day, int hour, int minute, int second) {
  struct tm newTime;
  newTime.tm_year = year - 1900;
  newTime.tm_mon = month - 1;
  newTime.tm_mday = day;
  newTime.tm_hour = hour;
  newTime.tm_min = minute;
  newTime.tm_sec = second;
  newTime.tm_isdst = -1;

  time_t initialTime = mktime(&newTime);
  struct timeval tv;
  tv.tv_sec = initialTime;
  tv.tv_usec = 0;
  settimeofday(&tv, NULL);
}

void setup() {
  if (DEV_Module_Init() != 0)
    Serial.println("GPIO Init Fail!");
  else
    Serial.println("GPIO Init successful!");

  LCD_1IN28_Init(HORIZONTAL);
  DEV_SET_PWM(50);
  LCD_1IN28_Clear(WHITE);
  UDOUBLE Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;

  BlackImage = (UWORD *)malloc(Imagesize);
  if (BlackImage == NULL) {
    Serial.println("Failed to apply for black memory...");
    exit(0);
  }

  Paint_NewImage((UBYTE *)BlackImage, LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
  Paint_SetScale(65);
  Paint_Clear(WHITE);
  Paint_SetRotate(ROTATE_0);
  Paint_Clear(WHITE);

  DrawBitmap(gImage_background);

  LCD_1IN28_Display(BlackImage);
  DEV_Delay_ms(2000);

  SetInitialTime(2024, 12, 19, 3, 00, 0); // Set the initial time
}

void loop() {
  time_t lastQuoteChange = 0;

  while (true) {
    time_t now;
    time(&now);

    if (now - lastQuoteChange >= 300) { // Change every 5 minutes (300 seconds)
      lastQuoteChange = now;
      currentQuoteIndex = (currentQuoteIndex + 1) % quoteCount;

      Paint_Clear(WHITE);
      DrawBitmap(gImage_background);

      Paint_DrawString_EN(15, 100, quotes[currentQuoteIndex], &Font16, BLACK, WHITE);

      LCD_1IN28_Display(BlackImage);
    }
    DEV_Delay_ms(1000); // Check every second
  }
}
