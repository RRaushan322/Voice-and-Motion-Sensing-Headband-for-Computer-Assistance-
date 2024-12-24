#include <Keyboard.h>
#include <Mouse.h>

// Define the baud rate of the VC-O2 sensor
const long baudRate = 9600;

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);   // For communicating with the Arduino IDE's serial monitor
  Serial1.begin(baudRate);  // Use Serial1 for UART communication on Arduino Pro Micro

  // Initialize the Keyboard and Mouse libraries
  Keyboard.begin();
  Mouse.begin();

  // Wait for the serial connection to establish
  while (!Serial) {
    ; // Wait for the serial port to connect
  }

  Serial.println("VC-O2 Communication Started!");
}

void loop() {
  // Check if data is available from the VC-O2 sensor
  if (Serial1.available()) {
    byte receivedByte1 = Serial1.read();  // Read first byte
    byte receivedByte2 = Serial1.read();  // Read second byte

    // Combine both bytes (for example: "12 0A")
    if (receivedByte1 == 0x12) {
      // Handle each command based on receivedByte2
      switch (receivedByte2) {
        case 0x0A:  // Right Click
          Mouse.click(MOUSE_RIGHT);
          Serial.println("Right Click");
          break;

        case 0x1A:  // Left Click
          Mouse.click(MOUSE_LEFT);
          Serial.println("Left Click");
          break;

        case 0x2A:  // Windows + H (Show desktop in Windows)
          Keyboard.press(KEY_LEFT_GUI); // Press the Windows key (GUI key)
          Keyboard.press('h');          // Press 'H'
          Keyboard.releaseAll();        // Release all keys
          Serial.println("Windows + H");
          break;

        case 0x3A:  // Backspace
          Keyboard.press(KEY_BACKSPACE); // Press Backspace key
          Keyboard.releaseAll();         // Release all keys
          Serial.println("Backspace");
          break;

        case 0x4A:  // Open Window (Start Menu)
          Keyboard.press(KEY_LEFT_GUI);  // Press Windows key (Start menu)
          Keyboard.releaseAll();         // Release all keys
          Serial.println("Open Window");
          break;

        case 0x5A:  // Copy (Ctrl + C)
          Keyboard.press(KEY_LEFT_CTRL);  // Press Ctrl key
          Keyboard.press('c');            // Press 'C' key
          Keyboard.releaseAll();          // Release all keys
          Serial.println("Copy");
          break;

        case 0x6A:  // Paste (Ctrl + V)
          Keyboard.press(KEY_LEFT_CTRL);  // Press Ctrl key
          Keyboard.press('v');            // Press 'V' key
          Keyboard.releaseAll();          // Release all keys
          Serial.println("Paste");
          break;

        case 0x7A:  // Task Manager (Ctrl + Shift + Esc)
          Keyboard.press(KEY_LEFT_CTRL);  // Press Ctrl key
          Keyboard.press(KEY_LEFT_SHIFT); // Press Shift key
          Keyboard.press(KEY_ESC);        // Press Esc key
          Keyboard.releaseAll();          // Release all keys
          Serial.println("Task Manager");
          break;

        case 0x8A:  // Select All (Ctrl + A)
          Keyboard.press(KEY_LEFT_CTRL);  // Press Ctrl key
          Keyboard.press('a');            // Press 'A' key
          Keyboard.releaseAll();          // Release all keys
          Serial.println("Select All");
          break;

        default:
          Serial.println("Unknown Command");
          break;
      }
    }
  }

  // Add a small delay to prevent overwhelming the serial output
  delay(100);
}
