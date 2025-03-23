#include <WiFi.h>                 // For ESP32 Wi-Fi support
#include <WebServer.h>            // For handling HTTP requests
#include <Arduino_JSON.h>         // For parsing JSON data

#include "esp_camera.h"           // ESP32-CAM camera library

#include "tensorflow/lite/micro/micro_interpreter.h"  // TensorFlow Lite Micro
#include "TensorFlowLite_ESP32/src/tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"

// WiFi credentials
const char* ssid = "Tabish realme 8 5G";
const char* password = "Zaintab@123";

// TensorFlow Lite model
extern const unsigned char model_data[]; // Replace with your model data
constexpr int kTensorArenaSize = 10 * 1024;  // Allocate memory for TensorFlow Lite
uint8_t tensor_arena[kTensorArenaSize];

// Declare WebServer on port 80
WebServer server(80);

// TensorFlow Lite interpreter
tflite::MicroInterpreter* interpreter;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  
  // Initialize WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Start server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Server started");

  // Initialize ESP32-CAM camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = CAM_PIN_D0;
  config.pin_d1 = CAM_PIN_D1;
  config.pin_d2 = CAM_PIN_D2;
  config.pin_d3 = CAM_PIN_D3;
  config.pin_d4 = CAM_PIN_D4;
  config.pin_d5 = CAM_PIN_D5;
  config.pin_d6 = CAM_PIN_D6;
  config.pin_d7 = CAM_PIN_D7;
  config.pin_xclk = CAM_PIN_XCLK;
  config.pin_pclk = CAM_PIN_PCLK;
  config.pin_vsync = CAM_PIN_VSYNC;
  config.pin_href = CAM_PIN_HREF;
  config.pin_sccb_sda = CAM_PIN_SIOD;
  config.pin_sccb_scl = CAM_PIN_SIOC;
  config.pin_pwdn = CAM_PIN_PWDN;
  config.pin_reset = CAM_PIN_RESET;
  config.pin_xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;  // Use JPEG format for web streaming

  // Frame buffer size
  config.frame_size = FRAMESIZE_QVGA;    // Use a smaller frame size for TensorFlow Lite compatibility
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Initialize TensorFlow Lite interpreter
  static tflite::AllOpsResolver resolver;
  tflite::MicroInterpreter static_interpreter(model_data, resolver, tensor_arena, kTensorArenaSize, nullptr);
  interpreter = &static_interpreter;

  // Allocate memory for input and output tensors
  interpreter->AllocateTensors();
  Serial.println("TensorFlow Lite Interpreter initialized");
}

void loop() {
  // Capture image
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Send the image to the TensorFlow Lite interpreter for image recognition
  uint8_t* input = interpreter->input(0)->data.uint8;
  memcpy(input, fb->buf, fb->len);  // Assuming your model input matches camera capture

  // Run inference
  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Failed to invoke TensorFlow Lite interpreter");
  } else {
    Serial.println("Inference success!");
    // Read the output tensor
    uint8_t* output = interpreter->output(0)->data.uint8;
    // Handle the result (e.g., send over HTTP, process locally)
  }

  // Return the frame buffer back to the driver for reuse
  esp_camera_fb_return(fb);

  // Handle client requests
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "ESP32-CAM TensorFlow Lite Server is Running");
}
