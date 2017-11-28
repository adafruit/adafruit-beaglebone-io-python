# Adafruit_BBIO.Encoder

This module enables access to the Beaglebone Black enhanced Quadrature Encoder Pulse (eQEP) modules: eQEP0, eQEP1 and eQEP2/eQEP2b.

## Usage

On a recent Beaglebone Debian image, access to the eQEP0 and eQEP2 channels should work out of the box:

```python
import Adafruit_BBIO.Encoder as Encoder

'''
Each channel can be accessed and initialized using its corresponding
channel name constants:

    Encoder.eQEP0
    Encoder.eQEP1  # Pins only available when video is disabled
    Encoder.eQEP2
    Encoder.eQEP2b # Pins only available when video is disabled
'''

# Instantiate the class to access channel eQEP2, and only initialize
# that channel
myEncoder = Encoder.RotaryEncoder(Encoder.eQEP2)
```

If you need to use further channels, read on the prerequisites in the following section.

## Prerequisites

These instructions are based on:

- Linux kernel: 4.4.x or later
- `bb-cape-overlays` package: version 4.4.20171120.0-0rcnee1~stretch+20171120 or later
- `bb-customizations` package: version 1.20171123-0rcnee0~stretch+20171123 or later

It's recommended to run the following commands to ensure you have the latest required packages:

```
sudo apt update
sudo apt upgrade bb-cape-overlays bb-customizations
```

In order to use all eQEP pins the BeagleBone must boot with the [cape-universal](https://github.com/beagleboard/bb.org-overlays/tree/master/tools/beaglebone-universal-io) enabled, and load the `cape-universal` overlay.

This is the default, thus **no further steps are initially required to use eQEP0 and eQEP2**. Simply double-check that the following line is present and not commented out on your `/boot/uEnv.txt` file:

```
enable_uboot_cape_universal=1
```

Note: Some older documentation recommends using the `cmdline` and `cape_enable` options instead. They are meant to load deprecated kernel-based overlays and it's not recommended to use them. Use the new way of [loading overlays via uboot](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#U-Boot_Overlays) instead, as instructed above.

### Enabling additional eQEP modules

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
