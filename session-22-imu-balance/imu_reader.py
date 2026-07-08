import time
import math
from smbus2 import SMBus

MPU_ADDR = 0x68
PWR_MGMT_1 = 0x6B
ACCEL_XOUT_H = 0x3B

bus = SMBus(1)

# Wake up MPU6050
bus.write_byte_data(MPU_ADDR, PWR_MGMT_1, 0)

def read_word(reg):
    high = bus.read_byte_data(MPU_ADDR, reg)
    low = bus.read_byte_data(MPU_ADDR, reg + 1)
    value = (high << 8) + low

    if value >= 0x8000:
        value = -((65535 - value) + 1)

    return value

def read_imu():
    ax = read_word(ACCEL_XOUT_H) / 16384.0
    ay = read_word(ACCEL_XOUT_H + 2) / 16384.0
    az = read_word(ACCEL_XOUT_H + 4) / 16384.0

    gx = read_word(ACCEL_XOUT_H + 8) / 131.0
    gy = read_word(ACCEL_XOUT_H + 10) / 131.0
    gz = read_word(ACCEL_XOUT_H + 12) / 131.0

    return ax, ay, az, gx, gy, gz

def compute_tilt(ax, ay, az):
    pitch = math.degrees(math.atan2(ax, math.sqrt(ay * ay + az * az)))
    roll = math.degrees(math.atan2(ay, math.sqrt(ax * ax + az * az)))
    return pitch, roll

while True:
    ax, ay, az, gx, gy, gz = read_imu()
    pitch, roll = compute_tilt(ax, ay, az)

    print(
        f"Accel: x={ax:.2f}, y={ay:.2f}, z={az:.2f} | "
        f"Gyro: x={gx:.2f}, y={gy:.2f}, z={gz:.2f} | "
        f"Pitch={pitch:.1f}°, Roll={roll:.1f}°"
    )

    time.sleep(0.5)
