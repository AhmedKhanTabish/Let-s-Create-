#include <Wire.h>
#include "LCD_Test.h"
#include "ImageData.h"
#include "MPU6050.h" // Include IMU sensor library

MPU6050 mpu; // Gyroscope instance

UWORD *BlackImage;
int playerX = LCD_1IN28_WIDTH / 2;  // Player start position
int playerY = LCD_1IN28_HEIGHT - 20;
const int playerSpeed = 5;

struct UFO {
  int x, y;
  bool active;
};

const int maxUFOs = 5;
UFO ufos[maxUFOs];

void setup() {
  Serial.begin(115200);
  DEV_Module_Init();
  LCD_1IN28_Init(HORIZONTAL);
  DEV_SET_PWM(50);
  LCD_1IN28_Clear(WHITE);
  
  // Allocate memory for the screen
  UDOUBLE Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
  BlackImage = (UWORD *)malloc(Imagesize);
  if (BlackImage == NULL) {
    Serial.println("Memory allocation failed!");
    exit(0);
  }
  
  Paint_NewImage((UBYTE *)BlackImage, LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
  Paint_Clear(WHITE);
  
  // Initialize IMU sensor
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  // Initialize UFOs
  for (int i = 0; i < maxUFOs; i++) {
    ufos[i] = { random(10, LCD_1IN28_WIDTH - 10), random(-50, -10), true };
  }
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Move player based on gyroscope
  if (gx > 2000) playerX -= playerSpeed; // Tilt left
  if (gx < -2000) playerX += playerSpeed; // Tilt right

  // Keep player within screen bounds
  playerX = constrain(playerX, 0, LCD_1IN28_WIDTH - 10);

  // Update UFO positions
  for (int i = 0; i < maxUFOs; i++) {
    if (ufos[i].active) {
      ufos[i].y += 3; // Move downward
      
      // Respawn UFOs
      if (ufos[i].y > LCD_1IN28_HEIGHT) {
        ufos[i].y = random(-50, -10);
        ufos[i].x = random(10, LCD_1IN28_WIDTH - 10);
      }
    }
  }

  // Draw Scene
  Paint_Clear(WHITE);
  
  // Draw UFOs
  for (int i = 0; i < maxUFOs; i++) {
    if (ufos[i].active) {
      Paint_DrawCircle(ufos[i].x, ufos[i].y, 5, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    }
  }

  // Draw Player
  Paint_DrawRectangle(playerX, playerY, playerX + 10, playerY + 5, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

  // Update Screen
  LCD_1IN28_Display(BlackImage);
  
  DEV_Delay_ms(50);
}

