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
    ("STAND",   HexLeg.STAND),
    ("LIFT",    HexLeg.LIFT),
    ("EXTEND",  HexLeg.EXTEND),
    ("STAND",   HexLeg.STAND),
    ("RETRACT", HexLeg.RETRACT),
    ("STAND",   HexLeg.STAND),
]

print("Session 15 - One Leg Demo")
print("Press Ctrl+C to stop")

try:
    leg0.move_to(HexLeg.HOME)
    time.sleep(1)

    while True:
        for name, pos in STEP_CYCLE:
            print(name, pos)
            leg0.smooth_to(pos, steps=25, delay=0.012)

        time.sleep(0.3)

except KeyboardInterrupt:
    print("\nStopped")

finally:
    leg0.move_to(HexLeg.HOME)
    print("Returned to HOME")
