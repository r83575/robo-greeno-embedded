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

commands = {
    "w": ("LIFT", HexLeg.LIFT),
    "s": ("STAND", HexLeg.STAND),
    "a": ("RETRACT", HexLeg.RETRACT),
    "d": ("EXTEND", HexLeg.EXTEND),
    "h": ("HOME", HexLeg.HOME),
    "c": ("CROUCH", HexLeg.CROUCH),
    "f": ("FORWARD_LOW", HexLeg.FORWARD_LOW),
    "u": ("HIGH_LIFT", HexLeg.HIGH_LIFT),
}

print("=== Keyboard Control ===")
print("W = LIFT")
print("S = STAND")
print("A = RETRACT")
print("D = EXTEND")
print("H = HOME")
print("C = CROUCH")
print("F = FORWARD_LOW")
print("U = HIGH_LIFT")
print("Q = Quit")

leg0.move_to(HexLeg.HOME)

while True:
    key = input("\nCommand: ").strip().lower()

    if key == "q":
        break

    if key in commands:
        name, position = commands[key]
        print(f"Moving to {name}")
        leg0.smooth_to(position, steps=25, delay=0.012)
    else:
        print("Unknown command.")

leg0.move_to(HexLeg.HOME)
print("Returned to HOME.")
