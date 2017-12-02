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

   :param str channel: PWM channel. It can be specified in the form
       of of 'P8_10', or 'EHRPWM2A'.
   :param int duty_cycle: PWM duty cycle. It must have a value from 0 to 100.
   :param int frequency: PWM frequency, in Hz. It must be greater than 0.
   :param int polarity: defines whether the value for ``duty_cycle`` affects the
       rising edge or the falling edge of the waveform. Allowed values -- 0
       (rising edge, default) or 1 (falling edge).

.. function:: stop(channel)

   Stop the given PWM channel.

   :param str channel: PWM channel. It can be specified in the form
       of of 'P8_10', or 'EHRPWM2A'.

.. function:: set_duty_cycle(channel, duty_cycle)

   Change the duty cycle of the given PWM channel.

   :note: You must have started the PWM channel with :func:`start()`
       once, before changing the duty cycle.

   :param str channel: PWM channel. It can be specified in the form
       of of 'P8_10', or 'EHRPWM2A'.
   :param int duty_cycle: PWM duty cycle. It must have a value from 0 to 100.

.. function:: set_frequency(channel, frequency)

   Change the frequency of the given PWM channel.

   :note: You must have started the PWM channel with :func:`start()`
       once, before changing the frequency.

   :param str channel: PWM channel. It can be specified in the form
       of of 'P8_10', or 'EHRPWM2A'.
   :param int frequency: PWM frequency. It must be greater than 0.

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

.. module:: Adafruit_BBIO.UART

.. function:: setup_uart(channel)

   Set up and start the UART channel. This function will effectively export
   the given UART so that it can be accessed by other software that controls
   its serial lines.

   :param channel: UART channel to set up. One of "UART1", "UART2",
       "UART4" or "UART5"
   :type channel: str

.. function:: cleanup()

   Cleans up the UART.

:mod:`ADC` --- A/D Converter input interface
--------------------------------------------

This module enables reading analog input values from the analog to digital
converter (ADC) on the Beaglebone processor.

Example::

    import Adafruit_BBIO.ADC as ADC

    ADC.setup()

    # The read method returns normalized values from 0 to 1.0
    value = ADC.read("P9_40")

    # The read_raw returns non-normalized values
    value = ADC.read_raw("P9_40")

.. module:: Adafruit_BBIO.ADC

.. function:: setup_adc()

   Setup and start the ADC hardware.

.. function:: setup_read()

   Read the normalized 0-1.0 analog value for the channel.

.. function:: setup_read_raw()

   Read the raw analog value for the channel.

:mod:`SPI` --- Serial Peripheral Interface
------------------------------------------

This module defines an object type that allows Serial Peripheral Interface
(SPI) bus transactions on hosts running the Linux kernel. The host kernel
must have SPI support and SPI device interface support.

Because the SPI device interface is opened R/W, users of this module 
usually must have root permissions or be members of a group with granted
access rights.

Example::

    import Adafruit_BBIO.SPI as SPI

    from Adafruit_BBIO.SPI import SPI
    # spi = SPI(bus, device) #/dev/spidev<bus>.<device>

    # /dev/spidev0.0
    spi = SPI(1, 0)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close() 

    # /dev/spidev0.1
    spi = SPI(1,1)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close() 

    # /dev/spidev1.0
    spi = SPI(2,0)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close() 

    # /dev/spidev1.1
    spi = SPI(2,1)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close() 

.. module:: Adafruit_BBIO.SPI

.. class:: SPI(bus, client)

   :param bus: bus number.
   :param client: client number.
   :returns: a new SPI object, optionally connected to the specified SPI
       device interface.
   :rtype: :class:`SPI`

   .. attribute:: bpw

      Bits per word.

   .. attribute:: cshigh

      Chip Select (CS or Slave Select, SS) active high.

   .. attribute:: loop

      Loopback configuration.

   .. attribute:: lsbfirst

      Least Significant Bit (LSB) first.

   .. attribute:: mode

      SPI mode as two bit pattern of Clock Polarity and Phase [CPOL|CPHA]; min-- 0b00 = 0, max-- 0b11 = 3.

   .. attribute:: msh

      Maximum speed in Hz.

   .. attribute:: threewire

      SI/SO signals are shared.

   .. method:: open(bus, device)

      Connects the object to the specified SPI device. `open(X, Y)` will open
      `/dev/spidev-X.Y`

      :param int bus: bus number
      :param str device: device number

   .. method:: close()

      Disconnects the object from the interface.

   .. method:: readbytes(len)

      Read the specified length of bytes from the SPI device.

      :param int len: length of bytes to read, 1024 maximum.
      :returns: values read
      :rtype: list[int]

   .. method:: writebytes(values)

      Write bytes to the SPI device.

      :param values: list of values to write, with a maximum length of 1024.
      :type values: list[int]

   .. method:: xfer(values[,delay=0])

      xfer([values]) -> [values]
      Perform an SPI transaction of values. Slave Select (SS or CS) will be
      released and reactivated between blocks.

      :param values: list of values to transfer, with a maximum length of 1024.
      :type values: list[int]
      :param delay: delay in microseconds between blocks.
      :returns: values transferred
      :rtype: list[int]

   .. method:: xfer2(values)

      xfer2([values]) -> [values]
      Perform an SPI transaction of values. Slave Select (SS or CS) will be
      held active between blocks.

      :param values: list of values to transfer, with a maximum length of 1024.
      :type values: list[int]
      :returns: values transferred
      :rtype: list[int]

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`


