import glob
import os
import platform

import Adafruit_BBIO.PWM as PWM
import pytest

kernel = platform.release()


def teardown_module(module):
    PWM.cleanup()


def get_pwm_dir():
    if kernel >= '4.1.0':
        # On 4.1+, the pwm subdirectory sometimes takes different names:
        # .pwm or .ehrpwm, etc.
        results = glob.glob(
            "/sys/devices/platform/ocp/48302000.*/" +
            "48302200.*/pwm/pwmchip?/pwm0")
        # We expect that there will be a result (a directory fitting
        # our path exists) so test that with an assertion.
        assert len(results) > 0
        # Continue with the pwm_dir found
        return results[0]
    else:
        files = os.listdir('/sys/devices')
        ocp = '/sys/devices/' + [s for s in files if s.startswith('ocp')][0]
        files = os.listdir(ocp)
        return ocp + '/' + [s for s in files if s.startswith('pwm_test_P9_14')][0]


class TestPwmSetup:
    def test_start_pwm(self):
        PWM.cleanup()
        PWM.start("P9_14", 0)

        pwm_dir = get_pwm_dir()

        assert os.path.exists(pwm_dir)
        if kernel >= '4.1.0':
            duty = open(pwm_dir + '/duty_cycle').read()
        else:
            duty = open(pwm_dir + '/duty').read()
        period = open(pwm_dir + '/period').read()
        assert int(duty) == 0
        assert int(period) == 500000
        PWM.cleanup()

    def test_start_pwm_ecap0(self):
        print("test_start_pwm_ecap0\n");
        PWM.cleanup()
        PWM.start("P9_42", 0)
        pwm_dir = get_pwm_dir()
        assert os.path.exists(pwm_dir)
        if kernel >= '4.1.0':
            duty = open(pwm_dir + '/duty_cycle').read()
        else:
            duty = open(pwm_dir + '/duty').read()
        period = open(pwm_dir + '/period').read()
        assert int(duty) == 0
        assert int(period) == 500000
        PWM.cleanup()

    # test not enabled as default as
    # cape-universala overlay required
    #def test_start_pwm_ecap2(self):
        #print("test_start_pwm_ecap2\n");
        #PWM.cleanup()
        #PWM.start("P9_28", 0)
        #pwm_dir = get_pwm_dir()
        #assert os.path.exists(pwm_dir)
        #if kernel >= '4.1.0':
            #duty = open(pwm_dir + '/duty_cycle').read()
        #else:
            #duty = open(pwm_dir + '/duty').read()
        #period = open(pwm_dir + '/period').read()
        #assert int(duty) == 0
        #assert int(period) == 500000
        #PWM.cleanup()

    def test_start_pwm_with_polarity_one(self):
        PWM.cleanup()
        PWM.start("P9_14", 0, 2000, 1)

        pwm_dir = get_pwm_dir()

        assert os.path.exists(pwm_dir)
        if kernel >= '4.1.0':
            duty = open(pwm_dir + '/duty_cycle').read()
        else:
            duty = open(pwm_dir + '/duty').read()
        period = open(pwm_dir + '/period').read()
        polarity = open(pwm_dir + '/polarity').read()
        assert int(duty) == 0
        assert int(period) == 500000

        if kernel >= '4.1.0':
            assert polarity == "inversed\n"
        else:
            assert int(polarity) == 1
        PWM.cleanup()

    def test_start_pwm_with_polarity_default(self):
        PWM.cleanup()
        PWM.start("P9_14", 0, 2000)

        pwm_dir = get_pwm_dir()

        assert os.path.exists(pwm_dir)
        if kernel >= '4.1.0':
            duty = open(pwm_dir + '/duty_cycle').read()
        else:
            duty = open(pwm_dir + '/duty').read()
        period = open(pwm_dir + '/period').read()
        polarity = open(pwm_dir + '/polarity').read()
        assert int(duty) == 0
        assert int(period) == 500000

        if kernel >= '4.1.0':
            assert polarity == 'normal\n'
        else:
            assert int(polarity) == 0
        PWM.cleanup()

    def test_start_pwm_with_polarity_zero(self):
        PWM.cleanup()
        PWM.start("P9_14", 0, 2000, 0)

        pwm_dir = get_pwm_dir()

        assert os.path.exists(pwm_dir)
        if kernel >= '4.1.0':
            duty = open(pwm_dir + '/duty_cycle').read()
        else:
            duty = open(pwm_dir + '/duty').read()
        period = open(pwm_dir + '/period').read()
        polarity = open(pwm_dir + '/polarity').read()
        assert int(duty) == 0
        assert int(period) == 500000

        if kernel >= '4.1.0':
            assert polarity == 'normal\n'
        else:
            assert int(polarity) == 0
        PWM.cleanup()

    def test_pwm_start_invalid_pwm_key(self):
        PWM.cleanup()
        with pytest.raises(ValueError):
            PWM.start("P8_25", -1)

    def test_pwm_start_invalid_duty_cycle_negative(self):
        PWM.cleanup()
        with pytest.raises(ValueError):
            PWM.start("P9_14", -1)

    def test_pwm_start_valid_duty_cycle_min(self):
        PWM.cleanup()
        # testing an exception isn't thrown
        PWM.start("P9_14", 0)
        PWM.cleanup()

    def test_pwm_start_valid_duty_cycle_max(self):
        PWM.cleanup()
        # testing an exception isn't thrown
        PWM.start("P9_14", 100)
        PWM.cleanup()

    def test_pwm_start_invalid_duty_cycle_high(self):
        PWM.cleanup()
        with pytest.raises(ValueError):
            PWM.start("P9_14", 101)

    def test_pwm_start_invalid_duty_cycle_string(self):
        PWM.cleanup()
        with pytest.raises(TypeError):
            PWM.start("P9_14", "1")

    def test_pwm_start_invalid_frequency_negative(self):
        PWM.cleanup()
        with pytest.raises(ValueError):
            PWM.start("P9_14", 0, -1)

    def test_pwm_start_invalid_frequency_string(self):
        PWM.cleanup()
        with pytest.raises(TypeError):
            PWM.start("P9_14", 0, "1")

    def test_pwm_start_negative_polarity(self):
        PWM.cleanup()
        with pytest.raises(ValueError):
            PWM.start("P9_14", 0, 100, -1)

    def test_pwm_start_invalid_positive_polarity(self):
        PWM.cleanup()
        with pytest.raises(ValueError):
            PWM.start("P9_14", 0, 100, 2)

    def test_pwm_start_invalid_polarity_type(self):
        PWM.cleanup()
        with pytest.raises(TypeError):
            PWM.start("P9_14", 0, 100, "1")

    def test_pwm_duty_modified(self):
        PWM.cleanup()
        PWM.start("P9_14", 0)

        pwm_dir = get_pwm_dir()

        assert os.path.exists(pwm_dir)
        if kernel >= '4.1.0':
            duty = open(pwm_dir + '/duty_cycle').read()
        else:
            duty = open(pwm_dir + '/duty').read()
        period = open(pwm_dir + '/period').read()
        assert int(duty) == 0
        assert int(period) == 500000

        PWM.set_duty_cycle("P9_14", 100)
        if kernel >= '4.1.0':
            duty = open(pwm_dir + '/duty_cycle').read()
        else:
            duty = open(pwm_dir + '/duty').read()
        period = open(pwm_dir + '/period').read()
        assert int(duty) == 500000
        assert int(period) == 500000
        PWM.cleanup()

    def test_pwm_duty_cycle_non_setup_key(self):
        PWM.cleanup()
        with pytest.raises(RuntimeError):
            PWM.set_duty_cycle("P9_14", 100)
        PWM.cleanup()

    def test_pwm_duty_cycle_invalid_key(self):
        PWM.cleanup()
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("P9_15", 100)
        PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_high(self):
        PWM.cleanup()
        PWM.start("P9_14", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("P9_14", 101)
        PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_negative(self):
        PWM.cleanup()
        PWM.start("P9_14", 0)
        with pytest.raises(ValueError):
            PWM.set_duty_cycle("P9_14", -1)
        PWM.cleanup()

    def test_pwm_duty_cycle_invalid_value_string(self):
        PWM.cleanup()
        PWM.start("P9_14", 0)
        with pytest.raises(TypeError):
            PWM.set_duty_cycle("P9_14", "a")
        PWM.cleanup()

    def test_pwm_frequency_invalid_value_negative(self):
        PWM.cleanup()
        PWM.start("P9_14", 0)
        with pytest.raises(ValueError):
            PWM.set_frequency("P9_14", -1)
        PWM.cleanup()

    def test_pwm_frequency_invalid_value_string(self):
        PWM.cleanup()
        PWM.start("P9_14", 0)
        with pytest.raises(TypeError):
            PWM.set_frequency("P9_14", "11")
        PWM.cleanup()

    def test_pwm_freq_non_setup_key(self):
        PWM.cleanup()
        with pytest.raises(RuntimeError):
            PWM.set_frequency("P9_14", 100)
        PWM.cleanup()

    def test_pwm_freq_non_setup_invalid_key(self):
        PWM.cleanup()
        with pytest.raises(ValueError):
            PWM.set_frequency("P9_15", 100)
        PWM.cleanup()

    def test_stop_pwm(self):
        pass
        # PWM.start("P9_14", 1)
        # PWM.stop("P9_14")
        # assert os.path.exists('/sys/class/gpio/gpio68')
        # direction = open('/sys/class/gpio/gpio68/direction').read()
        # assert direction == 'out\n'
        # PWM.cleanup()
