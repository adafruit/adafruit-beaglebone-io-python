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
