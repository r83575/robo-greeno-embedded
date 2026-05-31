# Smart Robot Status Monitor

## Description
ESP32-S3 telemetry system simulation using Wokwi and PlatformIO.

The project monitors:
- Temperature
- Humidity
- Distance to obstacles

The system activates a DANGER LED when an object is detected closer than 20 cm.

Sensor data is printed to the Serial Monitor every 2 seconds.

---

## Features
- DHT22 temperature and humidity sensor
- HC-SR04 ultrasonic distance sensor
- Danger warning LED
- Serial monitor telemetry output
- ESP32-S3 simulation in Wokwi

---

## Components
- ESP32-S3 DevKit
- DHT22 Sensor
- HC-SR04 Ultrasonic Sensor
- LED
- Resistor
- Wokwi Simulator

---

## Pin Connections

| Component | GPIO |
|---|---|
| DHT22 Data | GPIO 15 |
| HC-SR04 Trigger | GPIO 5 |
| HC-SR04 Echo | GPIO 6 |
| Danger LED | GPIO 7 |

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
2. Start Wokwi simulation
3. Open Serial Monitor
4. Observe sensor data and LED behavior

---

## Author

Ruth Cohen