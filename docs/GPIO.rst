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


.. note::

   On-board LEDs (USR0-USR3) are handled by LED class driver rather than the GPIO pin driver.

   They have a different path in the /sys/ filesystem.

   Setup the pin for output and write GPIO.HIGH or GPIO.LOW

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


    # Blinking onboard led example
    import Adafruit_BBIO.GPIO as GPIO
    import time

    for i in range(4):
        GPIO.setup("USR%d" % i, GPIO.OUT)

    while True:
        for i in range(4):
            GPIO.output("USR%d" % i, GPIO.HIGH)
            time.sleep(1)
        for i in range(4):
            GPIO.output("USR%d" % i, GPIO.LOW)
            time.sleep(1)


.. module:: Adafruit_BBIO.GPIO

.. function:: setup(channel, direction[, pull_up_down=GPIO.PUD_OFF, initial=None, delay=0])

   Set up the given GPIO channel, its direction and (optional) pull/up down control

   :param str channel: GPIO channel to set up (e.g. "P8_16").
   :param int direction: GPIO channel direction
       (:data:`GPIO.IN` or :data:`GPIO.OUT`).
   :param int pull_up_down: pull-up/pull-down resistor configuration
       (:data:`GPIO.PUD_OFF`, :data:`GPIO.PUD_UP` or :data:`GPIO.PUD_DOWN`).
   :param int initial: initial value for an output channel
       (:data:`GPIO.LOW`/:data:`GPIO.HIGH`).
   :param int delay: time in milliseconds to wait after exporting the GPIO pin.

.. function:: cleanup()

   Clean up by resetting all GPIO channels that have been used by
   the application to :data:`IN` with no pullup/pulldown and no event
   detection.

   :note: It's recommended that you call this function upon exiting your
       application.

.. function:: output(channel, value)

   Set the given output channel to the given digital value.

   :param str channel: GPIO channel to output the value to (e.g. "P8_16").
   :param value: value to set the output to-- 0/1 or False/True
       or :data:`GPIO.LOW`/:data:`GPIO.HIGH`.
   :type value: int or bool

.. function:: input(channel)

   Get the given input channel's digital value.

   :param str channel: GPIO channel to read the value from (e.g. "P8_16").
   :returns: Channel value–– 0 or 1.
   :rtype: int

.. function:: add_event_detect(channel, edge[, callback=None, bouncetime=0])

   Enable edge detection events for the given GPIO channel.

   :param str channel: GPIO channel to detect events from (e.g. "P8_16").
   :param int edge: edge to detect–– :data:`GPIO.RISING`, :data:`GPIO.FALLING`
       or :data:`GPIO.BOTH`
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
   :param int edge: edge to detect–– :data:`GPIO.RISING`, :data:`GPIO.FALLING`
       or :data:`GPIO.BOTH`
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

