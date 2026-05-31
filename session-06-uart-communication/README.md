# Session 6 - UART Communication

## Description
ESP32 UART communication project using Wokwi and PlatformIO.

The project demonstrates UART serial communication using TX and RX pins on the ESP32.

A loopback connection is used between GPIO17 (TX) and GPIO16 (RX) to test UART data transmission.

---

## Features
- UART serial communication
- TX/RX loopback testing
- Serial Monitor output
- ESP32 UART configuration
- Wokwi simulation

---

## Components
- ESP32-S3 DevKit
- UART TX/RX connection
- Wokwi Simulator

---

## UART Pins

| Function | GPIO |
|---|---|
| TX | GPIO 17 |
| RX | GPIO 16 |

---

## Files

```txt
main.cpp
diagram.json
platformio.ini
wokwi.toml
README.md
```

---

## Run

1. Open the project in VS Code
2. Start the Wokwi simulation
3. Open the Serial Monitor
4. Observe UART communication output

---

## Author

Ruth Cohen