# Session 9 - Arduino IoT Cloud

## Description
ESP32 Arduino IoT Cloud projects for remote device control and real-time sensor monitoring.

This session demonstrates communication between ESP32 devices and the Arduino IoT Cloud using WiFi and MQTT/TLS communication.

---

## Projects

| Project | Description | Link |
|---|---|---|
| Led With Switch | Remote LED control from Arduino IoT Cloud dashboard | [Open](./led-with-switch/README.md) |
| Distance Sensor | Real-time HC-SR04 monitoring with Arduino IoT Cloud | [Open](./distance-sensor/README.md) |

---

## Features

- ESP32 WiFi connection
- Arduino IoT Cloud integration
- MQTT/TLS communication
- Remote LED control
- Real-time dashboard monitoring
- Distance sensor telemetry
- Cloud variables and callbacks

---

## How It Works

### LED Remote Control

The ESP32 connects to Arduino IoT Cloud using WiFi.

A dashboard button updates a cloud variable.

When the variable changes:
- LED ON → ESP32 turns the LED ON
- LED OFF → ESP32 turns the LED OFF

This allows remote device control directly from the Arduino Cloud dashboard.

---

### Distance Sensor Monitoring

The ESP32 reads distance values from the HC-SR04 sensor.

The measured values are uploaded to Arduino IoT Cloud and displayed on the dashboard in real time.

This demonstrates:
- IoT telemetry
- Cloud-based monitoring
- Real-time sensor streaming
- ESP32 cloud communication

---

## Communication Flow

```txt
Arduino Cloud Dashboard
        ↓
WiFi + MQTT/TLS
        ↓
ESP32 Device
        ↓
LED / Distance Sensor
```

---

## Folder Structure

```txt
session-09-arduino-iot-cloud/
├── led-with-switch/
│   └── README.md
│
├── distance-sensor/
│   └── README.md
│
└── README.md
```

---

## Technologies Used

- ESP32
- Arduino IoT Cloud
- MQTT/TLS
- WiFi
- HC-SR04
- Cloud Dashboard
- Arduino Framework

---

## Author

Ruth Cohen