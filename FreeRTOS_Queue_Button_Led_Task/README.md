# FreeRTOS Queue Button LED Project

## Description
ESP32 FreeRTOS project using queues for communication between tasks.

The system reads a push button state and sends values through a FreeRTOS queue to control an LED.

---

## Features
- FreeRTOS multitasking
- Queue communication between tasks
- Button state monitoring
- LED ON/OFF control
- Serial Monitor debugging output

---

## FreeRTOS Functions Used
- `xTaskCreate()`
- `xQueueCreate()`
- `xQueueSend()`
- `xQueueReceive()`
- `vTaskDelay()`

---

## Tasks

### Button Task
- Reads push button state
- Sends:
  - `1` when button is pressed
  - `0` when button is released
- Prints sent values to Serial Monitor

### LED Task
- Receives values from the queue
- Turns LED:
  - ON when receiving `1`
  - OFF when receiving `0`
- Prints received values to Serial Monitor

---

## Components
- ESP32 DOIT DevKit V1
- Push Button
- LED
- 220Ω Resistor
- Wokwi Simulator

---

## Files

```txt
src/main.cpp
diagram.json
platformio.ini
wokwi.toml
.gitignore
README.md
```

---

## Run

1. Open the project in VS Code
2. Start Wokwi simulation
3. Open Serial Monitor
4. Press and release the button
5. Observe queue messages and LED behavior

---

## Author

Ruth Cohen