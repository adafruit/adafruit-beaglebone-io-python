import pytest
import os

import Adafruit_BBIO.PWM as PWM

def teardown_module(module):
    PWM.cleanup()

class TestPwmSetup:    
    def test_start_pwm(self):
        PWM.start("P9_14", 0)

        files = os.listdir('/sys/devices')
        ocp = '/sys/devices/'+[s for s in files if s.startswith('ocp')][0]
        files = os.listdir(ocp)
        pwm_test = ocp+'/'+[s for s in files if s.startswith('pwm_test_P9_14')][0]

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty').read()
        period = open(pwm_test + '/period').read()
        assert int(duty) == 0
        assert int(period) == 500000
        PWM.cleanup()

    def test_start_pwm_with_polarity_one(self):
        PWM.start("P9_14", 0, 2000, 1)

        files = os.listdir('/sys/devices')
        ocp = '/sys/devices/'+[s for s in files if s.startswith('ocp')][0]
        files = os.listdir(ocp)
        pwm_test = ocp+'/'+[s for s in files if s.startswith('pwm_test_P9_14')][0]

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty').read()
        period = open(pwm_test + '/period').read()
        polarity = open(pwm_test + '/polarity').read()
        assert int(duty) == 0
        assert int(period) == 500000
        assert int(polarity) == 1
        PWM.cleanup()      

    def test_start_pwm_with_polarity_default(self):
        PWM.start("P9_14", 0, 2000, 0)

        files = os.listdir('/sys/devices')
        ocp = '/sys/devices/'+[s for s in files if s.startswith('ocp')][0]
        files = os.listdir(ocp)
        pwm_test = ocp+'/'+[s for s in files if s.startswith('pwm_test_P9_14')][0]

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty').read()
        period = open(pwm_test + '/period').read()
        polarity = open(pwm_test + '/polarity').read()
        assert int(duty) == 0
        assert int(period) == 500000
        assert int(polarity) == 0
        PWM.cleanup()         

    def test_start_pwm_with_polarity_zero(self):
        PWM.start("P9_14", 0, 2000, 0)

        files = os.listdir('/sys/devices')
        ocp = '/sys/devices/'+[s for s in files if s.startswith('ocp')][0]
        files = os.listdir(ocp)
        pwm_test = ocp+'/'+[s for s in files if s.startswith('pwm_test_P9_14')][0]

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty').read()
        period = open(pwm_test + '/period').read()
        polarity = open(pwm_test + '/polarity').read()
        assert int(duty) == 0
        assert int(period) == 500000
        assert int(polarity) == 0
        PWM.cleanup()  

    def test_pwm_start_invalid_pwm_key(self):
        with pytest.raises(ValueError):   
            PWM.start("P8_25", -1)             

    def test_pwm_start_invalid_duty_cycle_negative(self):
        with pytest.raises(ValueError):   
            PWM.start("P9_14", -1)     

    def test_pwm_start_valid_duty_cycle_min(self):
        #testing an exception isn't thrown
        PWM.start("P9_14", 0)    
        PWM.cleanup()

    def test_pwm_start_valid_duty_cycle_max(self):
        #testing an exception isn't thrown
        PWM.start("P9_14", 100)  
        PWM.cleanup()            

    def test_pwm_start_invalid_duty_cycle_high(self):
        with pytest.raises(ValueError):   
            PWM.start("P9_14", 101)  

    def test_pwm_start_invalid_duty_cycle_string(self):
        with pytest.raises(TypeError):   
            PWM.start("P9_14", "1")              

    def test_pwm_start_invalid_frequency_negative(self):
        with pytest.raises(ValueError):
            PWM.start("P9_14", 0, -1)

    def test_pwm_start_invalid_frequency_string(self):
        with pytest.raises(TypeError):   
            PWM.start("P9_14", 0, "1")      

    def test_pwm_start_negative_polarity(self):
        with pytest.raises(ValueError):   
            PWM.start("P9_14", 0, 100, -1)     

    def test_pwm_start_invalid_positive_polarity(self):
        with pytest.raises(ValueError):   
            PWM.start("P9_14", 0, 100, 2)       

    def test_pwm_start_invalid_polarity_type(self):
        with pytest.raises(TypeError):   
            PWM.start("P9_14", 0, 100, "1")                                            

    def test_pwm_duty_modified(self):
        PWM.start("P9_14", 0)

        files = os.listdir('/sys/devices')
        ocp = '/sys/devices/'+[s for s in files if s.startswith('ocp')][0]
        files = os.listdir(ocp)
        pwm_test = ocp+'/'+[s for s in files if s.startswith('pwm_test_P9_14')][0]

        assert os.path.exists(pwm_test)
        duty = open(pwm_test + '/duty').read()
        period = open(pwm_test + '/period').read()
        assert int(duty) == 0
        assert int(period) == 500000

        PWM.set_duty_cycle("P9_14", 100)
        duty = open(pwm_test + '/duty').read()
        period = open(pwm_test + '/period').read()
        assert int(duty) == 500000
        assert int(period) == 500000        
        PWM.cleanup()

    def test_pwm_duty_cycle_non_setup_key(self):
        with pytest.raises(RuntimeError):
            PWM.set_duty_cycle("P9_14", 100)
            PWM.cleanup()    

    def test_pwm_duty_cycle_invalid_key(self):
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("P9_15", 100)
            PWM.cleanup()                  

    def test_pwm_duty_cycle_invalid_value_high(self):
        PWM.start("P9_14", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("P9_14", 101)
            PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_negative(self):
        PWM.start("P9_14", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("P9_14", -1)
            PWM.cleanup()            

    def test_pwm_duty_cycle_invalid_value_string(self):
        PWM.start("P9_14", 0)
        with pytest.raises(TypeError):
            PWM.set_duty_cycle("P9_14", "a")
            PWM.cleanup()               

    def test_pwm_frequency_invalid_value_negative(self):
        PWM.start("P9_14", 0)
        with pytest.raises(ValueError):
            PWM.set_frequency("P9_14", -1)
            PWM.cleanup()            

    def test_pwm_frequency_invalid_value_string(self):
        PWM.start("P9_14", 0)
        with pytest.raises(TypeError):
            PWM.set_frequency("P9_14", "11")
            PWM.cleanup()

    def test_pwm_freq_non_setup_key(self):
        with pytest.raises(RuntimeError):
            PWM.set_frequency("P9_14", 100)
            PWM.cleanup()       

    def test_pwm_freq_non_setup_key(self):
        with pytest.raises(ValueError):
            PWM.set_frequency("P9_15", 100)
            PWM.cleanup()                            

    def test_stop_pwm(self):
        pass
        #PWM.start("P9_14", 1)
        #PWM.stop("P9_14")
        #assert os.path.exists('/sys/class/gpio/gpio68')
        #direction = open('/sys/class/gpio/gpio68/direction').read()
        #assert direction == 'out\n'        
        #PWM.cleanup()