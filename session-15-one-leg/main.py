#!/usr/bin/env python3

import time
from adafruit_servokit import ServoKit
from leg import HexLeg

kit = ServoKit(channels=16)

leg0 = HexLeg(
    kit,
    hip_ch=0,
    fem_ch=1,
    tib_ch=2
)

STEP_CYCLE = [
    ("STAND",       HexLeg.STAND),
    ("LIFT",        HexLeg.LIFT),
    ("EXTEND",      HexLeg.EXTEND),
    ("STAND",       HexLeg.STAND),
    ("RETRACT",     HexLeg.RETRACT),
    ("STAND",       HexLeg.STAND),

    # Custom positions
    ("CROUCH",      HexLeg.CROUCH),
    ("HIGH_LIFT",   HexLeg.HIGH_LIFT),
    ("FORWARD_LOW", HexLeg.FORWARD_LOW),
    ("HOME",        HexLeg.HOME),
]

STEP_COUNT = 10

print("Session 15 - One Leg Demo")
print(f"Running {STEP_COUNT} step cycles...")

try:
    # Safe start
    leg0.move_to(HexLeg.HOME)
    time.sleep(1)

    start_time = time.perf_counter()

    for rep in range(1, STEP_COUNT + 1):
        print(f"\n--- Step cycle #{rep} ---")

        for name, pos in STEP_CYCLE:
            print(f"  -> {name}: {pos}")
            leg0.smooth_to(pos, steps=25, delay=0.012)

        time.sleep(0.3)

    end_time = time.perf_counter()

    total_time = end_time - start_time
    avg_cycle = total_time / STEP_COUNT
    cycles_per_minute = 60 / avg_cycle

    print("\nPerformance Results")
    print(f"Total time: {total_time:.2f} seconds")
    print(f"Average cycle time: {avg_cycle:.2f} seconds")
    print(f"Cycles per minute: {cycles_per_minute:.2f}")

except KeyboardInterrupt:
    print("\nStopped by user.")

finally:
    leg0.move_to(HexLeg.HOME)
    print("Returned to HOME. Safe to power off.")
