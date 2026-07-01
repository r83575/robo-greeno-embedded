import serial
import time
import json
import threading
import queue

PORT = "/dev/serial0"
BAUDRATE = 115200
ACK_TIMEOUT = 0.7
STATUS_TIMEOUT = 5.0

ser = serial.Serial(
    port=PORT,
    baudrate=BAUDRATE,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout=0.2
)

seq = 0
ack_queue = queue.Queue()
last_status_time = time.monotonic()
running = True


def build_cmd(seq_num, angles, speed=25):
    return {
        "type": "CMD",
        "seq": seq_num,
        "ts": int(time.monotonic() * 1000),
        "data": {
            "joints": angles,
            "speed": speed
        }
    }


def send_frame(frame):
    msg = json.dumps(frame) + "\n"
    ser.write(msg.encode("utf-8"))
    print("Sent:", msg.strip())


def reader_thread():
    global last_status_time

    while running:
        try:
            raw = ser.readline().decode("utf-8").strip()
            if not raw:
                continue

            print("ESP32:", raw)

            try:
                reply = json.loads(raw)
            except json.JSONDecodeError:
                print("Bad JSON from ESP32")
                continue

            msg_type = reply.get("type")

            if msg_type == "STATUS":
                last_status_time = time.monotonic()
                print("STATUS:", reply.get("data"))

            elif msg_type == "ACK":
                ack_queue.put(reply)

            elif msg_type == "ERR":
                print("ERR:", reply.get("data"))
                ack_queue.put(reply)

            else:
                print("Unknown message:", reply)

        except Exception as e:
            print("Reader error:", e)


def wait_for_ack(expected_seq):
    start = time.monotonic()

    while time.monotonic() - start < ACK_TIMEOUT:
        try:
            reply = ack_queue.get(timeout=0.1)
        except queue.Empty:
            continue

        if reply.get("type") == "ACK" and reply.get("seq") == expected_seq:
            print("ACK matched:", expected_seq)
            return True

        if reply.get("type") == "ERR":
            print("Command failed:", reply.get("data"))
            return False

    return False


def health_check():
    if time.monotonic() - last_status_time > STATUS_TIMEOUT:
        print("WARNING: ESP32 silent - no STATUS for 5 seconds")


def send_command_with_retry(angles, speed=25):
    if len(angles) != 18:
        raise ValueError("CMD must contain exactly 18 joint angles")

    if not all(0 <= a <= 180 for a in angles):
        raise ValueError("All angles must be between 0 and 180")
        
    global seq

    seq = (seq + 1) % 65536
    frame = build_cmd(seq, angles, speed)

    for attempt in range(1, 4):
        print(f"Attempt {attempt} for seq {seq}")
        send_frame(frame)

        if wait_for_ack(seq):
            return True

        print("No valid ACK, retrying...")

    print("FAILED: no ACK after 3 attempts")
    return False


STAND = [
    90, 60, 120, 90, 60, 120,
    90, 60, 120, 90, 60, 120,
    90, 60, 120, 90, 60, 120
]

POSE_1 = [
    90, 70, 110, 90, 60, 120,
    90, 70, 110, 90, 60, 120,
    90, 70, 110, 90, 60, 120
]

POSE_2 = [
    90, 50, 130, 90, 60, 120,
    90, 50, 130, 90, 60, 120,
    90, 50, 130, 90, 60, 120
]


print("UART open:", ser.name)
time.sleep(2)

thread = threading.Thread(target=reader_thread, daemon=True)
thread.start()

try:
    while True:
        send_command_with_retry(STAND, speed=25)
        time.sleep(1)
        health_check()

        send_command_with_retry(POSE_1, speed=25)
        time.sleep(1)
        health_check()

        send_command_with_retry(POSE_2, speed=25)
        time.sleep(1)
        health_check()

except KeyboardInterrupt:
    running = False
    time.sleep(0.3)
    ser.close()
    print("UART closed")
