.. Adafruit-BBIO documentation master file, created by
   sphinx-quickstart on Fri Dec  1 12:56:03 2017.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. toctree::
   :hidden:
   :maxdepth: 2

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

.. note::

   You need to be part of the ``pwm`` group of the OS running on the
   Beaglebone to be able to run PWM code as a non-root user. The default
   user created upon the Debian image installation should already be
   part of the group. Otherwise, you can use
   ``sudo usermod -a -G pwm userName`` to add ``userName`` to the group.

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

   :param str channel: UART channel to set up. One of "UART1", "UART2",
       "UART4" or "UART5"

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

Pins used for SPI0 and SPI1
+++++++++++++++++++++++++++

====  =====  =====  =====  =====
PORT  CS0    DO     DI      SCLK
====  =====  =====  =====  =====
SPI0  P9_17  P9_21  P9_18  P9_22
SPI1  P9_28  P9_29  P9_30  P9_31
====  =====  =====  =====  =====

Example::

    import Adafruit_BBIO.SPI as SPI

    from Adafruit_BBIO.SPI import SPI
    # spi = SPI(bus, device) #/dev/spidev<bus>.<device>

    # /dev/spidev0.0
    spi = SPI(1, 0)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close()

    # /dev/spidev0.1
    spi = SPI(1, 1)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close()

    # /dev/spidev1.0
    spi = SPI(2, 0)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close()

    # /dev/spidev1.1
    spi = SPI(2, 1)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close()

.. module:: Adafruit_BBIO.SPI

.. class:: SPI(bus, client)

   :param bus: bus number.
   :param client: client device number.
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

      Perform an SPI transaction of values. Slave Select (SS or CS) will be
      released and reactivated between blocks.

      :param values: list of values to transfer, with a maximum length of 1024.
      :type values: list[int]
      :param delay: delay in microseconds between blocks.
      :returns: values transferred
      :rtype: list[int]

   .. method:: xfer2(values)

      Perform an SPI transaction of values. Slave Select (SS or CS) will be
      held active between blocks.

      :param values: list of values to transfer, with a maximum length of 1024.
      :type values: list[int]
      :returns: values transferred
      :rtype: list[int]

:mod:`GPIO` --- General Purpose I/O interface
---------------------------------------------

This module provides access and control of pins set up as General Purpose
I/O (GPIO).

.. note::

   You need to be part of the ``gpio`` group of the OS running on the
   Beaglebone to be able to run GPIO code as a non-root user. The default
   user created upon the Debian image installation should already be
   part of the group. Otherwise, you can use
   ``sudo usermod -a -G gpio userName`` to add ``userName`` to the group.

.. note::

   When coding with this module, you will be using pin names for
   better readability. As such, you can specify them in the header 8 or 9
   form (e.g. "P8_16") or in pin name form (e.g. "GPIO1_14").
   For easy reference, you can use the
   `Beaglebone pin names table <https://github.com/adafruit/adafruit-beaglebone-io-python/blob/master/source/common.c#L73>`_


Example::

    # Use the config-pin command line tool to set a pin's function to GPIO
    # Then you can control it with the GPIO module from Python
    config-pin P9_14 gpio

    import Adafruit_BBIO.GPIO as GPIO

    # Set up pins as inputs or outputs
    GPIO.setup("P8_13", GPIO.IN)
    GPIO.setup("P8_14", GPIO.OUT)
    GPIO.setup("GPIO0_26", GPIO.OUT)  # Alternative: use actual pin names

    # Write a logic high or logic low
    GPIO.output("P8_14", GPIO.HIGH)  # You can also write '1' instead
    GPIO.output("P8_14", GPIO.LOW)   # You can also write '0' instead

.. module:: Adafruit_BBIO.GPIO

.. function:: setup(channel, direction[, pull_up_down=:data:`PUD_OFF`, initial=None, delay=0])

   Set up the given GPIO channel, its direction and (optional) pull/up down control

   :param str channel: GPIO channel to set up (e.g. "P8_16").
   :param int direction: GPIO channel direction (:data:`IN` or :data:`OUT`).
   :param int pull_up_down: pull-up/pull-down resistor configuration
       (:data:`PUD_OFF`, :data:`PUD_UP` or :data:`PUD_DOWN`).
   :param int initial: initial value for an output channel (:data:`LOW`/:data:`HIGH`).
   :param int delay: time in milliseconds to wait after exporting the GPIO pin.

