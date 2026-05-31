import uasyncio as asyncio
from machine import Pin, PWM

# ---------------- RGB ----------------
red = PWM(Pin(4), freq=1000)
green = PWM(Pin(5), freq=1000)
blue = PWM(Pin(6), freq=1000)

def set_color(r, g, b):
    red.duty_u16(r)
    green.duty_u16(g)
    blue.duty_u16(b)

# ---------------- LED ----------------
led = PWM(Pin(7), freq=1000)

breathing_enabled = True

# ---------------- SERVO ----------------
servo = PWM(Pin(9), freq=50)

def set_angle(angle):
    min_duty = 1638
    max_duty = 8192
    duty = int(min_duty + (angle / 180) * (max_duty - min_duty))
    servo.duty_u16(duty)

# ---------------- KEYPAD ----------------

rows = [Pin(10, Pin.OUT), Pin(11, Pin.OUT), Pin(12, Pin.OUT), Pin(13, Pin.OUT)]
cols = [Pin(14, Pin.IN, Pin.PULL_DOWN),
         Pin(15, Pin.IN, Pin.PULL_DOWN),
         Pin(16, Pin.IN, Pin.PULL_DOWN),
         Pin(17, Pin.IN, Pin.PULL_DOWN)]

keys = [
    ["1","2","3","A"],
    ["4","5","6","B"],
    ["7","8","9","C"],
    ["*","0","#","D"]
]

def scan_keypad():
    for i, row in enumerate(rows):
        row.value(1)
        for j, col in enumerate(cols):
            if col.value():
                row.value(0)
                return keys[i][j]
        row.value(0)
    return None

# ---------------- TASKS ----------------

angle = 90

async def keypad_task():
    global angle, breathing_enabled

    while True:
        key = scan_keypad()

        if key:
            print("Key:", key)

            if key.isdigit():
                angle = int(key) * 18
                set_angle(angle)

            elif key == "A":
                set_color(65535, 0, 0)

            elif key == "B":
                set_color(0, 65535, 0)

            elif key == "C":
                set_color(0, 0, 65535)

            elif key == "D":
                breathing_enabled = not breathing_enabled

        await asyncio.sleep_ms(100)

async def rgb_task():
    while True:
        await asyncio.sleep(1)

async def led_task():
    global breathing_enabled

    while True:
        if breathing_enabled:
            for d in range(0, 65535, 500):
                led.duty_u16(d)
                await asyncio.sleep_ms(10)

            for d in range(65535, 0, -500):
                led.duty_u16(d)
                await asyncio.sleep_ms(10)
        else:
            led.duty_u16(0)
            await asyncio.sleep(0.1)

async def main():
    asyncio.create_task(keypad_task())
    asyncio.create_task(led_task())
    asyncio.create_task(rgb_task())

    while True:
        await asyncio.sleep(1)

asyncio.run(main())