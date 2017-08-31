#!/usr/bin/env python3
# Blinks some gpio pins on the Blue
import Adafruit_BBIO.GPIO as GPIO
import time

LEDs = ["GP0_3", "GP0_4", "GP0_5", "GP0_6", "GP1_3", "GP1_4",
        "RED_LED", "GREEN_LED"]
for LED in LEDs:
    print(LED)
    GPIO.setup(LED, GPIO.OUT)

while True:
    for LED in LEDs:
        GPIO.output(LED, GPIO.HIGH)
        time.sleep(0.1)
        GPIO.output(LED, GPIO.LOW)
