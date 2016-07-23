#debian@beaglebone:~/adafruit-beaglebone-io-python/test$ cat test_adc.py
import pytest
import os
import platform
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

        kernel = platform.release()
        if kernel >= '4.1.0':
            test_path = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
        else:
            files = os.listdir('/sys/devices')
            ocp = '/sys/devices/'+[s for s in files if s.startswith('ocp')][0]
            files = os.listdir(ocp)
            helper_path = ocp+'/'+[s for s in files if s.startswith('helper')][0]
            test_path = helper_path + "/AIN1"

        assert os.path.exists(test_path);
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
