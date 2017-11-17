#!/usr/bin/python

from subprocess import check_output, STDOUT, CalledProcessError
import os
import logging


class QEP :

  def __init__(self, channel=1, debug=False):
    self.channel = channel
    self.debug = debug

  def errMsg(self):
    print("Error accessing 0x%02X: Check your QEP channel" % self.address)
    return -1

# example method from Adafruit_I2C
# TODO: delete this
#  def write8(self, reg, value):
#    "Writes an 8-bit value to the specified register/address"
#    try:
#      self.bus.write_byte_data(self.address, reg, value)
#      if self.debug:
#        print("Rotary: Wrote 0x%02X to register 0x%02X" % (value, reg))
#    except IOError as err:
#      return self.errMsg()
#
#
#if __name__ == '__main__':
#  try:
#    qep = Adafruit_BBIO.Encoder.QEP()
#    print("Default QEP channel is accessible")
#  except:
#    print("Error accessing default Rotary bus")



class RotaryEncoder(object):
  # TODO: check that kernel 4.1+
  # TODO: use config-pin to set qep mode
  OCP_PATH = "/sys/devices/platform/ocp"
  _eqep_dirs = [
    '%s/48300000.epwmss/48300180.eqep' % OCP_PATH,
    '%s/48302000.epwmss/48302180.eqep' % OCP_PATH,
    '%s/48304000.epwmss/48304180.eqep' % OCP_PATH
  ]

  EQEP0 = 0
  EQEP1 = 1
  EQEP2 = 2
  EQEP2b = 3

  def _run_cmd(self, cmd):
    '''Runs a command. If not successful (i.e. error code different than zero),
    print the stderr output as a warning.
    '''

    try:
      output = check_output(cmd, stderr=STDOUT)
      self._logger.info("_run_cmd(): cmd='{}' return code={} output={}".format(
        " ".join(cmd), 0, output))
    except CalledProcessError as e:
      self._logger.warning(
        "_run_cmd(): cmd='{}' return code={} output={}".format(
          " ".join(cmd), e.returncode, e.output))

  def config_pin(self, pin):
    '''
    config_pin()
    Config pin for QEP
    '''

    self._run_cmd(["config-pin", pin, "qep"])

  def cat_file(self, path):
    '''
    cat_file()
    Print contents of file
    '''

    self._run_cmd(["cat", path])

  def __init__(self, eqep_num):
    '''
    RotaryEncoder(eqep_num)
    Creates an instance of the class RotaryEncoder.
    eqep_num determines which eQEP pins are set up.
    eqep_num can be: EQEP0, EQEP1, EQEP2 or EQEP2b based on which pins \
    the rotary encoder is connected to.
    '''

    self._logger = logging.getLogger(__name__)
    self._logger.addHandler(logging.NullHandler())

    # Configure eqep0
    self._logger.info("Configuring eqep0, pins: P9.27, P9.92")

    pin = "P9_27"
    self.config_pin(pin)

    pin = "P9_92"
    self.config_pin(pin)

    path = "/sys/devices/platform/ocp/48300000.epwmss/48300180.eqep/position"
    self.cat_file(path)

    # Configure eqep1
    self._logger.info("Configuring eqep1, pins: P8.33, P8.35")

    pin = "P8.33"
    self.config_pin(pin)

    pin = "P8.35"
    self.config_pin(pin)

    path = "/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/position"
    self.cat_file(path);

    # Configure eqep2
    self._logger.info("Configuring eqep2, pins: P8.11, P8.12")

    pin = "P8.11"
    self.config_pin(pin)

    pin = "P8.12"
    self.config_pin(pin)

    path = "/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position"
    self.cat_file(path);

    # Configure eqep2b
    self._logger.info("Configuring eqep2, pins: P8.41, P8.42")

    pin = "P8.41"
    self.config_pin(pin)

    pin = "P8.42"
    self.config_pin(pin)

    path = "/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position"
    self.cat_file(path);

    self._logger.debug("RotaryEncoder(): eqep_num: {0}".format(eqep_num))
    self._logger.debug("RotaryEncoder(): self._eqep_dirs[0]: {0}".format(self._eqep_dirs[0]))
    self._logger.debug("RotaryEncoder(): self._eqep_dirs[1]: {0}".format(self._eqep_dirs[1]))
    self._logger.debug("RotaryEncoder(): self._eqep_dirs[2]: {0}".format(self._eqep_dirs[2]))
    self._logger.debug("RotaryEncoder(): self._eqep_dirs[eqep_num: {0}]: {1}".format(eqep_num, self._eqep_dirs[eqep_num]))

    assert 0 <= eqep_num <= 3 , "eqep_num must be between 0 and 3"

    self.base_dir = self._eqep_dirs[eqep_num]
    self._logger.debug("RotaryEncoder(): self.base_dir: {0}".format(self.base_dir))

    self.enable()

  def enable(self):
    '''
    enable()
    Turns the eQEP hardware ON
    '''
    enable_file = "%s/enabled" % self.base_dir
    self._logger.debug("enable(): enable_file: {0}".format(enable_file))
    self._logger.warning(
      "enable(): TODO: not implemented, write 1 to {}".format(enable_file))
    #return sysfs.kernelFileIO(enable_file, '1')

  def disable(self):
    '''
    disable()
    Turns the eQEP hardware OFF
    '''
    enable_file = "%s/enabled" % self.base_dir
    self._logger.debug("disable(): enable_file: {0}".format(enable_file))
    self._logger.warning(
      "disable(): TODO: not implemented, write 0 to {}".format(enable_file))
    #return sysfs.kernelFileIO(enable_file, '0')

  def setAbsolute(self):
    '''
    setAbsolute()
    Set mode as Absolute
    The position starts at zero and is incremented or
    decremented by the encoder's movement
    '''
    mode_file = "%s/mode" % self.base_dir
    self._logger.debug("setAbsolute(): mode_file: {0}".format(mode_file))
    self._logger.warning(
      "setAbsolute(): TODO: not implemented, write 0 to {}".format(mode_file))
    #return sysfs.kernelFileIO(mode_file, '0')

  def setRelative(self):
    '''
    setRelative()
    Set mode as Relative
    The position is reset when the unit timer overflows.
    '''
    mode_file = "%s/mode" % self.base_dir
    self._logger.debug("setRelative(): mode_file: {0}".format(mode_file))
    self._logger.warning(
      "setRelative(): TODO: not implemented, write 1 to {}".format(mode_file))
    #return sysfs.kernelFileIO(mode_file, '1')

  def getMode(self):
    '''
    getMode()
    Returns the mode the eQEP hardware is in.
    '''
    mode_file = "%s/mode" % self.base_dir
    self._logger.debug("getMode(): mode_file: {0}".format(mode_file))
    self._logger.warning("getMode(): TODO: read mode_file")
    #return sysfs.kernelFileIO(mode_file)

  def getPosition(self):
    '''
    getPosition()
    Get the current position of the encoder.
    In absolute mode, this attribute represents the current position
    of the encoder.
    In relative mode, this attribute represents the position of the
    encoder at the last unit timer overflow.
    '''
    position_file = "%s/position" % self.base_dir
    self._logger.debug("getPosition(): position_file: {0}".format(position_file))
    position_handle = open(position_file, 'r')
    self._logger.debug("getPosition(): position_handle: {0}".format(position_handle))
    position = position_handle.read()
    self._logger.debug("getPosition(): position: {0}".format(position))
    #return sysfs.kernelFileIO(position_file)

    return position

  def setFrequency(self, freq):
    '''
    setFrequency(freq)
    Set the frequency in Hz at which the driver reports new positions.
    '''
    period_file = "%s/period" % self.base_dir
    self._logger.debug("setFrequency(): period_file: {0}".format(period_file))
    self._logger.debug("setFrequency(): freq: {0}".format(freq))
    self._logger.debug("setFrequency(): 1000000000/freq: {0}".format(1000000000/freq))
    self._logger.debug("setFrequency(): str(1000000000/freq)): {0}".format(str(1000000000/freq)))
    self._logger.warning(
      "setFrequency(): TODO: not implemented, set {} to {}".format(
        period_file, str(1000000000/freq)))
    #return sysfs.kernelFileIO(period_file, str(1000000000/freq))

  def setPosition(self, val):
    '''
    setPosition(value)
    Give a new value to the current position
    '''
    position_file = "%s/position" % self.base_dir
    self._logger.warning(
      "setPosition(): TODO: not implemented, write position to {}".format(
        position_file))
    #return sysfs.kernelFileIO(position_file, str(val))

  def zero(self):
    '''
    zero()s
    Set the current position to 0
    '''
    return self.setPosition(0)


#"""
# encoder_test.py
# Rekha Seethamraju
# An example to demonstrate the use of the eQEP library
# for PyBBIO.
# This example program is in the public domain.
#"""
#from bbio import *
#from bbio.libraries.RotaryEncoder import RotaryEncoder
#
#encoder = RotaryEncoder(RotaryEncoder.EQEP2b)
#
#def setup():
#  encoder.setAbsolute()
#  encoder.zero()
#
#def loop():
#  print("encoder position : "+encoder.getPosition())
#  delay(1000)
#
#run(setup, loop)
