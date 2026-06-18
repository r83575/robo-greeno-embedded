import time
from adafruit_servokit import ServoKit

kit = ServoKit(channels=16)

for ch in range(2):
    kit.servo[ch].set_pulse_width_range(500, 2500)

while True:
    kit.servo[0].angle = 0
    kit.servo[1].angle = 180
    time.sleep(1)

    kit.servo[0].angle = 90
    kit.servo[1].angle = 90
    time.sleep(1)

    kit.servo[0].angle = 180
    kit.servo[1].angle = 0
    time.sleep(1)
