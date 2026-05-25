# 🎧 Voice and Motion Sensing Headband for Computer Assistance

> **Hands-free computer control using head movements and voice commands — designed for individuals with physical disabilities.**

---

### 🏷️ Tech Stack

![MCU](https://img.shields.io/badge/MCU-Xiao%20ESP32--C3-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![Sensor](https://img.shields.io/badge/Sensor-MPU6050-0078D4?style=for-the-badge&logo=arduino&logoColor=white)
![Voice](https://img.shields.io/badge/Voice-VC--02%20Offline-8A2BE2?style=for-the-badge&logo=googleassistant&logoColor=white)
![HID](https://img.shields.io/badge/HID-Arduino%20Pro%20Micro-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![Protocol](https://img.shields.io/badge/Protocol-BLE%20%2B%20UART-F7931E?style=for-the-badge&logo=bluetooth&logoColor=white)
![Power](https://img.shields.io/badge/Power-LiPo%20Battery-4CAF50?style=for-the-badge&logo=battery&logoColor=white)
![Language](https://img.shields.io/badge/Language-C%2B%2B%20%2F%20Arduino-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Domain](https://img.shields.io/badge/Domain-Assistive%20Technology-FF6B6B?style=for-the-badge)

---

## 📌 Table of Contents

- [Overview](#-overview)
- [System Architecture](#-system-architecture)
- [Hardware Components](#-hardware-components)
- [How It Works](#-how-it-works)
- [Firmware Files](#-firmware-files)
- [Supported Voice Commands](#-supported-voice-commands)
- [Circuit Connections](#-circuit-connections)
- [Libraries Required](#-libraries-required)
- [Setup & Flashing Guide](#-setup--flashing-guide)
- [Future Improvements](#-future-improvements)
- [License](#-license)

---

## 🧠 Overview

This project provides an affordable, wearable assistive device for individuals who have lost the use of their arms due to accidents or disabilities. By combining **head motion tracking** and **offline voice recognition**, users can control a computer cursor and trigger keyboard shortcuts — entirely hands-free.

The system uses two microcontrollers working in tandem:
- **Xiao ESP32-C3** on the headband — reads motion data and sends it wirelessly via Bluetooth Low Energy (BLE)
- **Arduino Pro Micro** as a USB dongle — receives UART signals from voice module and acts as a standard HID (Human Interface Device) to the computer

---

## 🏗️ System Architecture

> The diagram below shows the complete data flow from sensor inputs on the headband to computer actions.

```
╔══════════════════════════════════════════════════════════════════════╗
║                  HEADBAND  (Worn by the user)                        ║
║                                                                      ║
║   ┌────────────────┐   I2C    ┌──────────────────────────┐          ║
║   │   MPU6050      │ ───────► │                          │          ║
║   │  Gyroscope /   │          │      Xiao ESP32-C3       │          ║
║   │ Accelerometer  │          │    (Main Controller)     │ ~~~BLE~~~► ║
║   └────────────────┘          │                          │          ║
║                               └──────────────────────────┘          ║
║   ┌────────────────┐   UART              ▲                           ║
║   │    VC-02       │ ───────────────────►│                           ║
║   │ Offline Voice  │                     │                           ║
║   │ Command Module │          ┌──────────┴───────┐                   ║
║   └────────────────┘          │   LiPo Battery   │                   ║
║                               └──────────────────┘                   ║
╚══════════════════════════════════════════════════════════════════════╝
                                        │
                              BLE Wireless Link
                                        │
                                        ▼
╔══════════════════════════════════════════════════════════════════════╗
║                USB DONGLE  (Plugged into PC)                         ║
║                                                                      ║
║           ┌─────────────────────────────────┐                        ║
║           │       Arduino Pro Micro         │                        ║
║           │   USB HID — Mouse & Keyboard    │                        ║
║           └────────────────┬────────────────┘                        ║
╚════════════════════════════╪═════════════════════════════════════════╝
                             │ USB
                             ▼
                    ┌─────────────────┐
                    │   Computer /    │
                    │   Windows PC    │
                    └─────────────────┘
```

### ⚡ Data Flow

```
Head Movement  ──►  MPU6050  ──►  ESP32-C3  ──►  BLE  ──►  Pro Micro  ──►  🖱️ Mouse Move
Voice Command  ──►  VC-02   ──►  ESP32-C3  ──►  BLE  ──►  Pro Micro  ──►  ⌨️ Key Press
```

---

## 🔧 Hardware Components

| # | Component | Role |
|---|---|---|
| 1 | **Xiao ESP32-C3** | Main MCU on headband — processes sensor data and transmits via BLE |
| 2 | **MPU6050** | 6-axis gyroscope + accelerometer — detects head pitch and roll |
| 3 | **VC-02** | Offline voice recognition module — no internet needed |
| 4 | **Arduino Pro Micro** | USB HID device — translates BLE/UART signals to mouse & keyboard events |
| 5 | **LiPo Battery** | Portable rechargeable power for the headband |

---

## ⚙️ How It Works

### 1. Head Motion → Mouse Control (`final_mouseble.ino`)

The MPU6050 reads 3-axis acceleration every 20ms. These values are converted to **pitch** and **roll** angles:

```
Pitch = atan2(ax, √(ay² + az²))  →  Vertical mouse movement   (Y-axis)
Roll  = atan2(ay, √(ax² + az²))  →  Horizontal mouse movement (X-axis)
```

A **±2° dead zone** filters accidental micro-movements.

| Head Movement | Mouse Action |
|---|---|
| Tilt Left / Right | Cursor moves Left / Right |
| Tilt Forward / Backward | Cursor moves Down / Up |
| Hold still | Cursor stays in place |

**Tunable sensitivity in code:**
```cpp
const float movementThreshold = 2.0;   // Dead zone (degrees)
const float horizontalScaling = 3.0;   // Left-right speed
const float verticalScaling   = 0.5;   // Up-down speed
```

---

### 2. Voice Commands → Keyboard Actions (`donggle_cmd.ino`)

The **VC-02** sends a 2-byte UART code per recognized phrase. The Arduino Pro Micro reads these via `Serial1` and fires the corresponding HID action.

Protocol: `[0x12] [Command Byte]`

---

## 📂 Firmware Files

| File | Runs On | Purpose |
|---|---|---|
| `final_mouseble.ino` | Xiao ESP32-C3 | Reads MPU6050, computes pitch/roll, sends BLE mouse events |
| `donggle_cmd.ino` | Arduino Pro Micro | Receives VC-02 UART codes, executes HID keyboard/mouse actions |

---

## 🎙️ Supported Voice Commands

| Voice Phrase | Byte Code | Action |
|---|---|---|
| *"Right Click"* | `0x12 0x0A` | 🖱️ Right mouse click |
| *"Left Click"* | `0x12 0x1A` | 🖱️ Left mouse click |
| *"Open Dictation"* | `0x12 0x2A` | ⊞ Win + H |
| *"Backspace"* | `0x12 0x3A` | ⌫ Delete last character |
| *"Open Start"* | `0x12 0x4A` | ⊞ Windows Start Menu |
| *"Copy"* | `0x12 0x5A` | Ctrl + C |
| *"Paste"* | `0x12 0x6A` | Ctrl + V |
| *"Task Manager"* | `0x12 0x7A` | Ctrl + Shift + Esc |
| *"Select All"* | `0x12 0x8A` | Ctrl + A |

> ✅ VC-02 works **fully offline** — no Wi-Fi or internet required.

---

## 🔌 Circuit Connections

### ESP32-C3 ↔ MPU6050 (I2C)

| ESP32-C3 Pin | MPU6050 Pin |
|---|---|
| 3.3V | VCC |
| GND | GND |
| GPIO6 (SDA) | SDA |
| GPIO7 (SCL) | SCL |

### ESP32-C3 ↔ VC-02 (UART)

| ESP32-C3 Pin | VC-02 Pin |
|---|---|
| GPIO20 (RX) | TX |
| GPIO21 (TX) | RX |
| 3.3V | VCC |
| GND | GND |

> **Arduino Pro Micro** connects directly to PC via USB — enumerates as HID automatically.

---

## 📦 Libraries Required

### `final_mouseble.ino` — Xiao ESP32-C3

Install via Arduino Library Manager:

| Library | Author |
|---|---|
| `Adafruit MPU6050` | Adafruit |
| `Adafruit Unified Sensor` | Adafruit |
| `ESP32-BLE-Mouse` | [T-vK (GitHub)](https://github.com/T-vK/ESP32-BLE-Mouse) — install as ZIP |

### `donggle_cmd.ino` — Arduino Pro Micro

| Library | Source |
|---|---|
| `Keyboard.h` | Built into Arduino IDE |
| `Mouse.h` | Built into Arduino IDE |

---

## 🚀 Setup & Flashing Guide

### Step 1 — Install Arduino IDE
Download from [arduino.cc](https://www.arduino.cc/en/software)

### Step 2 — Add ESP32 Board Support
Go to **File → Preferences**, add this URL under Additional Board Manager URLs:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
Then: **Tools → Board → Board Manager** → Install `esp32 by Espressif`

### Step 3 — Install Libraries
**Tools → Manage Libraries** → Install:
- `Adafruit MPU6050`
- `Adafruit Unified Sensor`

Download [ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse) as ZIP → **Sketch → Include Library → Add .ZIP Library**

### Step 4 — Flash the Headband (ESP32-C3)
```
Board   : XIAO_ESP32C3
File    : final_mouseble.ino
→ Upload
```

### Step 5 — Flash the Dongle (Pro Micro)
```
Board   : Arduino Leonardo
File    : donggle_cmd.ino
→ Upload
```

### Step 6 — Pair via Bluetooth
1. Plug the Pro Micro dongle into your PC
2. Power on the headband
3. **Windows Settings → Bluetooth → Add Device**
4. Connect to `"ESP32 BLE Mouse"`
5. Done — start moving your head!

---

## 🔮 Future Improvements

- [ ] Gyroscope drift correction using a **Complementary or Kalman filter**
- [ ] **Dwell-to-click** — auto-click after holding still for N seconds
- [ ] Scroll gesture detection via specific head nod patterns
- [ ] AI-based gesture recognition with ML model on-device
- [ ] Mobile companion app for sensitivity tuning and calibration
- [ ] Expand VC-02 voice model with more commands and accent support
- [ ] Custom miniaturized PCB for a cleaner wearable design

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

## 🙌 Contributing

Contributions are welcome! Fork the repo, make your changes, and open a pull request.
For major changes, please open an issue first to discuss what you'd like to change.

---

*Built with ❤️ to make computers accessible for everyone.*
