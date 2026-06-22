import RPi.GPIO as GPIO
import time
from datetime import datetime

RED = 17
YELLOW = 27
GREEN = 22

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

GPIO.setup(RED, GPIO.OUT)
GPIO.setup(YELLOW, GPIO.OUT)
GPIO.setup(GREEN, GPIO.OUT)

def log_state(state):
    now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    message = f"{now} | {state}"

    print(message)

    with open("traffic_log.txt", "a") as log_file:
        log_file.write(message + "\n")

def all_off():
    GPIO.output(RED, GPIO.LOW)
    GPIO.output(YELLOW, GPIO.LOW)
    GPIO.output(GREEN, GPIO.LOW)

try:
    all_off()

    while True:
        GPIO.output(RED, GPIO.HIGH)
        GPIO.output(YELLOW, GPIO.LOW)
        GPIO.output(GREEN, GPIO.LOW)
        log_state("RED")
        time.sleep(3)

        GPIO.output(RED, GPIO.LOW)
        GPIO.output(YELLOW, GPIO.HIGH)
        GPIO.output(GREEN, GPIO.LOW)
        log_state("YELLOW")
        time.sleep(1)

        GPIO.output(RED, GPIO.LOW)
        GPIO.output(YELLOW, GPIO.LOW)
        GPIO.output(GREEN, GPIO.HIGH)
        log_state("GREEN")
        time.sleep(3)

        GPIO.output(GREEN, GPIO.LOW)
        GPIO.output(YELLOW, GPIO.HIGH)
        log_state("YELLOW")
        time.sleep(1)

except KeyboardInterrupt:
    all_off()
    GPIO.cleanup()
