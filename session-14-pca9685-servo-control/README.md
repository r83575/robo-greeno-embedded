# Session 14 - PCA9685 Servo Control

## Description

Raspberry Pi servo control using the PCA9685 16-channel PWM driver over I2C.

This session demonstrates servo calibration, angle control from terminal input, and coordinated movement of multiple servos.

---

## Projects

| Project | Description |
|---|---|
| `servo_sweep.py` | Basic servo sweep between 0°, 90° and 180° |
| `angle_input.py` | Control one servo by entering an angle from the terminal |
| `angle_input_2servos.py` | Control two servos using terminal input |
| `smooth_sequence_2servos.py` | Smooth coordinated movement of two servos |

---

## Features

- Raspberry Pi I2C communication
- PCA9685 16-channel PWM controller
- Servo calibration
- Terminal-based angle control
- Smooth servo movement
- Multiple servo support

---

## Hardware Used

- Raspberry Pi 3 Model B
- PCA9685 PWM Driver
- 2 × MG996R Servo Motors
- External 5V Power Supply

---

## Calibration

```text
Servo model: MG996R
Pulse width range: 500–2500 µs
Channels used: 0, 1
Actual range achieved: 0–180 degrees
```

---

## Files

```text
servo_sweep.py
angle_input.py
angle_input_2servos.py
smooth_sequence_2servos.py
calibration_notes.txt
README.md
```

---

## Technologies Used

- Raspberry Pi
- Python
- I2C
- PCA9685
- PWM
- Adafruit ServoKit

---

## Demo Videos

See the demonstration videos here:

[Demo Videos](./demo_videos.md)

---

## Notes

The original assignment required controlling three servos.

This implementation demonstrates the same concepts using two MG996R servos because only two servos were available.

---

## Author

Ruth Cohen
