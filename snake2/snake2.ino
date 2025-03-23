#include <LedControl.h>

const int DIN_PIN = 12;
const int CLK_PIN = 10;
const int CS_PIN = 11;
const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_Y_PIN = A1;

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1); // MAX7219 device

int snakeX, snakeY;             // Snake head position
int snakeDirectionX, snakeDirectionY; // Snake movement direction
const int maxSnakeLength = 64;  // Maximum snake length (8x8 matrix size)

struct SnakeSegment {
  int x;
  int y;
};

SnakeSegment snake[maxSnakeLength]; // Snake segments array
int snakeLength = 1;                // Initial snake length
int snakeTailIndex = 0;             // Index of the snake's tail segment

unsigned long lastMoveTime = 0;     // Time of last snake movement
const unsigned long moveInterval = 200; // Snake movement interval (ms)

unsigned long lastSizeChangeTime = 0;   // Time of last size change
const unsigned long sizeIncreaseInterval = 10000; // Size increase interval (ms)
const unsigned long sizeDecreaseInterval = 20000; // Size decrease interval (ms)

void setup() {
  lc.shutdown(0, false);       // Wake up MAX7219
  lc.setIntensity(0, 8);       // Set brightness level
  lc.clearDisplay(0);          // Clear the display

  // Initialize snake starting position and direction
  snakeX = 2;
  snakeY = 2;
  snakeDirectionX = 1;
  snakeDirectionY = 0;

  // Initialize snake segments array
  snake[0].x = snakeX;
  snake[0].y = snakeY;

  lastMoveTime = millis(); // Initialize last move time
}

void loop() {
  // Read joystick input
  int joystickX = analogRead(JOYSTICK_X_PIN);
  int joystickY = analogRead(JOYSTICK_Y_PIN);

  // Determine direction based on joystick input
  if (joystickX < 300) {
    snakeDirectionX = -1; // Move left
    snakeDirectionY = 0;
  } else if (joystickX > 700) {
    snakeDirectionX = 1; // Move right
    snakeDirectionY = 0;
  } else if (joystickY < 300) {
    snakeDirectionX = 0;
    snakeDirectionY = 1; // Move up
  } else if (joystickY > 700) {
    snakeDirectionX = 0;
    snakeDirectionY = -1; // Move down
  }

  // Update snake position if interval has passed
  unsigned long currentTime = millis();
  if (currentTime - lastMoveTime >= moveInterval) {
    lastMoveTime = currentTime;

    // Erase tail segment from display
    lc.setLed(0, snake[snakeTailIndex].x, snake[snakeTailIndex].y, false);

    // Update snake segments positions
    snakeX += snakeDirectionX;
    snakeY += snakeDirectionY;

    // Wrap snake around screen edges
    snakeX = (snakeX + 8) % 8;
    snakeY = (snakeY + 8) % 8;

    // Update snake head position
    snake[0].x = snakeX;
    snake[0].y = snakeY;

    // Update snake segments positions
    for (int i = snakeLength - 1; i > 0; --i) {
      snake[i] = snake[i - 1];
    }

    // Redraw snake segments on display
    for (int i = 0; i < snakeLength; ++i) {
      lc.setLed(0, snake[i].x, snake[i].y, true);
    }

    // Check if snake length should increase
    if (currentTime - lastSizeChangeTime >= sizeIncreaseInterval) {
      lastSizeChangeTime = currentTime;
      if (snakeLength < maxSnakeLength) {
        snakeLength++;
      }
    }

    // Check if snake length should decrease
    if (currentTime - lastSizeChangeTime >= sizeDecreaseInterval) {
      lastSizeChangeTime = currentTime;
      if (snakeLength > 1) {
        // Erase the tail segment from display
        lc.setLed(0, snake[snakeTailIndex].x, snake[snakeTailIndex].y, false);
        
        snakeTailIndex = (snakeTailIndex + 1) % maxSnakeLength;
        snakeLength--;
      }
    }
  }

  // Introduce delay for controlling snake speed
  delay(50); // Adjust delay to control snake speed
}
