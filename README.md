
# Voice and Motion Sensing Headband for Computer Assistance

This project is an innovative, headband-based solution designed to assist individuals with physical disabilities, particularly those who have lost the use of their arms due to accidents. By leveraging motion sensing and voice commands, this system offers hands-free control of essential computer functionalities, including mouse control and keyboard shortcuts.

---

## Features

- **Hands-Free Operation**: Combines head movements and voice commands to control the computer.
- **Motion Sensing**: Tracks head movements for precise mouse control.
- **Voice Commands**: Processes offline voice inputs for actions like right-click, left-click, copy, and paste.
- **Wireless Communication**: Uses Bluetooth Low Energy (BLE) for seamless connectivity.
- **Portable Design**: Powered by a compact, rechargeable LiPo battery for enhanced user comfort and mobility.

---

## Hardware Components

1. **Xiao ESP32-C3 Microcontroller**: Handles motion data processing and Bluetooth communication.
2. **MPU6050 Gyroscope Sensor**: Detects head movements to control the cursor.
3. **VC-02 Offline Voice Command Module**: Recognizes and processes voice commands.
4. **LiPo Battery**: Provides portable power to the headband system.
5. **Arduino Pro Micro**: Functions as a Human Interface Device (HID) to execute mouse and keyboard actions.

---

## How It Works

1. **Head Motion Detection**:
   - The gyroscope captures head movements such as pitch, roll, and yaw.
   - The Xiao ESP32-C3 processes this data to move the cursor on the screen.

2. **Voice Command Processing**:
   - The VC-02 module interprets predefined voice commands.
   - Commands trigger specific actions like left-click, right-click, copy, and paste.

3. **Wireless Connectivity**:
   - The system communicates wirelessly with the computer via BLE, ensuring minimal latency.
   - The Arduino Pro Micro translates received signals into executable inputs.

---

## Benefits

- **Accessibility**: Provides an affordable solution for individuals with physical disabilities.
- **Ease of Use**: Simplifies computer interaction without the need for traditional input devices.
- **Low Cost**: Designed with inexpensive components to ensure affordability.

---

## Future Improvements

- Integration of artificial intelligence for advanced gesture and voice recognition.
- Expansion of supported commands to include a broader range of computer functionalities.
- Development of a companion mobile app for system configuration and calibration.
- Miniaturization of hardware components for enhanced portability and comfort.

---

## Contributions

Contributions and suggestions are welcome! If you'd like to improve the system or add new features, feel free to fork the repository and submit a pull request.

---

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use and modify it as per the terms of the license.
