from adafruit_servokit import ServoKit
from time import sleep

kit = ServoKit(channels=16)

HIP_CHANNEL = 0
FEMUR_CHANNEL = 1
TIBIA_CHANNEL = 2


def move_leg(hip, femur, tibia):
    print(f"Moving: hip={hip}, femur={femur}, tibia={tibia}")
    kit.servo[HIP_CHANNEL].angle = hip
    kit.servo[FEMUR_CHANNEL].angle = femur
    kit.servo[TIBIA_CHANNEL].angle = tibia
    sleep(1)


# Center position
move_leg(90, 90, 90)

# Small movement tests
move_leg(90, 70, 60)
move_leg(100, 80, 70)
move_leg(80, 80, 70)

# Return to center
move_leg(90, 90, 90)

print("Hardware test completed.")
