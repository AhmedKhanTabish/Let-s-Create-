#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

const float FILTER_ALPHA = 0.1;  // Smoothing factor for low-pass filter

// Variables for storing filtered data
float ax_filtered, ay_filtered, az_filtered;
float gx_filtered, gy_filtered, gz_filtered;

// Bluetooth serial
#define BT_SERIAL Serial

void setup() {
  // Start serial communication with computer and Bluetooth
  Serial.begin(9600);
  BT_SERIAL.begin(9600);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Apply low-pass filter
  ax_filtered = FILTER_ALPHA * a.acceleration.x + (1 - FILTER_ALPHA) * ax_filtered;
  ay_filtered = FILTER_ALPHA * a.acceleration.y + (1 - FILTER_ALPHA) * ay_filtered;
  az_filtered = FILTER_ALPHA * a.acceleration.z + (1 - FILTER_ALPHA) * az_filtered;
  gx_filtered = FILTER_ALPHA * g.gyro.x + (1 - FILTER_ALPHA) * gx_filtered;
  gy_filtered = FILTER_ALPHA * g.gyro.y + (1 - FILTER_ALPHA) * gy_filtered;
  gz_filtered = FILTER_ALPHA * g.gyro.z + (1 - FILTER_ALPHA) * gz_filtered;

  // Convert filtered data to a letter or pattern
  String letter = recognizeLetter(ax_filtered, ay_filtered, az_filtered, gx_filtered, gy_filtered, gz_filtered);

  // Send recognized letter to Bluetooth serial
  BT_SERIAL.println(letter);

  delay(100);  // Adjust delay as needed
}

String recognizeLetter(float ax, float ay, float az, float gx, float gy, float gz) {
  // Simple placeholder for recognition logic
  // Implement your letter recognition algorithm here
  // For example:
  if (gx > 1.0 && gy > 1.0) {
    return "A";  // Example condition
  }
  return "Unknown";
}
