import pytest
import serial
import platform

import Adafruit_BBIO.UART as UART

kernel = platform.release()


def teardown_module(module):
    pass
    # ADC.cleanup()


class TestUart:
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

    def test_setup_uart_multiple(self):
        if kernel >= '4.1.0':
            pass
        else:
            UART.setup("UART1")
            UART.setup("UART1")

    # test UART entries for the PocketBeagle (issue #243)
    def test_pocketbeagle(self):
        if kernel < '4.1.0':
            pass
        value = open('/proc/device-tree/model').read()
        if(value.startswith("TI AM335x PocketBeagle")):
          uarts = {
            'PB-UART0': '/dev/ttyO0',
            'PB-UART1': '/dev/ttyO1',
            'PB-UART2': '/dev/ttyO2',
          }
        else:
          uarts = {
            'UART1': '/dev/ttyO1',
            'UART2': '/dev/ttyO2',
            'UART4': '/dev/ttyO4'
            # note: UART5 requires
            # "disable_uboot_overlay_video=1" in /boot/uEnv.txt
            #'UART5': '/dev/ttyO5'
          }

        for name, device in sorted(uarts.items()):
          UART.setup(name)
          uart = serial.Serial(port = device, baudrate=9600)
          uart.close()
          uart.open()
          if uart.isOpen():
            uart.write("hello world".encode("utf-8"))
          uart.close()
