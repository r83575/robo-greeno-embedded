# Session 13 - Raspberry Pi GPIO with Python

## Description

Introduction to Raspberry Pi GPIO programming using Python.

This session covers digital outputs, digital inputs, PWM, LED control, push buttons and GPIO logging on real Raspberry Pi hardware.

---

## Projects

| Project | Description |
|---|---|
| `blink.py` | Basic LED blink using GPIO17 |
| `led_button_controller.py` | Two LEDs, two buttons, PWM brightness control and bonus fade mode |
| `traffic_light.py` | Traffic light simulation with timestamp logging and log file generation |

---

## Features

- Raspberry Pi GPIO programming
- Digital output
- Digital input with internal pull-up
- LED blinking
- Push button control
- PWM LED brightness control
- Traffic light simulation
- Timestamp logging
- Log file generation
- KeyboardInterrupt handling
- GPIO cleanup

---

## Part 1 - LED Blink

Simple LED blink using GPIO17.

Features:
- LED ON/OFF every 0.5 seconds
- Safe GPIO cleanup

---

## Part 2 - LED Button Controller

Two LEDs and two buttons.

### Functions

| Button | Action |
|---|---|
| GPIO27 | Toggle LED1 |
| GPIO5 | Increase LED2 brightness by 10% |

### Bonus

When both buttons are pressed simultaneously:

- Both LEDs perform synchronized PWM fade.

---

## Part 3 - Traffic Light

Traffic light sequence:

- Red → 3 seconds
- Yellow → 1 second
- Green → 3 seconds
- Yellow → 1 second

Each state is printed with a timestamp.

Example:

```txt
2026-06-22 12:05:03 | RED
2026-06-22 12:05:06 | YELLOW
2026-06-22 12:05:07 | GREEN
```

---

## Hardware Used

- Raspberry Pi 3 Model B
- 3 LEDs
- 2 Push Buttons
- 220Ω–330Ω Resistors
- Breadboard
- Jumper Wires

---

## Pin Connections

| Component | BCM GPIO |
|---|---|
| LED1 | GPIO17 |
| LED2 | GPIO22 |
| Button1 | GPIO27 |
| Button2 | GPIO5 |

Traffic Light:

| LED | BCM GPIO |
|---|---|
| Red | GPIO17 |
| Yellow | GPIO27 |
| Green | GPIO22 |

---

## Files

```txt
blink.py
led_button_controller.py
traffic_light.py
README.md
```

---

## Technologies Used

- Raspberry Pi 3
- Raspberry Pi OS
- Python 3
- RPi.GPIO
- GPIO
- PWM

---

## Author

Ruth Cohen

Mentor: Dosithee Miet
