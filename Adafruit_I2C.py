#!/usr/bin/python

import smbus

# ===========================================================================
# Adafruit_I2C Class
# Adafruit_I2C.py is essentially a fork of the Adafruit Raspberry Pi I2C module. 
# Any pull requests for this module should be directed to the following, and I 
# can pull them. I'd rather not deviate from the original:
# https://github.com/adafruit/Adafruit-Raspberry-Pi-Python-Code/tree/master/Adafruit_I2C
# ===========================================================================

class Adafruit_I2C :

  def __init__(self, address, busnum=-1, debug=False):
    self.address = address
    self.bus = smbus.SMBus(busnum if busnum >= 0 else 1)
    self.debug = debug

  def reverseByteOrder(self, data):
    "Reverses the byte order of an int (16-bit) or long (32-bit) value"
    # Courtesy Vishal Sapre
    byteCount = len(hex(data)[2:].replace('L','')[::2])
    val       = 0
    for i in range(byteCount):
      val    = (val << 8) | (data & 0xff)
      data >>= 8
    return val

  def errMsg(self):
    print("Error accessing 0x%02X: Check your I2C address" % self.address)
    return -1

  def write8(self, reg, value):
    "Writes an 8-bit value to the specified register/address"
    try:
      self.bus.write_byte_data(self.address, reg, value)
      if self.debug:
        print("I2C: Wrote 0x%02X to register 0x%02X" % (value, reg))
    except IOError as err:
      return self.errMsg()

  def write16(self, reg, value):
    "Writes a 16-bit value to the specified register/address pair"
    try:
      self.bus.write_word_data(self.address, reg, value)
      if self.debug:
        print(("I2C: Wrote 0x%02X to register pair 0x%02X,0x%02X" %
         (value, reg, reg+1)))
    except IOError as err:
      return self.errMsg()

  def writeList(self, reg, list):
    "Writes an array of bytes using I2C format"
    try:
      if self.debug:
        print("I2C: Writing list to register 0x%02X:" % reg)
        print(list)
      self.bus.write_i2c_block_data(self.address, reg, list)
    except IOError as err:
      return self.errMsg()

  def readList(self, reg, length):
    "Read a list of bytes from the I2C device"
    try:
      results = self.bus.read_i2c_block_data(self.address, reg, length)
      if self.debug:
        print(("I2C: Device 0x%02X returned the following from reg 0x%02X" %
         (self.address, reg)))
        print(results)
      return results
    except IOError as err:
      return self.errMsg()

  def readU8(self, reg):
    "Read an unsigned byte from the I2C device"
    try:
      result = self.bus.read_byte_data(self.address, reg)
      if self.debug:
        print(("I2C: Device 0x%02X returned 0x%02X from reg 0x%02X" %
         (self.address, result & 0xFF, reg)))
      return result
    except IOError as err:
      return self.errMsg()

  def readS8(self, reg):
    "Reads a signed byte from the I2C device"
    try:
      result = self.bus.read_byte_data(self.address, reg)
      if result > 127: result -= 256
      if self.debug:
        print(("I2C: Device 0x%02X returned 0x%02X from reg 0x%02X" %
         (self.address, result & 0xFF, reg)))
      return result
    except IOError as err:
      return self.errMsg()

  def readU16(self, reg):
    "Reads an unsigned 16-bit value from the I2C device"
    try:
      result = self.bus.read_word_data(self.address,reg)
      if (self.debug):
        print("I2C: Device 0x%02X returned 0x%04X from reg 0x%02X" % (self.address, result & 0xFFFF, reg))
      return result
    except IOError as err:
      return self.errMsg()

  def readS16(self, reg):
    "Reads a signed 16-bit value from the I2C device"
    try:
      result = self.bus.read_word_data(self.address,reg)
      if (self.debug):
        print("I2C: Device 0x%02X returned 0x%04X from reg 0x%02X" % (self.address, result & 0xFFFF, reg))
      return result
    except IOError as err:
      return self.errMsg()

if __name__ == '__main__':
  try:
    bus = Adafruit_I2C(address=0)
    print("Default I2C bus is accessible")
  except:
    print("Error accessing default I2C bus")
