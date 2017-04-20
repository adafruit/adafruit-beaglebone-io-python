#!/usr/bin/python

# ===========================================================================
# Adafruit_BBIO.Encoder.QEP Class
# ===========================================================================

# refers to graycatlabs/PyBBIO/bbio/libraries/RotaryEncoder/rotary_encoder.py
# /sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/position
# /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
# /sys/devices/platform/ocp/48300000.epwmss/48300180.eqep/position

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
  OCP_PATH = "/sys/TEST/TODO/"
  _eqep_dirs = [
    '%s/48300000.epwmss/48300180.eqep' % OCP_PATH,
    '%s/48302000.epwmss/48302180.eqep' % OCP_PATH,
    '%s/48304000.epwmss/48304180.eqep' % OCP_PATH
  ]
  EQEP0 = 0
  EQEP1 = 1
  EQEP2 = 2
  EQEP2b = 3
    
  def __init__(self, eqep_num):
    '''
    RotaryEncoder(eqep_num)
    Creates an instance of the class RotaryEncoder. 
    eqep_num determines which eQEP pins are set up. 
    eqep_num can be: EQEP0, EQEP1, EQEP2 or EQEP2b based on which pins \
    the rotary encoder is connected to.
    '''
    assert 0 <= eqep_num <= 3 , "eqep_num must be between 0 and 3"
    if eqep_num == 3:
        overlay = "PyBBIO-eqep2b"
    	eqep_num = 2
    else:
    	overlay = 'PyBBIO-eqep%i' % eqep_num

    pwmss_overlay = "PyBBIO-epwmss%i" % eqep_num
    #cape_manager.load(pwmss_overlay, auto_unload=False)
    delay(10)
    #cape_manager.load(overlay, auto_unload=False)
    delay(250) # Give driver time to load 
    self.base_dir = self._eqep_dirs[eqep_num]
    self.enable()
    addToCleanup(self.disable)
    
  def enable(self):
    '''
    enable()
    Turns the eQEP hardware ON
    '''
    enable_file = "%s/enabled" % self.base_dir
    #return sysfs.kernelFileIO(enable_file, '1') 
    
  def disable(self):
    '''
    disable()
    Turns the eQEP hardware OFF
    '''
    enable_file = "%s/enabled" % self.base_dir
    #return sysfs.kernelFileIO(enable_file, '0')

  def setAbsolute(self):
    '''
    setAbsolute()
    Set mode as Absolute
    The position starts at zero and is incremented or 
    decremented by the encoder's movement
    '''
    mode_file = "%s/mode" % self.base_dir
    #return sysfs.kernelFileIO(mode_file, '0')
    
  def setRelative(self):
    '''
    setRelative()
    Set mode as Relative
    The position is reset when the unit timer overflows.
    '''
    mode_file = "%s/mode" % self.base_dir
    #return sysfs.kernelFileIO(mode_file, '1')
    
  def getMode(self):
    '''
    getMode()
    Returns the mode the eQEP hardware is in.
    '''
    mode_file = "%s/mode" % self.base_dir
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
    #return sysfs.kernelFileIO(position_file)
    
  def setFrequency(self,freq):
    '''
    setFrequency(freq)
    Set the frequency in Hz at which the driver reports new positions.
    '''
    period_file = "%s/period" % self.base_dir
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
#  print "encoder position : "+encoder.getPosition()
#  delay(1000)
#  
#run(setup, loop)
