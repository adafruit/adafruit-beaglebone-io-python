:mod:`UART` --- UART communications interface
---------------------------------------------

UART functionality of a BeagleBone using Python. Generally used to set up
and grant access to a given UART device, which will then be accessed by
other software or modules (e.g. `pyserial`)::

    sudo pip install pyserial

Example::

    import Adafruit_BBIO.UART as UART
    import serial

    UART.setup("UART1")

    with serial.Serial(port = "/dev/ttyO1", baudrate=9600) as ser:
        print("Serial is open!")
        ser.write(b"Hello World!")

.. module:: Adafruit_BBIO.UART

.. function:: setup_uart(channel)

   Set up and start the UART channel. This function will effectively export
   the given UART so that it can be accessed by other software that controls
   its serial lines.

   :param str channel: UART channel to set up. One of "UART1", "UART2",
       "UART4" or "UART5"

.. function:: cleanup()

   Cleans up the UART.

