import pytest
import os

import Adafruit_BBIO.UART as UART

def teardown_module(module):
    pass
    #ADC.cleanup()

class TestAdc:    
    def test_setup_uart_wrong_name(self):
        with pytest.raises(ValueError):   
            UART.setup("UART7")

    def test_setup_adc(self):
        UART.setup("UART1")

    def test_setup_adc_multiple(self):
        UART.setup("UART1")        
        UART.setup("UART1")   