# Session 15 - One Leg Manual Control

## Description

Raspberry Pi project for controlling a single hexapod robot leg using three servo motors and the PCA9685 PWM controller.

This session introduces the mechanical structure of a hexapod leg (Hip, Femur and Tibia), reusable object-oriented control with a `HexLeg` class, named leg positions and smooth servo interpolation.

---

## Features

- Raspberry Pi servo control
- PCA9685 PWM controller
- Three-servo hexapod leg
- Object-oriented `HexLeg` class
- Named leg positions
- Smooth interpolation between positions
- Continuous step cycle
- Custom leg positions
- Keyboard-controlled leg movement
- Step cycle performance measurement
- Browser-based leg simulator

---

## Hardware Used

- Raspberry Pi 3B
- PCA9685 Servo Driver
- 3 × Servo Motors (SG90 / MG996R)
- External 5V Power Supply

---

## Servo Channels

| Joint | PCA9685 Channel |
|---|---|
| Hip (Coxa) | 0 |
| Femur | 1 |
| Tibia | 2 |

---

## Named Positions

| Position | Angles (Hip, Femur, Tibia) |
|---|---|
| HOME | (90, 90, 90) |
| STAND | (90, 60, 120) |
| LIFT | (90, 30, 90) |
| EXTEND | (135, 60, 120) |
| RETRACT | (45, 60, 120) |
| CROUCH | (90, 110, 140) |
| HIGH_LIFT | (90, 20, 80) |
| FORWARD_LOW | (130, 80, 130) |
---

## Files

| File | Description |
|---|---|
| `leg.py` | `HexLeg` class with reusable servo control methods |
| `main.py` | Demonstration of one-leg movement and continuous step cycle |
| `keyboard_control.py` | Interactive keyboard control for manual leg movement |
| `custom_positions.md` | Documentation of additional custom leg positions |
| `hexleg_simulator.html` | Interactive browser simulator for visualizing leg positions and commands |
| `README.md` | Session documentation |

---

## Technologies Used

- Python
- Raspberry Pi
- PCA9685
- ServoKit
- I2C
- HTML
- CSS
- JavaScript
- Object-Oriented Programming (OOP)

---

## Author

Ruth Cohen

Mentor: Dosithee Miet
