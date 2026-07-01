# Session 18 - Sensor Pipeline

## Description

ESP32-C5 sensor monitoring project using UART communication with a Raspberry Pi.

This session extends the JSON protocol introduced in Session 17 by collecting real-time sensor data and periodically transmitting STATUS frames over UART.

The ESP32 reads environmental and distance data while continuing to receive robot joint commands through the same communication channel.

---

## Features

- ESP32 ↔ Raspberry Pi JSON communication
- Periodic STATUS messages
- DHT22 temperature and humidity monitoring
- HC-SR04 distance measurement
- FreeRTOS multitasking
- UART command handling
- JSON serialization
- Live sensor dashboard on Raspberry Pi
- PCA9685 monitoring
- Serial Monitor debugging

---

## Hardware Used

- ESP32-C5 DevKitC-1
- Raspberry Pi 3B
- DHT22 Sensor
- HC-SR04 Ultrasonic Sensor
- PCA9685 PWM Driver
- Servo Motors
- USB Cable
- Jumper Wires

---

## UART Wiring

| Raspberry Pi | ESP32-C5 |
|---|---|
| GPIO14 (TX) | GPIO4 (RX) |
| GPIO15 (RX) | GPIO5 (TX) |
| GND | GND |

UART Configuration

- Baud Rate: 115200
- Data Bits: 8
- Parity: None
- Stop Bits: 1 (8N1)

---

## I2C Wiring

| ESP32-C5 | PCA9685 |
|---|---|
| GPIO2 | SDA |
| GPIO3 | SCL |
| 3.3V | VCC |
| GND | GND |

---

## Sensor Connections

### DHT22

| DHT22 | ESP32-C5 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO6 |

### HC-SR04

| HC-SR04 | ESP32-C5 |
|---|---|
| TRIG | GPIO7 |
| ECHO | GPIO8 |
| VCC | 5V |
| GND | GND |

---

## STATUS Frame

Example:

```json
{
  "type": "STATUS",
  "seq": 1001,
  "ts": 12345,
  "data": {
    "temp": 24.5,
    "hum": 58.3,
    "dist": 31.4,
    "battery": 85,
    "uptime": 12345,
    "active_servos": 16,
    "total_joints": 18,
    "pca9685_found": true
  }
}
```

---

## Communication Flow

```text
DHT22
     |
HC-SR04
     |
     v
ESP32-C5
     |
JSON STATUS
     |
UART
     |
Raspberry Pi
     |
Live Dashboard
```

---

## FreeRTOS Tasks

| Task | Description |
|---|---|
| `sensorTask` | Reads DHT22 and HC-SR04 sensors |
| `statusTask` | Sends STATUS frame every 2 seconds |
| `uartTask` | Receives CMD frames and sends ACK/ERR |
| `i2cTask` | Monitors PCA9685 availability |

---

## Files

| File | Description |
|---|---|
| `sensor_pipeline.ino` | ESP32 sensor acquisition, UART protocol and STATUS transmission |
| `rpi_receiver.py` | Raspberry Pi live STATUS receiver and dashboard |
| `README.md` | Session documentation |

---

## Technologies Used

- ESP32-C5
- Raspberry Pi
- Arduino Framework
- FreeRTOS
- UART
- JSON
- ArduinoJson
- DHT22
- HC-SR04
- I2C
- PCA9685
- pySerial

---

## Author

Ruth Cohen

Mentor: Dosithee Miet