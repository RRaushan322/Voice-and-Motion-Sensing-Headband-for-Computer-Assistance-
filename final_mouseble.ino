#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <BleMouse.h>
#include <math.h> // For trigonometric functions

Adafruit_MPU6050 mpu;
BleMouse bleMouse;

// Sensitivity settings
const float movementThreshold = 2.0; // Ignore small head movements
const float horizontalScaling = 3.0; // Scaling factor for left-right movement
const float verticalScaling = 0.5;   // Scaling factor for up-down movement

void setup() {
  Serial.begin(115200);
  Serial.println("Starting MPU6050 and BLE Mouse!");

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Initialized!");

  // Initialize BLE Mouse
  bleMouse.begin();

  delay(100);
}

void loop() {
  // Check if BLE Mouse is connected
  if (bleMouse.isConnected()) {
    // Read MPU6050 accelerometer data
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Calculate Pitch and Roll
    float pitch = atan2(a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;
    float roll = atan2(a.acceleration.y, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.z * a.acceleration.z)) * 180 / PI;

    // Debug: Print pitch and roll values
    Serial.print("Pitch: ");
    Serial.print(pitch);
    Serial.print(", Roll: ");
    Serial.println(roll);

    // Initialize mouse movement variables
    int moveX = 0; // Left-Right movement (roll)
    int moveY = 0; // Up-Down movement (pitch)

    // Map Roll to Horizontal Mouse Movement (Left/Right)
    if (abs(roll) > movementThreshold) {
      moveX = horizontalScaling * roll; // Scale roll for left-right movement
    }

    // Map Pitch to Vertical Mouse Movement (Up/Down)
    if (abs(pitch) > movementThreshold) {
      moveY = -verticalScaling * pitch; // Scale pitch for up-down movement
    }

    // Apply mouse movement
    bleMouse.move(moveX, moveY);

    // Debug: Show mapped mouse movement
    Serial.print("MoveX: ");
    Serial.print(moveX);
    Serial.print(", MoveY: ");
    Serial.println(moveY);
  }

  delay(20); // Adjust delay for smoother movement
}
