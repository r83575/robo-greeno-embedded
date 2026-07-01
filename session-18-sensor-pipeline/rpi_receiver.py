import serial
import json
import threading
import time
import os

PORT = "/dev/serial0"
BAUDRATE = 115200

sensor = {
    "temp": 0,
    "hum": 0,
    "dist": 0,
    "battery": 0,
    "seq": 0,
    "ts": 0,
    "pca9685_found": False
}

lock = threading.Lock()
running = True

ser = serial.Serial(
    port=PORT,
    baudrate=BAUDRATE,
    timeout=0.2
)


def recv_thread():
    global running

    while running:
        try:
            raw = ser.readline().decode("utf-8").strip()

            if not raw:
                continue

            try:
                frame = json.loads(raw)
            except json.JSONDecodeError:
                print("Bad JSON:", raw)
                continue

            msg_type = frame.get("type")

            if msg_type == "STATUS":
                data = frame.get("data", {})

                with lock:
                    sensor.update(data)
                    sensor["seq"] = frame.get("seq", 0)
                    sensor["ts"] = frame.get("ts", 0)

            elif msg_type == "ACK":
                print("ACK:", frame)

            elif msg_type == "ERR":
                print("ERR:", frame)

            else:
                print("Unknown frame:", frame)

        except Exception as e:
            print("Receiver error:", e)


def print_dashboard():
    os.system("clear")

    with lock:
        s = dict(sensor)

    print("=== RoboGreeno Live Sensor Dashboard ===")
    print(f"seq          : {s['seq']}")
    print(f"temp         : {s['temp']:.1f} C")
    print(f"hum          : {s['hum']:.1f} %")
    print(f"dist         : {s['dist']:.1f} cm")
    print(f"battery      : {s['battery']} %")
    print(f"ts           : {s['ts']} ms")
    print(f"pca9685_found: {s['pca9685_found']}")


print("Listening for ESP32 STATUS frames...")
print("UART open:", ser.name)

thread = threading.Thread(target=recv_thread, daemon=True)
thread.start()

try:
    while True:
        print_dashboard()
        time.sleep(1)

except KeyboardInterrupt:
    running = False
    time.sleep(0.3)
    ser.close()
    print("UART closed")