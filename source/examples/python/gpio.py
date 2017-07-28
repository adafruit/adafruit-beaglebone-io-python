#!/usr/bin/env python3
# Blinks some gpio pins on the Blue
import Adafruit_BBIO.GPIO as GPIO
import time

LED = "GP0_1"

GPIO.setup(LED, GPIO.OUT)

while True:
    GPIO.output(LED, GPIO.HIGH)
    time.sleep(0.25)
    GPIO.output(LED, GPIO.LOW)
    time.sleep(0.25)