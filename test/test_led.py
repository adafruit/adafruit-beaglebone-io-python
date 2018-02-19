import pytest
import os
import errno

import Adafruit_BBIO.GPIO as GPIO

def teardown_module(module):
    GPIO.cleanup()

class TestLED:
    def set_brightness(self, state, led, name):
        GPIO.setup(led, GPIO.OUT)
        GPIO.output(led, state)
        prefix = "/sys/class/leds/beaglebone:green:{0}/brightness"
        path = prefix.format(led.lower())
        value = self.read_led_file(path)
        if value == "":
            path = prefix.format(name)
            value = self.read_led_file(path)
        if state == 1:
            assert int(value) > 0
        else:
            assert int(value) == 0

    def read_led_file(self, path):
        try:
            return open(path).read()
        except (IOError, e):
            if e.errno == errno.ENOENT:
                return ""

    def set_all_leds(self, state):
        test_leds = { "USR0": "heartbeat", \
                      "USR1": "mmc0", \
                      "USR2": "cpu0", \
                      "USR3": "mmc1" }
        for led, name in test_leds.items():
            self.set_brightness(state, led, name)
            GPIO.cleanup()
   
    def test_brightness_high(self):
        self.set_all_leds(GPIO.HIGH)

    def test_brightness_low(self):
        self.set_all_leds(GPIO.LOW)


