# Adafruit_BBIO.Encoder

This module enables access to the Beaglebone Black enhanced Quadrature Encoder Pulse (eQEP) modules: eQEP0, eQEP1 and eQEP2/eQEP2b.

## Usage

On a recent Beaglebone Debian image, access to the eQEP0 and eQEP2 channels should work out of the box, at least as root user. To ensure you can run the code as a regular user, read on the prerequisites section below.

```python
from Adafruit_BBIO.Encoder import RotaryEncoder, eQEP2

'''
Each channel can be accessed and initialized using its corresponding
channel name constants:

    eQEP0
    eQEP1  # Pins only available when video is disabled
    eQEP2
    eQEP2b # Pins only available when video is disabled
'''

# Instantiate the class to access channel eQEP2, and only initialize
# that channel
myEncoder = RotaryEncoder(eQEP2)

# Get the current position
cur_position = myEncoder.position

# Position can also be set as a property
next_position = 15
myEncoder.position = next_position

# Reset position to 0
myEncoder.zero()

# Change mode to relative (default is absolute)
# You can use setAbsolute() to change back to absolute
# Absolute: the position starts at zero and is incremented or
#           decremented by the encoder's movement
# Relative: the position is reset when the unit timer overflows.
myEncoder.setRelative()

# Read the current mode (0: absolute, 1: relative)
# Mode can also be set as a property
mode = myEncoder.mode

# Read the current frequency of update
# Returned value is in Hz
# If you want to set the frequency, specify it also in Hz
freq = myEncoder.frequency

# Disable your eQEP channel
myEncoder.disable()

# Check if the channel is enabled
# The 'enabled' property is read-only
# Use the enable() and disable() methods to
# safely enable or disable the module
isEnabled = myEncoder.enabled

```

If you need to use further channels, read on the prerequisites in the following section.

## Prerequisites

### Kernel and packages

These instructions are based on:

- Linux kernel: 4.4.x or later
- `bb-cape-overlays` package: version 4.4.20171120.0-0rcnee1~stretch+20171120 or later
- `bb-customizations` package: version 1.20171123-0rcnee0~stretch+20171123 or later

It's recommended to run the following commands to ensure you have the latest required packages:

```
sudo apt update
sudo apt upgrade bb-cape-overlays bb-customizations
```

### User permissions

In order to be able to run code that accesses the eQEP modules as a regular user, as opposed to root, that user must be part of the `eqep` group.

To check which users are part of the `eqep` group:

```
cat /etc/group | grep eqep
```

To add user `userName` to the `eqep` group (run this command as root):
```
usermod -a -G eqep userName
```

Note: you will need to log out and log back in for the group membership change to take effect.

### Capes

In order to use all eQEP pins the BeagleBone must boot with the [cape-universal](https://github.com/beagleboard/bb.org-overlays/tree/master/tools/beaglebone-universal-io) enabled, and load the `cape-universal` overlay.

This is the default, thus **no further steps are initially required to use eQEP0 and eQEP2**. Simply double-check that the following line is present and not commented out on your `/boot/uEnv.txt` file:

```
enable_uboot_cape_universal=1
```

Note: Some older documentation recommends using the `cmdline` and `cape_enable` options instead. They are meant to load deprecated kernel-based overlays and it's not recommended to use them. Use the new way of [loading overlays via uboot](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#U-Boot_Overlays) instead, as instructed above.

#### Enabling additional eQEP modules

The `cape-universal` overlay will enable access to the eQEP0 and eQEP2 modules. As it does not expose pins that are shared with the HDMI interface, eQEP1 and eQEP2b will **not** be available.

To disable the HDMI interface and gain access to the pins and peripherals that share its pins, comment out the following line on the `/boot/uEnv.txt` file and reboot:

```
disable_uboot_overlay_video=1
```

## eQEP configuraton

Notes:

- If either eQEP1 or eQEP2b are used on the Beaglebone Black, video must be disabled, as their pins are shared with the LCD_DATAx lines of the HDMI interface.
- eQEP2 and eQEP2b are the same module, but with the alternative of accessing it via two sets of pins. These are mutually exclusive.

### eQEP0

Pins: `P9_27`, `P9_92`

```
$ config-pin P9_27 qep
$ config-pin P9_92 qep # alias for P9_42.1
$ cat /sys/devices/platform/ocp/48300000.epwmss/48300180.eqep/position
```

### eQEP1

Pins: `P8.33`, `P8.35`

```
$ config-pin P8.33 qep
$ config-pin P8.35 qep
$ cat /sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/position
```

### eQEP2

#### eQEP2

Pins: `P8.11`, `P8.12`

```
$ config-pin P8.11 qep 
$ config-pin P8.12 qep
$ cat /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
```

#### eQEP2b

Note: alternate pins for eQEP2 (mutually exclusive)

Pins: `P8.41`, `P8.42`

```
$ config-pin P8.41 qep 
$ config-pin P8.42 qep
$ cat /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
```

## Credits

Initially based on the [PyBBIO](https://github.com/graycatlabs/PyBBIO/bbio/libraries/RotaryEncoder/rotary_encoder.py) rotary encoder code.

## Further reading

1. [Beaglebone encoder inputs](https://github.com/Teknoman117/beaglebot/tree/master/encoders)
1. [Beaglebone eQEP overlays](https://github.com/Teknoman117/beaglebot/tree/master/encoders/dts)
