#!/usr/bin/python

from subprocess import check_output, STDOUT, CalledProcessError
import os
import logging
import itertools
from .sysfs import Node
import platform

(major, minor, patch) = platform.release().split("-")[0].split(".")
if not (int(major) >= 4 and int(minor) >=  4):
    raise ImportError(
        'The Encoder module requires Linux kernel version >= 4.4.x.\n'
        'Please upgrade your kernel to use this module.\n'
        'Your Linux kernel version is {}.'.format(platform.release()))


# eQEP module channel identifiers
# eQEP 2 and 2b are the same channel, exposed on two different sets of pins,
# which are mutually exclusive
eQEP0 = 0
eQEP1 = 1
eQEP2 = 2
eQEP2b = 3

# Definitions to initialize the eQEP modules
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
        df = {k: v for k, v in d.items() if k in allowed}
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
        self.node = Node(sys_path)


class RotaryEncoder(object):
    '''
    Rotary encoder class abstraction to control a given QEP channel.

    Constructor:
        eqep_num: QEP object that determines which channel to control

    Properties:
        position: current position of the encoder
        frequency: frequency at which the encoder reports new positions
        enabled: (read only) true if the module is enabled, false otherwise
        mode: current mode of the encoder (absolute: 0, relative: 1)

    Methods:
        enable: enable the QEP channel
        disable: disable the QEP channel
        setAbsolute: shortcut for setting the mode to absolute
        setRelative: shortcut for setting the mode to relative
        zero: shortcut for setting the position to 0
    '''

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

    def _config_pin(self, pin):
        '''Configures a pin in QEP mode using the `config-pin` binary'''

        self._run_cmd(["config-pin", pin, "qep"])

    def __init__(self, eqep_num):
        '''Creates an instance of the class RotaryEncoder.

        Arguments:
           eqep_num: determines which eQEP pins are set up.
               Allowed values: EQEP0, EQEP1, EQEP2 or EQEP2b,
               based on which pins the physical rotary encoder
               is connected to.

        '''
        # nanoseconds factor to convert period to frequency and back
        self._NS_FACTOR = 1000000000

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
        self._config_pin(self._eqep.pin_A)
        self._config_pin(self._eqep.pin_B)

        self._logger.debug(
            "RotaryEncoder(): sys node: {0}".format(self._eqep.sys_path))

        # Enable the channel upon initialization
        self.enable()

    @property
    def enabled(self):
        '''Returns the enabled status of the module:

            true: module is enabled
            false: module is disabled
        '''
        isEnabled = bool(int(self._eqep.node.enabled))

        return isEnabled

    def _setEnable(self, enabled):
        '''Turns the eQEP hardware ON or OFF

           value (int): 1 represents enabled, 0 is disabled

        '''
        enabled = int(enabled)
        if enabled < 0 or enabled > 1:
            raise ValueError(
                'The "enabled" attribute can only be set to 0 or 1. '
                'You attempted to set it to {}.'.format(enabled))

        self._eqep.node.enabled = str(enabled)
        self._logger.info("Channel: {}, enabled: {}".format(
            self._eqep.channel, self._eqep.node.enabled))

    def enable(self):
        '''Turns the eQEP hardware ON'''

        self._setEnable(1)

    def disable(self):
        '''Turns the eQEP hardware OFF'''

        self._setEnable(0)

    @property
    def mode(self):
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

    @mode.setter
    def mode(self, mode):
        '''Sets the eQEP mode as absolute (0) or relative (1).
        See the setAbsolute() and setRelative() methods for
        more information.

        '''
        mode = int(mode)
        if mode < 0 or mode > 1:
            raise ValueError(
                'The "mode" attribute can only be set to 0 or 1. '
                'You attempted to set it to {}.'.format(mode))

        self._eqep.node.mode = str(mode)
        self._logger.debug("Mode set to: {}".format(
            self._eqep.node.mode))

    def setAbsolute(self):
        '''Sets the eQEP mode as Absolute:
        The position starts at zero and is incremented or
        decremented by the encoder's movement

        '''
        self.mode = 0

    def setRelative(self):
        '''Sets the eQEP mode as Relative:
        The position is reset when the unit timer overflows.

        '''
        self.mode = 1

    @property
    def position(self):
        '''Returns the current position of the encoder.
        In absolute mode, this attribute represents the current position
        of the encoder.
        In relative mode, this attribute represents the position of the
        encoder at the last unit timer overflow.

        '''
        position = self._eqep.node.position

        self._logger.debug("Get position: Channel {}, position: {}".format(
            self._eqep.channel, position))

        return int(position)

    @position.setter
    def position(self, position):
        '''Sets the current position to a new value'''

        position = int(position)
        self._eqep.node.position = str(position)

        self._logger.debug("Set position: Channel {}, position: {}".format(
            self._eqep.channel, position))


    @property
    def frequency(self):
        '''Sets the frequency in Hz at which the driver reports
        new positions.

        '''
        frequency = self._NS_FACTOR / int(self._eqep.node.period) 

        self._logger.debug(
            "Set frequency(): Channel {}, frequency: {} Hz, "
            "period: {} ns".format(
                self._eqep.channel, frequency,
                self._eqep.node.period))

        return frequency

    @frequency.setter
    def frequency(self, frequency):
        '''Sets the frequency in Hz at which the driver reports
        new positions.

        '''
        period = self._NS_FACTOR / frequency  # Period in nanoseconds
        self._eqep.node.period = str(period)
        self._logger.debug(
            "Set frequency(): Channel {}, frequency: {} Hz, "
            "period: {} ns".format(
                self._eqep.channel, frequency, period))

    def zero(self):
        '''Resets the current position to 0'''

        self.position = 0
