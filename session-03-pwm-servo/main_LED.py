from machine import Pin, PWM
from time import sleep_ms

led = PWM(Pin(7), freq=1000)

while True:

    # עליה הדרגתית
    for duty in range(0, 65535, 500):
        led.duty_u16(duty)
        sleep_ms(10)

    # ירידה הדרגתית
    for duty in range(65535, 0, -500):
        led.duty_u16(duty)
        sleep_ms(10)