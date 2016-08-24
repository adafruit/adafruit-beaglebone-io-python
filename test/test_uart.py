import pytest
import platform

import Adafruit_BBIO.UART as UART

kernel = platform.release()


def teardown_module(module):
    pass
    # ADC.cleanup()


class TestAdc: 
    def test_setup_uart_wrong_name(self):
        if kernel >= '4.1.0':
            pass
        else:
            with pytest.raises(ValueError):
                UART.setup("UART7")

    def test_setup_adc(self):
        if kernel >= '4.1.0':
            pass
        else:
            UART.setup("UART1")

    def test_setup_adc_multiple(self):
        if kernel >= '4.1.0':
            pass
        else:
            UART.setup("UART1")
            UART.setup("UART1")