.. function:: cleanup()

   Clean up by resetting all GPIO channels that have been used by
   the application to :data:`IN` with no pullup/pulldown and no event
   detection.

.. function:: output(channel, value)

   Set the given output channel to the given digital value.

   :param str channel: GPIO channel to output the value to (e.g. "P8_16").
   :param value: value to set the output to-- 0/1 or False/True
       or :data:`LOW`/:data:`HIGH`.
   :type value: int or bool

.. function:: input(channel)

   Get the given input channel's digital value.

   :param str channel: GPIO channel to read the value from (e.g. "P8_16").
   :returns: Channel value–– 0 or 1.
   :rtype: int

.. function:: add_event_detect(channel, edge[, callback=None, bouncetime=0])

   Enable edge detection events for the given GPIO channel.

   :param str channel: GPIO channel to detect events from (e.g. "P8_16").
   :param int edge: edge to detect–– :data:`RISING`, :data:`FALLING`
       or :data:`BOTH`
   :param func callback: a function to call once the event has been detected.
   :param int bouncetime: switch bounce timeout in ms for the callback.

.. function:: remove_event_detect(channel)

   Remove edge detection for the given GPIO channel.

   :param str channel: GPIO channel to remove event detection
       from (e.g. "P8_16").

.. function:: event_detected(channel)

   Checks if an edge event has occured on a given GPIO.

   :note: You need to enable edge detection using :func:`add_event_detect()` first.

   :param str channel: GPIO channel to check for event detection
       for (e.g. "P8_16").
   :returns: True if an edge has occured on a given GPIO, False otherwise
   :rtype: bool

.. function:: add_event_callback(channel, callback[, bouncetime=0])

   Add a callback for an event already defined using :func:`add_event_detect()`

   :param str channel: GPIO channel to add a callback to (e.g. "P8_16").
   :param func callback: a function to call once the event has been detected.
   :param int bouncetime: switch bounce timeout in ms for the callback.

.. function:: wait_for_edge(channel, edge[, timeout=-1])

   Wait for an edge on the given channel.

   :param str channel: GPIO channel to wait on (e.g. "P8_16").
   :param int edge: edge to detect–– :data:`RISING`, :data:`FALLING`
       or :data:`BOTH`
   :param int timeout: time to wait for an edge, in milliseconds.
       -1 will wait forever.

.. function:: gpio_function(channel)

   Return the current GPIO function
   (:data:`IN`, :data:`IN`, :data:`ALT0`) of the given pin.

   :warning: Currently only returning the direction of the
       pin (input or output) is supported.

   :param str channel: GPIO pin to query the status of.
   :returns: 0 if :data:`IN`, 1 if :data:`OUT`
   :rtype: int

.. function:: setwarnings(gpio_warnings)

   Enable or disable GPIO warning messages.

   :warning: Currently enabling or disabling warnings
       has no effect.

   :param int gpio_warnings: 0–– disable warnings; 1–– enable warnings

.. attribute:: ALT0

   Pin mode-- alternate function 0.

.. attribute:: BOTH

   Edge detection-- detect both edges.

.. attribute:: FALLING

   Edge detection-- detect falling edge.

.. attribute:: HIGH

   Pin status-- logic low.

.. attribute:: IN

   Pin mode-- input.

.. attribute:: LOW

   Pin status-- logic low.

.. attribute:: OUT

   Pin mode-- output.

.. attribute:: PUD_OFF

   Pull-up/pull-down resistor type-- no pull-up/pull-down.

.. attribute:: PUD_DOWN

   Pull-up/pull-down resistor type-- pull-down.

.. attribute:: PUD_UP

   Pull-up/pull-down resistor type-- pull-up.

.. attribute:: RISING

   Edge detection-- detect rising edge.

.. attribute:: VERSION

   GPIO module version. Currently unused.

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`


