import pytest
import os

import Adafruit_BBIO.ADC as ADC

def teardown_module(module):
    pass
    #ADC.cleanup()

class TestAdc:    
    def test_read_adc_no_setup(self):
        with pytest.raises(RuntimeError):   
            ADC.read("P9_40")

    def test_read_raw_adc_no_setup(self):
        with pytest.raises(RuntimeError):   
            ADC.read_raw("P9_40")

    def test_setup_adc(self):
        ADC.setup()

        files = os.listdir('/sys/devices')
        ocp = '/sys/devices/'+[s for s in files if s.startswith('ocp')][0]
        files = os.listdir(ocp)
        helper_path = ocp+'/'+[s for s in files if s.startswith('helper')][0]

        assert os.path.exists(helper_path + "/AIN1")
        #ADC.cleanup()

    def test_read_adc(self):
        ADC.setup()
        value = -1
        value = ADC.read("P9_40")

        assert value != -1

    def test_read_raw_adc(self):
        ADC.setup()
        value = -1
        value = ADC.read_raw("P9_40")

        assert value != -1

    def test_many_read_adc(self):
        import time

        ADC.setup()

        for x in range(0,10000):
            start = time.time() 
            value = -1
            value = ADC.read("AIN1")
            assert value != -1