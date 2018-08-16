:mod:`ADC` --- A/D Converter input interface
--------------------------------------------

This module enables reading analog input values from the analog to digital
converter (ADC) on the TI AM3358 SoC on the BeagleBone.

Example::

    import Adafruit_BBIO.ADC as ADC

    ADC.setup()

    # The read method returns normalized values from 0 to 1.0
    value = ADC.read("P9_40")

    # The read_raw returns non-normalized values from 0 to 4095
    value = ADC.read_raw("P9_40")

.. module:: Adafruit_BBIO.ADC

.. function:: setup_adc()

   Setup and start the ADC hardware.

.. function:: setup_read(channel)

   Read the normalized analog value for the channel.

   :param str channel: GPIO channel to read the value from (e.g. "P8_16").
   :returns: normalized value reading from 0.0 to 1.0
   :rtype: float


.. function:: setup_read_raw(channel)

   Read the raw analog value for the channel.
   
   :note: Kernels older than 4.1.x returned a raw value range 0 - 1800
       based on the reference voltage of 1.8 V.

   :param str channel: GPIO channel to read the value from (e.g. "P8_16").
   :returns: raw value reading from 0 to 4095 (12 bits).
   :rtype: float


