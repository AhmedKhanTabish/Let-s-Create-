#include "LCD_Test.h"

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
  UWORD *BlackImage;
  if ((BlackImage = (UWORD *)malloc(Imagesize)) == NULL)
  {
      Serial.println("Failed to apply for black memory...");
      exit(0);
  }
  // /*1.Create a new image cache named IMAGE_RGB and fill it with white*/
  Paint_NewImage((UBYTE *)BlackImage, LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
  Paint_SetScale(65);
  Paint_Clear(WHITE);
  Paint_SetRotate(ROTATE_0);
  Paint_Clear(WHITE);
#if 1
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
  Paint_DrawString_EN(50, 100, "ABC", &Font20, 0x000f, 0xfff0);
  Paint_DrawString_CN(50, 120, "微雪电子", &Font24CN, WHITE, BLUE);

  Paint_DrawString_EN(50, 161, "WaveShare", &Font16, RED, WHITE);

  // /*3.Refresh the picture in RAM to LCD*/
  LCD_1IN28_Display(BlackImage);
  DEV_Delay_ms(2000);

#endif
  // /* GUI */
  Serial.println("drawing...\r\n");
  // /*2.Drawing on the image*/
#if 0
    Paint_DrawRectangle(35, 35, 206, 206, BLACK, DOT_PIXEL_3X3, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(0, 0, 33, 208,RED,DOT_PIXEL_1X1,DRAW_FILL_FULL);
    Paint_DrawRectangle(0, 0, 208, 33,GREEN,DOT_PIXEL_1X1,DRAW_FILL_FULL);
    Paint_DrawRectangle(0, 208, 240, 240,BLUE,DOT_PIXEL_1X1,DRAW_FILL_FULL);
    Paint_DrawRectangle(208, 0, 240, 240,BLUE,DOT_PIXEL_1X1,DRAW_FILL_FULL);
    LCD_1IN28_Display(BlackImage);
    DEV_Delay_ms(5000);

#endif
    float acc[3], gyro[3];
    unsigned int tim_count = 0;
    uint16_t result;
    QMI8658_init();
    Serial.println("QMI8658_init\r\n");
    DEV_SET_PWM(100);
    const float conversion_factor = 3.3f / (1 << 12) * 2;
    while (true)
    {
        result = DEC_ADC_Read();
        Paint_Clear(WHITE);
        QMI8658_read_xyz(acc, gyro, &tim_count);

        Paint_DrawString_EN(30, 50, "ACC_X = ", &Font16, WHITE, BLACK);
        Paint_DrawString_EN(30, 75, "ACC_Y = ", &Font16, WHITE, BLACK);
        Paint_DrawString_EN(30, 100, "ACC_Z = ", &Font16, WHITE, BLACK);
        Paint_DrawString_EN(30, 125, "GYR_X = ", &Font16, WHITE, BLACK);
        Paint_DrawString_EN(30, 150, "GYR_Y = ", &Font16, WHITE, BLACK);
        Paint_DrawString_EN(30, 175, "GYR_Z = ", &Font16, WHITE, BLACK);
        Paint_DrawString_EN(30, 175, "GYR_Z = ", &Font16, WHITE, BLACK);
        Paint_DrawNum(120, 50, acc[0], &Font16, 2, BLACK, WHITE);
        Paint_DrawNum(120, 75, acc[1], &Font16, 2, BLACK, WHITE);
        Paint_DrawNum(120, 100, acc[2], &Font16, 2, BLACK, WHITE);
        Paint_DrawNum(120, 125, gyro[0], &Font16, 2, BLACK, WHITE);
        Paint_DrawNum(120, 150, gyro[1], &Font16, 2, BLACK, WHITE);
        Paint_DrawNum(120, 175, gyro[2], &Font16, 2, BLACK, WHITE);
        Paint_DrawString_EN(50, 200, "BAT(V)=", &Font16, WHITE, BLACK);
        Paint_DrawNum(130, 200, result * conversion_factor, &Font16, 2, BLACK, WHITE);

        LCD_1IN28_Display(BlackImage);
//        DEV_Delay_ms(100);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println("GPIO Init successful!");
//  delay(500);
}