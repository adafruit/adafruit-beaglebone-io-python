import pytest
import os

import Adafruit_BBIO.GPIO as GPIO

def teardown_module(module):
    GPIO.cleanup()

class TestGPIOOutput:
    def test_output_high(self):
        GPIO.setup("P8_10", GPIO.OUT)
        GPIO.output("P8_10", GPIO.HIGH)
        value = open('/sys/class/gpio/gpio68/value').read()
        assert int(value)
        GPIO.cleanup()

    def test_output_low(self):
        GPIO.setup("P8_10", GPIO.OUT)
        GPIO.output("P8_10", GPIO.LOW)
        value = open('/sys/class/gpio/gpio68/value').read()
        assert not int(value)
        GPIO.cleanup()

    def test_direction_readback(self):
        GPIO.setup("P8_10", GPIO.OUT)
        direction = GPIO.gpio_function("P8_10")
        assert direction == GPIO.OUT
    def test_output_greater_than_one(self):
        GPIO.setup("P8_10", GPIO.OUT)
        GPIO.output("P8_10", 2)
        value = open('/sys/class/gpio/gpio68/value').read()
        assert int(value)
        GPIO.cleanup()

    def test_output_of_pin_not_setup(self):
        with pytest.raises(RuntimeError):
            GPIO.output("P8_11", GPIO.LOW)
            GPIO.cleanup() 

    def test_output_setup_as_input(self):
        GPIO.setup("P8_10", GPIO.IN)
        with pytest.raises(RuntimeError):
            GPIO.output("P8_10", GPIO.LOW)
            GPIO.cleanup()
