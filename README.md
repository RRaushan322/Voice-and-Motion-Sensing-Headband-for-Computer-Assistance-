# 🎧 Voice and Motion Sensing Headband for Computer Assistance

> **Hands-free computer control using head movements and voice commands — designed for individuals with physical disabilities.**

---

## 📌 Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Hardware Components](#hardware-components)
- [How It Works](#how-it-works)
- [Firmware Files](#firmware-files)
- [Supported Voice Commands](#supported-voice-commands)
- [Circuit Connections](#circuit-connections)
- [Libraries Required](#libraries-required)
- [Setup & Flashing Guide](#setup--flashing-guide)
- [Future Improvements](#future-improvements)
- [License](#license)

---

## 🧠 Overview

This project provides an affordable, wearable assistive device for individuals who have lost the use of their arms due to accidents or disabilities. By combining **head motion tracking** and **offline voice recognition**, users can control a computer cursor and trigger keyboard shortcuts — entirely hands-free.

The system uses two microcontrollers working in tandem:
- **Xiao ESP32-C3** on the headband — reads motion data and sends it wirelessly via Bluetooth Low Energy (BLE)
- **Arduino Pro Micro** as a USB dongle — receives BLE/UART signals and acts as a standard HID (Human Interface Device) to the computer

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        HEADBAND (Worn by User)                      │
│                                                                     │
│   ┌──────────────┐     I2C      ┌─────────────────────┐            │
│   │   MPU6050    │ ──────────►  │                     │            │
│   │  (Gyroscope/ │              │    Xiao ESP32-C3    │            │
│   │ Accelerometer│              │   (Main Controller) │            │
│   └──────────────┘              │                     │            │
│                                 └────────┬────────────┘            │
│   ┌──────────────┐    UART             │ BLE                       │
│   │    VC-02     │ ──────────►         │ (Bluetooth Low Energy)    │
│   │ (Offline     │              ┌──────▼──────┐                    │
│   │  Voice Cmd)  │              │  LiPo Battery│                   │
│   └──────────────┘              └─────────────┘                    │
└─────────────────────────────────────────────────────────────────────┘
                                        │
                              BLE Wireless Link
                                        │
                                        ▼
┌─────────────────────────────────────────────────────────────────────┐
│                     USB DONGLE (Plugged into PC)                    │
│                                                                     │
│              ┌────────────────────────────┐                         │
│              │    Arduino Pro Micro       │                         │
│              │  (USB HID — acts as Mouse  │                         │
│              │        & Keyboard)         │                         │
│              └────────────┬───────────────┘                         │
│                           │ USB                                     │
└───────────────────────────┼─────────────────────────────────────────┘
                            │
                            ▼
                    ┌───────────────┐
                    │   Computer    │
                    │  (Windows PC) │
                    └───────────────┘
```

### Data Flow Summary

```
Head Movement  ──►  MPU6050  ──► ESP32-C3  ──► BLE  ──► Pro Micro  ──► Mouse Move
Voice Command  ──►  VC-02    ──► ESP32-C3  ──► BLE  ──► Pro Micro  ──► Key Press
```

---

## 🔧 Hardware Components

| Component | Role | Qty |
|---|---|---|
| **Xiao ESP32-C3** | Main microcontroller on headband; processes sensor data and handles BLE | 1 |
| **MPU6050** | 6-axis gyroscope + accelerometer; detects head pitch and roll | 1 |
| **VC-02** | Offline voice command module; recognizes predefined spoken commands | 1 |
| **Arduino Pro Micro** | USB HID device; translates received commands into mouse/keyboard inputs | 1 |
| **LiPo Battery** | Portable rechargeable power source for the headband | 1 |

---

## ⚙️ How It Works

### 1. Head Motion → Mouse Control (`final_mouseble.ino`)

The MPU6050 sensor is mounted on the headband and reads raw 3-axis acceleration values every 20ms. These values are converted into **pitch** (up-down tilt) and **roll** (left-right tilt) angles using inverse trigonometry:

```
Pitch = atan2(ax, √(ay² + az²))   →  maps to Y-axis (vertical mouse movement)
Roll  = atan2(ay, √(ax² + az²))   →  maps to X-axis (horizontal mouse movement)
```

A **dead zone threshold of ±2°** filters out accidental micro-movements. Beyond that, the angles are scaled and sent as BLE HID mouse movement packets.

| Head Movement | Mapped Action |
|---|---|
| Tilt Left / Right | Move cursor Left / Right |
| Tilt Forward / Back | Move cursor Down / Up |
| Hold still | Cursor stays in place |

**Sensitivity settings** (adjustable in code):
```cpp
const float movementThreshold = 2.0;   // Dead zone (degrees)
const float horizontalScaling = 3.0;   // Left-right speed multiplier
const float verticalScaling   = 0.5;   // Up-down speed multiplier
```

---

### 2. Voice Commands → Keyboard Actions (`donggle_cmd.ino`)

The **VC-02 offline voice module** listens for spoken commands and outputs a 2-byte UART code for each recognized phrase. The Arduino Pro Micro reads these codes over `Serial1` and executes the corresponding HID keyboard or mouse action.

The first byte is always `0x12` (command header). The second byte identifies the specific command.

---

## 📂 Firmware Files

| File | Runs On | Purpose |
|---|---|---|
| `final_mouseble.ino` | Xiao ESP32-C3 | Reads MPU6050, computes pitch/roll, sends BLE mouse events |
| `donggle_cmd.ino` | Arduino Pro Micro | Receives UART from VC-02, executes keyboard/mouse HID actions |

---

## 🎙️ Supported Voice Commands

| Voice Phrase | Byte Code | Action |
|---|---|---|
| *Right Click* | `0x12 0x0A` | Right mouse click |
| *Left Click* | `0x12 0x1A` | Left mouse click |
| *Open Dictation* | `0x12 0x2A` | Windows + H (voice input) |
| *Backspace* | `0x12 0x3A` | Delete last character |
| *Open Start* | `0x12 0x4A` | Windows Start Menu |
| *Copy* | `0x12 0x5A` | Ctrl + C |
| *Paste* | `0x12 0x6A` | Ctrl + V |
| *Task Manager* | `0x12 0x7A` | Ctrl + Shift + Esc |
| *Select All* | `0x12 0x8A` | Ctrl + A |

> **Note:** The VC-02 module works fully offline — no internet connection is required for voice recognition.

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

### Arduino Pro Micro ↔ Computer

Connect via USB — the Pro Micro enumerates as a HID device automatically.

---

## 📦 Libraries Required

### For `final_mouseble.ino` (ESP32-C3)

Install via Arduino Library Manager:

```
Adafruit MPU6050       → by Adafruit
Adafruit Unified Sensor → by Adafruit
ESP32 BLE Mouse        → by T-vK (GitHub: T-vK/ESP32-BLE-Mouse)
```

### For `donggle_cmd.ino` (Arduino Pro Micro)

These are built into the Arduino AVR core:

```
Keyboard.h   → included with Arduino IDE
Mouse.h      → included with Arduino IDE
```

---

## 🚀 Setup & Flashing Guide

### Step 1 — Install Arduino IDE
Download from [arduino.cc](https://www.arduino.cc/en/software)

### Step 2 — Add ESP32 Board Support
Go to **File → Preferences** and add this URL to Board Manager:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
Then go to **Tools → Board → Board Manager** and install `esp32 by Espressif`.

### Step 3 — Install Required Libraries
Go to **Tools → Manage Libraries** and install:
- `Adafruit MPU6050`
- `Adafruit Unified Sensor`
- Download and install [ESP32-BLE-Mouse](https://github.com/T-vK/ESP32-BLE-Mouse) manually (ZIP)

### Step 4 — Flash the Headband
1. Select **Board**: `XIAO_ESP32C3`
2. Open `final_mouseble.ino`
3. Upload

### Step 5 — Flash the Dongle
1. Select **Board**: `Arduino Leonardo` (Pro Micro uses the same core)
2. Open `donggle_cmd.ino`
3. Upload

### Step 6 — Pair via Bluetooth
1. Plug the Pro Micro dongle into your PC
2. Power on the headband
3. Go to Windows **Bluetooth Settings → Add Device**
4. Connect to `"ESP32 BLE Mouse"`

---

## 🔮 Future Improvements

- [ ] **Gyroscope drift correction** using a complementary or Kalman filter for smoother cursor control
- [ ] **Scroll gesture** — detect specific head nod patterns for scrolling
- [ ] **Click dwell timer** — auto-click when cursor is held still for N seconds (removes need for voice click)
- [ ] **AI-powered gesture recognition** — expand beyond pitch/roll with ML models
- [ ] **Mobile companion app** — for configuring sensitivity, commands, and calibration
- [ ] **Custom VC-02 voice model** — train with more commands and Indian English accent support
- [ ] **Miniaturized PCB** — single-board design for a cleaner wearable form factor

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

## 🙌 Contributing

Contributions are welcome! Fork the repo, make your changes, and open a pull request. For major changes, please open an issue first to discuss what you'd like to change.

---

*Built with ❤️ to make computers accessible for everyone.*
