import pytest
import os
import platform

import Adafruit_BBIO.GPIO as GPIO

kernel = platform.release()


def teardown_module(module):
    GPIO.cleanup()


class TestSetup:
    def test_setup_output_key(self):
        GPIO.setup("P8_10", GPIO.OUT)
        assert os.path.exists('/sys/class/gpio/gpio68')
        direction = open('/sys/class/gpio/gpio68/direction').read()
        assert direction == 'out\n'        
        GPIO.cleanup()

    def test_setup_output_name(self):
        GPIO.setup("TIMER6", GPIO.OUT)
        assert os.path.exists('/sys/class/gpio/gpio68')
        direction = open('/sys/class/gpio/gpio68/direction').read()
        assert direction == 'out\n'        
        GPIO.cleanup()        

    def test_setup_input_key(self):
        GPIO.setup("P8_10", GPIO.IN)
        assert os.path.exists('/sys/class/gpio/gpio68')
        direction = open('/sys/class/gpio/gpio68/direction').read()
        assert direction == 'in\n'        
        GPIO.cleanup()

    def test_setup_input_name(self):
        GPIO.setup("TIMER6", GPIO.IN)
        assert os.path.exists('/sys/class/gpio/gpio68')
        direction = open('/sys/class/gpio/gpio68/direction').read()
        assert direction == 'in\n'        
        GPIO.cleanup()        

    def test_setup_input_pull_up(self):
        GPIO.setup("P8_10", GPIO.IN, pull_up_down=GPIO.PUD_UP)
        assert os.path.exists('/sys/class/gpio/gpio68')
        direction = open('/sys/class/gpio/gpio68/direction').read()
        assert direction == 'in\n'        
        GPIO.cleanup()

    def test_setup_input_pull_down(self):
        GPIO.setup("P8_10", GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
        assert os.path.exists('/sys/class/gpio/gpio68')
        direction = open('/sys/class/gpio/gpio68/direction').read()
        assert direction == 'in\n'        
        GPIO.cleanup()

    def test_setup_cleanup(self):
        GPIO.setup("P8_10", GPIO.OUT)
        assert os.path.exists('/sys/class/gpio/gpio68')
        GPIO.cleanup()
        if kernel < '4.1.0':
            assert not os.path.exists('/sys/class/gpio/gpio68')
            # for later kernels, the universal capemanager always loads the
            # UARTs.

    def test_setup_failed_type_error(self):
        with pytest.raises(TypeError):
            GPIO.setup("P8_10", "WEIRD")
            GPIO.cleanup()

    def test_setup_failed_value_error(self):
        with pytest.raises(ValueError):
            GPIO.setup("P8_10", 3)
            GPIO.cleanup()

    def test_setup_three_digit_gpio(self):
        GPIO.setup("P9_31", GPIO.OUT)
        assert os.path.exists('/sys/class/gpio/gpio110')
        GPIO.cleanup()
        if kernel < '4.1.0':
            assert not os.path.exists('/sys/class/gpio/gpio110')
            # for later kernels, the universal capemanager always loads the
            # UARTs.
