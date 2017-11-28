#!/usr/bin/python

from subprocess import check_output, STDOUT, CalledProcessError
import os
import logging
import itertools
import sysfs
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
            node (str): sys filesystem device node that contains the
                readable or writable attributes to control the QEP channel

        '''
        self.channel = channel
        self.pin_A = pin_A
        self.pin_B = pin_B
        self.sys_path = sys_path
        self.node = sysfs.Node(sys_path)


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

    def __init__(self, eqep_num):
        '''Creates an instance of the class RotaryEncoder.

        Arguments:
           eqep_num: determines which eQEP pins are set up.
               Allowed values: EQEP0, EQEP1, EQEP2 or EQEP2b,
               based on which pins the physical rotary encoder
               is connected to.
            
        '''
        # Set up logging at the module level
        self._logger = logging.getLogger(__name__)
        self._logger.addHandler(logging.NullHandler())

        # Initialize the eQEP channel structures
        self._eqep = eQEP.fromdict(_eQEP_DEFS[eqep_num])
        self._logger.info(
            "Configuring: {}, pin A: {}, pin B: {}, sys path: {}".format(
                self._eqep.channel, self._eqep.pin_A, self._eqep.pin_B,
                self._eqep.sys_path))

        # Configure the pins for the given channel
        self.config_pin(self._eqep.pin_A)
        self.config_pin(self._eqep.pin_B)

        self._logger.debug(
            "RotaryEncoder(): sys node: {0}".format(self._eqep.sys_path))

        # Enable the channel upon initialization
        self.enable()

    def _setEnable(self, value):
        '''Turns the eQEP hardware ON or OFF

           value (int): 1 represents enabled, 0 is disabled
        '''
        if value < 0 or value > 1:
            raise ValueError(
                'The "enabled" attribute can only be set to 0 or 1. '
                'You attempted to set it to {}.'.format(value))

        self._eqep.node.enabled = str(int(value))
        self._logger.info("Channel: {}, enabled: {}".format(
            self._eqep.channel, self._eqep.node.enabled))       

    def enable(self):
        '''Turns the eQEP hardware ON'''

        self._setEnable(1)

    def disable(self):
        '''Turns the eQEP hardware OFF'''

        self._setEnable(0)

    def _setMode(self, value):
        '''Sets the eQEP mode as absolute (0) or relative (1).
        See the setAbsolute() and setRelative() methods for
        more information.

        '''
        if value < 0 or value > 1:
            raise ValueError(
                'The "mode" attribute can only be set to 0 or 1. '
                'You attempted to set it to {}.'.format(value))

        self._eqep.node.mode = str(int(value))
        self._logger.debug("Mode set to: {}".format(
            self._eqep.node.mode))  

    def setAbsolute(self):
        '''Sets the eQEP mode as Absolute:
        The position starts at zero and is incremented or
        decremented by the encoder's movement

        '''
        self._setMode(0)

    def setRelative(self):
        '''Sets the eQEP mode as Relative:
        The position is reset when the unit timer overflows.

        '''
        self._setMode(1)

    def getMode(self):
        '''Returns the mode the eQEP hardware is in (absolute or relative).

        '''

        mode = int(self._eqep.node.mode)
        
        if mode == 0:
            mode_name = "absolute"
        elif mode == 1:
            mode_name = "relative"
        else:
            mode_name = "invalid" 

        self._logger.debug("getMode(): Channel {}, mode: {} ({})".format(
            self._eqep.channel, mode, mode_name))

        return mode

    def getPosition(self):
        '''Returns the current position of the encoder.
        In absolute mode, this attribute represents the current position
        of the encoder.
        In relative mode, this attribute represents the position of the
        encoder at the last unit timer overflow.

        '''
        position = self._eqep.node.position

        self._logger.debug("getPosition(): Channel {}, position: {}".format(
            self._eqep.channel, position))

        return int(position)

    def setFrequency(self, freq):
        '''Sets the frequency in Hz at which the driver reports
        new positions.

        '''
        ns_factor = 1000000000
        period = ns_factor/freq  # Period in nanoseconds
        self._eqep.node.period = str(period)
        self._logger.debug(
            "setFrequency(): Channel {}, frequency: {} Hz, "
            "period: {} ns".format(
                self._eqep.channel, freq, period))

    def setPosition(self, position):
        '''Sets the current position to a new value'''

        position = int(position)
        self._eqep.node.position = str(position)

    def zero(self):
        '''Resets the current position to 0'''

        return self.setPosition(0)
