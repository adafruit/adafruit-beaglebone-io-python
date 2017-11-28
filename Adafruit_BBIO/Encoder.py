#!/usr/bin/python

from subprocess import check_output, STDOUT, CalledProcessError
import os
import logging
import itertools
import platform

(major, minor, patch) = platform.release().split("-")[0].split(".")
if not (int(major) >= 4 and int(minor) >=  4):
    raise ImportError(
        'The Encoder module requires Linux kernel version >= 4.4.x.\n'
        'Please upgrade your kernel to use this module.\n'
        'Your Linux kernel version is {}.'.format(platform.release()))

eQEP0 = 0
eQEP1 = 1
eQEP2 = 2
eQEP2b = 3

_OCP_PATH = "/sys/devices/platform/ocp"
_eQEP_DEFS = [
   {'channel': 'eQEP0', 'pin_A': 'P9_92', 'pin_B': 'P9_27',
       'sys_path': os.path.join(_OCP_PATH, '48300000.epwmss/48300180.eqep')},
   {'channel': 'eQEP1', 'pin_A': 'P8_35', 'pin_B': 'P8_33',
       'sys_path': os.path.join(_OCP_PATH, '48302000.epwmss/48302180.eqep')},
   {'channel': 'eQEP2', 'pin_A': 'P8_12', 'pin_B': 'P8_11',
       'sys_path': os.path.join(_OCP_PATH, '48304000.epwmss/48304180.eqep')},
   {'channel': 'eQEP2b', 'pin_A': 'P8_41', 'pin_B': 'P8_42',
       'sys_path': os.path.join(_OCP_PATH, '48304000.epwmss/48304180.eqep')}
]


class eQEP(object):
    '''Enhanced Quadrature Encoder Pulse (eQEP) module class. Abstraction
    for either of the three available channels (eQEP0, eQEP1, eQEP2) on
    the Beaglebone'''

    @classmethod
    def fromdict(cls, d):
        '''Creates a class instance from a dictionary'''

        allowed = ('channel', 'pin_A', 'pin_B', 'sys_path')
        df = {k: v for k, v in d.iteritems() if k in allowed}
        return cls(**df)

    def __init__(self, channel, pin_A, pin_B, sys_path):
        '''Initialize the eQEP module

        Attributes:
            channel (str): eQEP channel name. E.g. "eQEP0", "eQEP1", etc.
                Note that "eQEP2" and  "eQEP2b" are channel aliases for the
                same module, but on different (mutually-exclusive) sets of
                pins
            pin_A (str): physical input pin for the A signal of the
                rotary encoder
            pin_B (str): physical input pin for the B signal of the
                rotary encoder
            sys_path (str): sys filesystem path to access the attributes
                of this eQEP module

        '''
        self.channel = channel
        self.pin_A = pin_A
        self.pin_B = pin_B
        self.sys_path = sys_path


class RotaryEncoder(object):

    def _run_cmd(self, cmd):
        '''Runs a command. If not successful (i.e. error code different than
        zero), print the stderr output as a warning.
        '''

        try:
            output = check_output(cmd, stderr=STDOUT)
            self._logger.info(
                "_run_cmd(): cmd='{}' return code={} output={}".format(
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

        # Configure eqep module
        self._eqep = eQEP.fromdict(_eQEP_DEFS[eqep_num])
        self._logger.info(
            "Configuring: {}, pin A: {}, pin B: {}, sys path: {}".format(
                self._eqep.channel, self._eqep.pin_A, self._eqep.pin_B,
                self._eqep.sys_path))

        self.config_pin(self._eqep.pin_A)
        self.config_pin(self._eqep.pin_B)

        self.base_dir = self._eqep.sys_path
        self._logger.debug(
            "RotaryEncoder(): self.base_dir: {0}".format(self.base_dir))

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
        # return sysfs.kernelFileIO(enable_file, '1')

    def disable(self):
        '''
        disable()
        Turns the eQEP hardware OFF
        '''
        enable_file = "%s/enabled" % self.base_dir
        self._logger.debug("disable(): enable_file: {0}".format(enable_file))
        self._logger.warning(
            "disable(): TODO: not implemented, write 0 to {}".format(
                enable_file))
        # return sysfs.kernelFileIO(enable_file, '0')

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
            "setAbsolute(): TODO: not implemented, write 0 to {}".format(
                mode_file))
        # return sysfs.kernelFileIO(mode_file, '0')

    def setRelative(self):
        '''
        setRelative()
        Set mode as Relative
        The position is reset when the unit timer overflows.
        '''
        mode_file = "%s/mode" % self.base_dir
        self._logger.debug("setRelative(): mode_file: {0}".format(mode_file))
        self._logger.warning(
            "setRelative(): TODO: not implemented, write 1 to {}".format(
                mode_file))
        # return sysfs.kernelFileIO(mode_file, '1')

    def getMode(self):
        '''
        getMode()
        Returns the mode the eQEP hardware is in.
        '''
        mode_file = "%s/mode" % self.base_dir
        self._logger.debug("getMode(): mode_file: {0}".format(mode_file))
        self._logger.warning("getMode(): TODO: read mode_file")
        # return sysfs.kernelFileIO(mode_file)

    def getPosition(self):
        '''
        getPosition()
        Get the current position of the encoder.
        In absolute mode, this attribute represents the current position
        of the encoder.
        In relative mode, this attribute represents the position of the
        encoder at the last unit timer overflow.
        '''
        self._logger.debug("Channel: {}".format(self._eqep.channel))
        position_file = "%s/position" % self.base_dir
        self._logger.debug(
            "getPosition(): position_file: {0}".format(position_file))
        position_handle = open(position_file, 'r')
        self._logger.debug(
            "getPosition(): position_handle: {0}".format(position_handle))
        position = position_handle.read()
        self._logger.debug("getPosition(): position: {0}".format(position))
        # return sysfs.kernelFileIO(position_file)

        return position

    def setFrequency(self, freq):
        '''
        setFrequency(freq)
        Set the frequency in Hz at which the driver reports new positions.
        '''
        period_file = "%s/period" % self.base_dir
        self._logger.debug(
            "setFrequency(): period_file: {0}".format(period_file))
        self._logger.debug("setFrequency(): freq: {0}".format(freq))
        self._logger.debug(
            "setFrequency(): 1000000000/freq: {0}".format(1000000000/freq))
        self._logger.debug("setFrequency(): str(1000000000/freq)): {0}".format(
            str(1000000000/freq)))
        self._logger.warning(
          "setFrequency(): TODO: not implemented, set {} to {}".format(
            period_file, str(1000000000/freq)))
        # return sysfs.kernelFileIO(period_file, str(1000000000/freq))

    def setPosition(self, val):
        '''
        setPosition(value)
        Give a new value to the current position
        '''
        position_file = "%s/position" % self.base_dir
        self._logger.warning(
          "setPosition(): TODO: not implemented, write position to {}".format(
            position_file))
        # return sysfs.kernelFileIO(position_file, str(val))

    def zero(self):
        '''
        zero()s
        Set the current position to 0
        '''
        return self.setPosition(0)
