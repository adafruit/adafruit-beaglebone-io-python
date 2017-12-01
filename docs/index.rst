.. Adafruit-BBIO documentation master file, created by
   sphinx-quickstart on Fri Dec  1 12:56:03 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Adafruit Beaglebone IO Python API
=================================

The Adafruit Beaglebone IO API enables access to the Beaglebone's GPIO, PWM,
ADC, UART and eQEP hardware modules from Python programs.

:mod:`Encoder` --- Quadrature Encoder interface
-----------------------------------------------

.. automodule:: Adafruit_BBIO.Encoder
   :members:

:mod:`PWM` --- Pulse Width Modulation interface
-----------------------------------------------

Enables access to the Pulse Width Modulation (PWM) module, to easily and 
accurately generate a PWM output signal of a given duty cycle and
frequency.

.. module:: Adafruit_BBIO.PWM

.. function:: start(channel, duty_cycle[, frequency=2000, polarity=0])

   Set up and start the given PWM channel.

   :param channel: PWM channel. It can be specified in the form
       of of 'P8_10', or 'EHRPWM2A'.
   :type channel: str
   :param duty_cycle: PWM duty cycle. It must have a value from 0 to 100.
   :type duty_cycle: int
   :param frequency: PWM frequency, in Hz. It must be greater than 0.
   :type frequency: int
   :param polarity: defines whether the value for ``duty_cycle`` affects the
       rising edge or the falling edge of the waveform. Allowed values -- 0
       (rising edge, default) or 1 (falling edge).
   :type polarity: int

.. function:: stop(channel)

   Stop the given PWM channel.

   :param channel: PWM channel. It can be specified in the form
       of of 'P8_10', or 'EHRPWM2A'.
   :type channel: str

.. function:: set_duty_cycle(channel, duty_cycle)

   Change the duty cycle of the given PWM channel.

   :note: You must have started the PWM channel with :func:`start()`
       once, before changing the duty cycle.

   :param channel: PWM channel. It can be specified in the form
       of of 'P8_10', or 'EHRPWM2A'.
   :type channel: str
   :param duty_cycle: PWM duty cycle. It must have a value from 0 to 100.
   :type duty_cycle: int

.. function:: set_frequency(channel, frequency)

   Change the frequency of the given PWM channel.

   :note: You must have started the PWM channel with :func:`start()`
       once, before changing the frequency.

   :param channel: PWM channel. It can be specified in the form
       of of 'P8_10', or 'EHRPWM2A'.
   :type channel: str
   :param frequency: PWM frequency. It must be greater than 0.
   :type frequency: int

.. function:: cleanup()

   Clean up by resetting all GPIO channels that have been used by this
   program to INPUT, with no pullup/pulldown and no event detection.

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

    ser = serial.Serial(port = "/dev/ttyO1", baudrate=9600)
    ser.close()
    ser.open()
    if ser.isOpen():
	 print "Serial is open!"
         ser.write("Hello World!")
    ser.close()

.. function:: setup_uart(channel)

   Set up and start the UART channel. This function will effectively export
   the given UART so that it can be accessed by other software that controls
   its serial lines.

   :param channel: UART channel to set up. One of "UART1", "UART2",
       "UART4" or "UART5"
   :type channel: str

.. function:: cleanup()

   Cleans up the UART.


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`


