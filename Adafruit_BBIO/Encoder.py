#!/usr/bin/python

# ===========================================================================
# Adafruit_BBIO.Encoder.QEP Class
# ===========================================================================

class QEP :

  def __init__(self, channel=1, debug=False):
    self.channel = channel
    self.debug = debug

  def errMsg(self):
    print("Error accessing 0x%02X: Check your QEP channel" % self.address)
    return -1

# example method from Adafruit_I2C
# TODO: delete this
  def write8(self, reg, value):
    "Writes an 8-bit value to the specified register/address"
    try:
      self.bus.write_byte_data(self.address, reg, value)
      if self.debug:
        print("Rotary: Wrote 0x%02X to register 0x%02X" % (value, reg))
    except IOError as err:
      return self.errMsg()


if __name__ == '__main__':
  try:
    qep = Adafruit_BBIO.Encoder.QEP()
    print("Default QEP channel is accessible")
  except:
    print("Error accessing default Rotary bus")
