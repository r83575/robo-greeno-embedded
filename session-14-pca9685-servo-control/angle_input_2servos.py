from adafruit_servokit import ServoKit

kit = ServoKit(channels=16, address=0x40)

for ch in range(2):
    kit.servo[ch].set_pulse_width_range(500, 2500)

while True:
    user_input = input("Enter 2 angles like 90,45 or q to quit: ")

    if user_input.lower() == "q":
        break

    parts = user_input.split(",")

    if len(parts) != 2:
        print("Error: enter exactly 2 angles, example: 90,45")
        continue

    try:
        angles = [int(x.strip()) for x in parts]
    except ValueError:
        print("Error: angles must be numbers")
        continue

    if any(angle < 0 or angle > 180 for angle in angles):
        print("Error: each angle must be between 0 and 180")
        continue

    kit.servo[0].angle = angles[0]
    kit.servo[1].angle = angles[1]

    print(f"Servo 0 -> {angles[0]}, Servo 1 -> {angles[1]}")
