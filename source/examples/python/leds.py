#!/usr/bin/env python3
# Blinks some gpio pins on the Blue
import Adafruit_BBIO.GPIO as GPIO
import time

LEDs = ["RED", "GREEN"]

for LED in LEDs:
    GPIO.setup(LED, GPIO.OUT)

while True:
    for LED in LEDs:
        GPIO.output(LED, GPIO.HIGH)
        time.sleep(0.1)
        GPIO.output(LED, GPIO.LOW)
        time.sleep(0.1)