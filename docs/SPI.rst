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
    spi = SPI(0, 0)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close()

    # /dev/spidev0.1
    spi = SPI(0, 1)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close()

    # /dev/spidev1.0
    spi = SPI(1, 0)
    print(spi.xfer2([32, 11, 110, 22, 220]))
    spi.close()

    # /dev/spidev1.1
    spi = SPI(1, 1)
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

