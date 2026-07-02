from adafruit_servokit import ServoKit
from time import sleep

kit = ServoKit(channels=16)

for channel in [0,1,2]:

    print("Testing Servo", channel)

    kit.servo[channel].angle = 90
    sleep(1)

    kit.servo[channel].angle = 30
    sleep(1)

    kit.servo[channel].angle = 150
    sleep(1)

    kit.servo[channel].angle = 90
