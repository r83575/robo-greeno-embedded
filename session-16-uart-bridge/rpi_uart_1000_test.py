import serial
import time

ser = serial.Serial(
    port="/dev/serial0",
    baudrate=115200,
    timeout=1
)

angles = [90, 60, 120, 90, 60, 120,
          90, 60, 120, 90, 60, 120,
          90, 60, 120, 90, 60, 120]

msg = ",".join(map(str, angles)) + "\n"

success = 0
failed = 0

time.sleep(2)

for i in range(1000):
    ser.write(msg.encode("utf-8"))

    response = ser.readline().decode("utf-8", errors="replace").strip()

    if response == '{"received":18,"status":"OK"}':
        success += 1
    else:
        failed += 1
        print(f"Failed #{i}: {response}")

    time.sleep(0.01)

ser.close()

print("Test complete")
print("Success:", success)
print("Failed:", failed)
print("Accuracy:", (success / 1000) * 100, "%")
