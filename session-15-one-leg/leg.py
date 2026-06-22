import time

class HexLeg:
    # Angle presets (hip, femur, tibia)
    HOME    = (90, 90, 90)
    STAND   = (90, 60, 120)
    LIFT    = (90, 30, 90)
    EXTEND  = (135, 60, 120)
    RETRACT = (45, 60, 120)

    def __init__(self, kit, hip_ch, fem_ch, tib_ch):
        self.kit = kit
        self.channels = (hip_ch, fem_ch, tib_ch)

        for ch in self.channels:
            self.kit.servo[ch].set_pulse_width_range(500, 2500)

        self.move_to(self.HOME)

    def move_to(self, position, delay=0.01):
        hip, femur, tibia = position

        self.kit.servo[self.channels[0]].angle = hip
        self.kit.servo[self.channels[1]].angle = femur
        self.kit.servo[self.channels[2]].angle = tibia

        time.sleep(delay)

    def smooth_to(self, target, steps=20, delay=0.01):
        current = tuple(
            self.kit.servo[ch].angle
            for ch in self.channels
        )

        for step in range(1, steps + 1):
            t = step / steps

            angles = tuple(
                int(current[i] + (target[i] - current[i]) * t)
                for i in range(3)
            )

            self.move_to(angles, delay)
