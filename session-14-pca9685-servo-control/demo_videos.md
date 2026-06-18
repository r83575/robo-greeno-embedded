# Session 14 - Demo Videos

This document contains demonstration videos for each Python script included in Session 14.

---

## Servo Sweep

Run:

```bash
python servo_sweep.py
```

This script continuously moves the servo between **0°**, **90°**, and **180°**.

The following video demonstrates the execution of this script.

**Demo Video:**

[servo_sweep.MP4](./videos/servo_sweep.MP4)

---

## Single Servo Angle Input

Run:

```bash
python angle_input.py
```

Example:

```text
Enter angle 0-180, or q to quit: 90
Servo moved to 90

Enter angle 0-180, or q to quit: 135
Servo moved to 135

Enter angle 0-180, or q to quit: 180
Servo moved to 180
```

The following video demonstrates the execution shown in the example above.

**Demo Video:**

[angle_input.MP4](./videos/angle_input.MP4)

---

## Two Servo Angle Input

Run:

```bash
python angle_input_2servos.py
```

Example:

```text
Enter 2 angles like 90,45 or q to quit: 45,90
Servo 0 -> 45, Servo 1 -> 90
```

The following video demonstrates the execution shown in the example above.

**Demo Video:**

[angle_input_2servos.MP4](./videos/angle_input_2servos.MP4)

---

## Smooth Two Servo Sequence

Run:

```bash
python smooth_sequence_2servos.py
```

This script smoothly moves two servos from the center position to a predefined walking position and then returns them to the center.

The following video demonstrates the movement sequence implemented by this script.

**Demo Video:**

[smooth_sequence_2servos.MP4](./videos/smooth_sequence_2servos.MP4)