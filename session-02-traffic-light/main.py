from machine import Pin
from time import sleep

red = Pin(2, Pin.OUT)
yellow = Pin(4, Pin.OUT)
green = Pin(5, Pin.OUT)

# Reset all LEDs
red.off()
yellow.off()
green.off()

while True:
    red.on()
    yellow.off()
    green.off()
    sleep(3)

    red.off()
    yellow.on()
    green.off()
    sleep(1)

    red.off()
    yellow.off()
    green.on()
    sleep(3)

    green.off()
    yellow.on()
    sleep(1)

    yellow.off()