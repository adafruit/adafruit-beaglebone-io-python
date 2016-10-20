import pytest
import os

from Adafruit_BBIO.SPI import SPI

def teardown_module(module):
    pass

class TestAdc:    
    def test_spi(self):
        SPI(1,1)

    def test_setup_spi_wrong_values(self):
        with pytest.raises(TypeError):   
            SPI('x', 'x')