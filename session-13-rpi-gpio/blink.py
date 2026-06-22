import RPi.GPIO as GPIO
import time

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)

try:
    while True:
        print("LED ON")
        GPIO.output(17, GPIO.HIGH)
        time.sleep(0.5)

        print("LED OFF")
        GPIO.output(17, GPIO.LOW)
        time.sleep(0.5)

except KeyboardInterrupt:
    GPIO.cleanup()
