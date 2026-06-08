# Session 10 - BLE Distance Monitor

## Description
ESP32 BLE project that measures distance using an HC-SR04 ultrasonic sensor and publishes the value through a BLE characteristic.

The distance value can be viewed from a BLE client application such as Bluetooth LE Explorer.

---

## Features
- BLE advertising using ESP32
- Custom BLE service and characteristic
- HC-SR04 distance measurement
- FreeRTOS tasks
- Serial Monitor output
- BLE characteristic update with distance value

---

## Components
- ESP32 Dev Module
- HC-SR04 Ultrasonic Sensor
- Bluetooth LE Explorer

---

## Pin Connections

| Component | ESP32 Pin |
|---|---|
| HC-SR04 TRIG | GPIO 4 |
| HC-SR04 ECHO | GPIO 5 |

---

## BLE Configuration

| Item | Value |
|---|---|
| Device Name | `RoboGreeno-ESP32` |
| Service UUID | `4fafc201-1fb5-459e-8fcc-c5c9c331914b` |
| Characteristic UUID | `beb5483e-36e1-4688-b7f5-ea07361b26a8` |

---

## FreeRTOS Tasks

| Task | Description |
|---|---|
| `sensorTask` | Reads distance from HC-SR04 every 1 second |
| `bleTask` | Updates BLE characteristic every 2 seconds |
| `displayTask` | Prints distance to Serial Monitor every 1 second |

---

## Files

```txt
ESP32_BLE_Advertising.ino
Bluetooth_With_ESP32_Distance.png
README.md
```

---

## Result

The distance value is printed in the Serial Monitor and exposed through BLE as:

```txt
D:<distance>cm
```

---

## Author

Ruth Cohen