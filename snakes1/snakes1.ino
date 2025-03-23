#include <LedControl.h>

const int DIN_PIN = 12;
const int CLK_PIN = 10;
const int CS_PIN = 11;
const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_Y_PIN = A1;

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

const int MAX_SNAKE_LENGTH = 10; // Adjust as needed
int snakeX[MAX_SNAKE_LENGTH];
int snakeY[MAX_SNAKE_LENGTH];
int snakeLength = 1;
int snakeHead = 0; // Index of the snake's head

void setup() {
    lc.shutdown(0, false);
    lc.setIntensity(0, 8);
    lc.clearDisplay(0);

    // Initialize snake starting position
    snakeX[0] = 2;
    snakeY[0] = 2;
}

void loop() {
    // Read joystick input
    int joystickX = analogRead(JOYSTICK_X_PIN);
    int joystickY = analogRead(JOYSTICK_Y_PIN);

    // Determine direction based on joystick input
    if (joystickX < 300) {
        moveSnake(-1, 0); // Move left
    } else if (joystickX > 700) {
        moveSnake(1, 0); // Move right
    } else if (joystickY < 300) {
        moveSnake(0, 1); // Move up
    } else if (joystickY > 700) {
        moveSnake(0, -1); // Move down
    }

    // Update snake position on the display
    updateSnakeDisplay();

    // Introduce delay for controlling snake speed
    delay(200); // Adjust delay to control snake speed

    // Erase snake's trail from 2 seconds ago
    eraseOldSnakePosition();
}

void moveSnake(int dx, int dy) {
    // Move the snake by shifting positions
    for (int i = snakeLength - 1; i > 0; --i) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    snakeX[0] += dx;
    snakeY[0] += dy;

    // Ensure snake stays within bounds (0-7 for an 8x8 matrix)
    snakeX[0] = constrain(snakeX[0], 0, 7);
    snakeY[0] = constrain(snakeY[0], 0, 7);
}

void updateSnakeDisplay() {
    lc.clearDisplay(0);
    for (int i = 0; i < snakeLength; ++i) {
        lc.setLed(0, snakeX[i], snakeY[i], true);
    }
}

void eraseOldSnakePosition() {
    static unsigned long lastEraseTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastEraseTime >= 20000) { // Erase every 2 seconds
        lc.setLed(0, snakeX[snakeLength - 1], snakeY[snakeLength - 1], false);
        lastEraseTime = currentTime;
    }
}
