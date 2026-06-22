import RPi.GPIO as GPIO
import time

LED1 = 17
LED2 = 22
BUTTON1 = 27
BUTTON2 = 5

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

GPIO.setup(LED1, GPIO.OUT)
GPIO.setup(LED2, GPIO.OUT)

GPIO.setup(BUTTON1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(BUTTON2, GPIO.IN, pull_up_down=GPIO.PUD_UP)

pwm1 = GPIO.PWM(LED1, 1000)
pwm2 = GPIO.PWM(LED2, 1000)

pwm1.start(0)
pwm2.start(0)

led1_state = False
brightness = 0

fade_value = 0
fade_direction = 5
bonus_active = False

last_button1 = GPIO.HIGH
last_button2 = GPIO.HIGH

try:
    while True:
        b1 = GPIO.input(BUTTON1)
        b2 = GPIO.input(BUTTON2)

        # BONUS: both buttons pressed
        if b1 == GPIO.LOW and b2 == GPIO.LOW:
            bonus_active = True

            fade_value += fade_direction

            if fade_value >= 100:
                fade_value = 100
                fade_direction = -5
            elif fade_value <= 0:
                fade_value = 0
                fade_direction = 5

            pwm1.ChangeDutyCycle(fade_value)
            pwm2.ChangeDutyCycle(fade_value)

            print(f"BONUS FADE: {fade_value}%")

            last_button1 = b1
            last_button2 = b2

            time.sleep(0.05)
            continue

        # after bonus, wait until both buttons are released
        if bonus_active:
            if b1 == GPIO.HIGH and b2 == GPIO.HIGH:
                bonus_active = False
                last_button1 = GPIO.HIGH
                last_button2 = GPIO.HIGH
            time.sleep(0.05)
            continue

        # Button 1 toggles LED1
        if last_button1 == GPIO.HIGH and b1 == GPIO.LOW:
            led1_state = not led1_state
            pwm1.ChangeDutyCycle(100 if led1_state else 0)
            print("LED1", "ON" if led1_state else "OFF")

        # Button 2 increases LED2 brightness
        if last_button2 == GPIO.HIGH and b2 == GPIO.LOW:
            brightness += 10
            if brightness > 100:
                brightness = 0

            pwm2.ChangeDutyCycle(brightness)
            print(f"Brightness: {brightness}%")

        last_button1 = b1
        last_button2 = b2

        time.sleep(0.05)

except KeyboardInterrupt:
    pass

finally:
    pwm1.stop()
    pwm2.stop()
    GPIO.cleanup()
