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

.. function:: setup_read(channel)

   :param str channel: GPIO channel to read the value from (e.g. "P8_16").

   Read the normalized 0-1.0 analog value for the channel.

.. function:: setup_read_raw(channel)

   :param str channel: GPIO channel to read the value from (e.g. "P8_16").

   Read the raw analog value for the channel.

