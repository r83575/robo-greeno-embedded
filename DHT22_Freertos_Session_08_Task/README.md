# Session 8 - DHT22 FreeRTOS Project

## Description
ESP32 FreeRTOS project using a DHT22 sensor for temperature and humidity monitoring in Wokwi.

The system reads sensor data every 2 seconds and prints formatted output to the Serial Monitor.

---

## Features
- Temperature and humidity monitoring using `DHT22`
- FreeRTOS task using `xTaskCreate()`
- Non-blocking delay using `vTaskDelay()`
- Invalid sensor reading detection using `isnan()`
- Heat index calculation
- Alert LED for high temperature or humidity
- Serial Monitor formatted output

---

## Components
- ESP32 DevKit
- DHT22 Sensor
- 10kΩ Pull-up Resistor
- Red LED
- 220Ω Resistor
- Wokwi Simulator

---

## Pin Connections

| Component | ESP32 Pin |
|---|---|
| DHT22 DATA | GPIO 4 |
| Alert LED | GPIO 2 |

---

## Files

```txt
sketch.ino
diagram.json
libraries.txt
wokwi-project.txt
README.md
```

---

## FreeRTOS Functions Used
- `xTaskCreate()`
- `vTaskDelay()`

---

## Run
1. Open the project in Wokwi
2. Start the simulation
3. Open the Serial Monitor
4. Observe temperature and humidity readings

---

## Author
Ruth Cohen