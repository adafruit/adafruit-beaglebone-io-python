import pytest
import os

import BBIO.PWM as PWM

def teardown_module(module):
    pass
    #PWM.cleanup()

class TestPwmSetup:
    def test_setup_pwm(self):
        PWM.setup("P8_13")
        #assert os.path.exists('/sys/class/gpio/gpio68')
        #direction = open('/sys/class/gpio/gpio68/direction').read()
        #assert direction == 'out\n'        
        #PWM.cleanup()
