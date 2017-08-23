#!/usr/bin/python

# ===========================================================================
# Adafruit_BBIO.Encoder Class
# ===========================================================================
# refers to graycatlabs/PyBBIO/bbio/libraries/RotaryEncoder/rotary_encoder.py

# BeagleBone must boot with cape-universal enabled
# and load the cape-universala overlay in order to
# use all the eQEP pins
#
# Install the latest Device Tree overlays:
# ========================================
# sudo apt-get upgrade bb-cape-overlays
#
# File: /boot/uEnv.txt
# ====================
# uname_r=4.4.62-ti-r99
# cmdline=coherent_pool=1M quiet cape_universal=enable
# cape_enable=bone_capemgr.enable_partno=cape-universala
#
# File: /sys/devices/platform/bone_capemgr/slots
# ==============================================
# 0: PF----  -1 
# 1: PF----  -1 
# 2: PF----  -1 
# 3: PF----  -1 
# 4: P-O-L-   0 Override Board Name,00A0,Override Manuf,cape-universala
#
# eqep0: P9_27, P9_92
# ===================
# config-pin P9_27 qep
# config-pin P9_92 qep # alias for P9_42.1
# cat /sys/devices/platform/ocp/48300000.epwmss/48300180.eqep/position
#
# eqep1: P8.33, P8.35
# ===================
# config-pin P8.33 qep 
# config-pin P8.35 qep
# cat /sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/position
# 
# eqep2: P8.11, P8.12
# ===================
# config-pin P8.11 qep 
# config-pin P8.12 qep 
# cat /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
#
# alternate pins for eqep2 (mutually exclusive)
# eqep2b: P8.41, P8.42
# ====================
# config-pin P8.41 qep 
# config-pin P8.42 qep 
# cat /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position

from subprocess import call
import os

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
 
  def config_pin(self, pin):
    '''
    config_pin()
    Config pin for QEP
    '''
    result = call(["config-pin", pin, "qep"])
    print("config_pin> pin={0} result={1}".format(pin, result))
    return result
  
  def cat_file(self, path):
    '''
    cat_file()
    Print contents of file
    '''
    result = call(["cat", path])
    print("cat_file> path={0} result={1}".format(path, result))
    return result
   
  def __init__(self, eqep_num):
    '''
    RotaryEncoder(eqep_num)
    Creates an instance of the class RotaryEncoder. 
    eqep_num determines which eQEP pins are set up. 
    eqep_num can be: EQEP0, EQEP1, EQEP2 or EQEP2b based on which pins \
    the rotary encoder is connected to.
    '''
    print(">>>>>>>> TEST CALL BEGIN")

    ###################################
    print(">>>>>> eqep0: P9_27, P9_92")
    pin = "P9_27"
    self.config_pin(pin)

    pin = "P9_92"
    self.config_pin(pin)

    path = "/sys/devices/platform/ocp/48300000.epwmss/48300180.eqep/position"
    self.cat_file(path);

    ###################################
    print(">>>>>>> eqep1: P8.33, P8.35")

    pin = "P8.33"
    self.config_pin(pin)

    pin = "P8.35"
    self.config_pin(pin)

    path = "/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/position"
    self.cat_file(path);

    ###################################
    print(">>>>>>> eqep2: P8.11, P8.12")

    pin = "P8.11"
    self.config_pin(pin)

    pin = "P8.12"
    self.config_pin(pin)

    path = "/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position"
    self.cat_file(path);

    ###################################
    print(">>>>>>> eqep2b: P8.41, P8.42")

    pin = "P8.41"
    self.config_pin(pin)

    pin = "P8.42"
    self.config_pin(pin)

    path = "/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position"
    self.cat_file(path);

    ###################################
    print(">>>>>>>> TEST CALL END")

    print("RotaryEncoder(): eqep_num: {0}".format(eqep_num))
    print("RotaryEncoder(): self._eqep_dirs[0]: {0}".format(self._eqep_dirs[0]))
    print("RotaryEncoder(): self._eqep_dirs[1]: {0}".format(self._eqep_dirs[1]))
    print("RotaryEncoder(): self._eqep_dirs[2]: {0}".format(self._eqep_dirs[2]))
    print("RotaryEncoder(): self._eqep_dirs[eqep_num: {0}]: {1}".format(eqep_num, self._eqep_dirs[eqep_num]))
    assert 0 <= eqep_num <= 3 , "eqep_num must be between 0 and 3"
    self.base_dir = self._eqep_dirs[eqep_num]
    print("RotaryEncoder(): self.base_dir: {0}".format(self.base_dir))
    self.enable()

  def enable(self):
    '''
    enable()
    Turns the eQEP hardware ON
    '''
    enable_file = "%s/enabled" % self.base_dir
    print("enable(): enable_file: {0}".format(enable_file))
    print("enable(): TODO: write 1 to enable_file")
    #return sysfs.kernelFileIO(enable_file, '1') 
    
  def disable(self):
    '''
    disable()
    Turns the eQEP hardware OFF
    '''
    enable_file = "%s/enabled" % self.base_dir
    print("disable(): enable_file: {0}".format(enable_file))
    print("disable(): TODO: write 0 to enable_file")
    #return sysfs.kernelFileIO(enable_file, '0')

  def setAbsolute(self):
    '''
    setAbsolute()
    Set mode as Absolute
    The position starts at zero and is incremented or 
    decremented by the encoder's movement
    '''
    mode_file = "%s/mode" % self.base_dir
    print("setAbsolute(): mode_file: {0}".format(mode_file))
    print("setAbsolute(): TODO: write 0 to mode_file")
    #return sysfs.kernelFileIO(mode_file, '0')
    
  def setRelative(self):
    '''
    setRelative()
    Set mode as Relative
    The position is reset when the unit timer overflows.
    '''
    mode_file = "%s/mode" % self.base_dir
    print("setRelative(): mode_file: {0}".format(mode_file))
    print("setRelative(): TODO: write 1 to mode_file")
    #return sysfs.kernelFileIO(mode_file, '1')
    
  def getMode(self):
    '''
    getMode()
    Returns the mode the eQEP hardware is in.
    '''
    mode_file = "%s/mode" % self.base_dir
    print("getMode(): mode_file: {0}".format(mode_file))
    print("getMode(): TODO: read mode_file")
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
    print("getPosition(): position_file: {0}".format(position_file))
    position_handle = open(position_file, 'r')
    print("getPosition(): position_handle: {0}".format(position_handle))
    position = position_handle.read()
    print("getPosition(): position: {0}".format(position))
    #return sysfs.kernelFileIO(position_file)
    return position
    
  def setFrequency(self,freq):
    '''
    setFrequency(freq)
    Set the frequency in Hz at which the driver reports new positions.
    '''
    period_file = "%s/period" % self.base_dir
    print("setFrequency(): period_file: {0}".format(period_file))
    print("setFrequency(): freq: {0}".format(period_file))
    print("setFrequency(): freq: {0}".format(freq))
    print("setFrequency(): 1000000000/freq: {0}".format(1000000000/freq))
    print("setFrequency(): str(1000000000/freq)): {0}".format(str(1000000000/freq)))
    print("setFrequency(): TODO: set period_file: {0}".format(str(1000000000/freq)))
    #return sysfs.kernelFileIO(period_file, str(1000000000/freq))
    
  def setPosition(self,val):
    ''' 
    setPosition(value)
    Give a new value to the current position
    '''
    position_file = "%s/position" % self.base_dir
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
