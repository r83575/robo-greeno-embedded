from machine import Pin, PWM
from time import sleep

red = PWM(Pin(4), freq=1000)
green = PWM(Pin(5), freq=1000)
blue = PWM(Pin(6), freq=1000)

def set_color(r, g, b):
    red.duty_u16(r)
    green.duty_u16(g)
    blue.duty_u16(b)

while True:
    set_color(65535, 0, 0)
    sleep(1)

    set_color(0, 65535, 0)
    sleep(1)

    set_color(0, 0, 65535)
    sleep(1)

    set_color(65535, 65535, 0)
    sleep(1)

    set_color(0, 65535, 65535)
    sleep(1)

    set_color(65535, 0, 65535)
    sleep(1)