import time
from adafruit_servokit import ServoKit

kit = ServoKit(channels=16, address=0x40)

for ch in range(2):
    kit.servo[ch].set_pulse_width_range(500, 2500)

def move_smooth(ch, start, end, delay=0.02):
    step = 1 if end > start else -1
    for angle in range(start, end + step, step):
        kit.servo[ch].angle = angle
        time.sleep(delay)

while True:
    kit.servo[0].angle = 90
    kit.servo[1].angle = 90
    time.sleep(1)

    move_smooth(0, 90, 45)
    move_smooth(1, 90, 135)
    time.sleep(1)

    move_smooth(0, 45, 90)
    move_smooth(1, 135, 90)
    time.sleep(1)
