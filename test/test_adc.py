import pytest
import os

import BBIO.ADC as ADC

def teardown_module(module):
    ADC.cleanup()

class TestAdc:    
    def test_setup_adc(self):
        ADC.setup()

        files = os.listdir('/sys/devices')
        ocp = '/sys/devices/'+[s for s in files if s.startswith('ocp')][0]
        files = os.listdir(ocp)
        helper_path = ocp+'/'+[s for s in files if s.startswith('helper')][0]

        assert os.path.exists(helper_path + "/AIN1")
        ADC.cleanup()