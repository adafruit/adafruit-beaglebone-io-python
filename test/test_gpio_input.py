import pytest
import os

import Adafruit_BBIO.GPIO as GPIO

def teardown_module(module):
    GPIO.cleanup()

class TestGPIOInput:
    def test_input(self):
        GPIO.setup("P8_10", GPIO.IN)
        #returned as an int type
        input_value = GPIO.input("P8_10")
        #value read from the file will have a \n new line
        value = open('/sys/class/gpio/gpio68/value').read()
        assert int(value) == input_value
        GPIO.cleanup()

    def test_direction_readback(self):
        GPIO.setup("P8_10", GPIO.IN)
        direction = GPIO.gpio_function("P8_10")
        assert direction == GPIO.IN
