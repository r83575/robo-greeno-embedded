# ESP32 PWM and Servo Projects

## Description
Collection of ESP32 MicroPython projects created on Wokwi.

This session includes:
- RGB LED PWM color control
- Breathing LED effect
- Servo motor angle control
- Integrated multitasking project using `uasyncio`

---

## 1. RGB LED PWM

Control RGB LED colors using PWM signals.

### Features
- Red, Green, Blue color mixing
- PWM brightness control
- Automatic color transitions

### File
`main_RGB.py`

---

## 2. Breathing LED Effect

Create smooth LED fading using PWM.

### Features
- Smooth brightness increase/decrease
- Non-blocking async timing

### File
`main_LED.py`

---

## 3. Servo Motor Control

Control servo motor angle using keypad input.

### Features
- User angle input
- Servo rotation from 0° to 180°
- Input validation

### File
`main_SERVO.py`

---

## 4. Integrated Async Project

Combined project running:
- RGB PWM task
- Breathing LED task
- Servo keypad task

### Uses
- `uasyncio`
- Multiple asynchronous tasks
- PWM control

### File
`main.py`

---

## Components
- ESP32 S3 DevKit
- RGB LED
- Standard LEDs
- Servo Motor
- Keypad
- Resistors
- Wokwi Simulator

---

## Technologies
- MicroPython
- PWM
- uasyncio
- ESP32 GPIO
- Servo Control

---

## Files

```txt
README.md
diagram.json
main.py
main_RGB.py
main_LED.py
main_SERVO.py
```

---

## Run

Open the Wokwi project and run the simulation using MicroPython.

---

## Author

Ruth Cohen