from time import sleep
from adafruit_servokit import ServoKit
from ik_solver import leg_ik, ik_to_servo

kit = ServoKit(channels=16)

HIP_CHANNEL = 0
FEMUR_CHANNEL = 1
TIBIA_CHANNEL = 2

test_points = [
    ("HOME", 12.5, 0, -8),
    ("STAND", 12.5, 0, -10),
    ("LIFT", 12.5, 0, -4),
    ("EXTEND", 17, 0, -8),
    ("RETRACT", 8.5, 0, -8),
]

for name, x, y, z in test_points:
    angles = leg_ik(x, y, z)

    if angles is None:
        print(f"{name}: unreachable")
        continue

    hip, femur, tibia = ik_to_servo(*angles)

    print(f"{name}: servo angles = {hip:.1f}, {femur:.1f}, {tibia:.1f}")

    kit.servo[HIP_CHANNEL].angle = hip
    kit.servo[FEMUR_CHANNEL].angle = femur
    kit.servo[TIBIA_CHANNEL].angle = tibia

    sleep(1)

print("IK hardware test completed.")
