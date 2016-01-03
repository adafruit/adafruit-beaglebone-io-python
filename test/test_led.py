import pytest
import os

import Adafruit_BBIO.GPIO as GPIO

def teardown_module(module):
    GPIO.cleanup()

class TestLED:
    def test_brightness_high(self):
        GPIO.setup("USR0", GPIO.OUT)
        GPIO.output("USR0", GPIO.HIGH)
        value = open('/sys/class/leds/beaglebone:green:usr0/brightness').read()
        assert int(value) > 0
        GPIO.cleanup()

    def test_brightness_low(self):
        GPIO.setup("USR0", GPIO.OUT)
        GPIO.output("USR0", GPIO.LOW)
        value = open('/sys/class/leds/beaglebone:green:usr0/brightness').read()
        assert int(value) == 0
        GPIO.cleanup()
