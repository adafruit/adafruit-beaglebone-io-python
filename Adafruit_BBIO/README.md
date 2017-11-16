# Adafruit_BBIO.Encoder Class

Initially based on the [PyBBIO](https://github.com/graycatlabs/PyBBIO/bbio/libraries/RotaryEncoder/rotary_encoder.py) rotary encoder code.

## Prerequisites

These instructions are based on a 4.4.x Linux kernel.

In order to use all eQEP pins the BeagleBone must:

- boot with cape-universal enabled, and
- load the cape-universala overlay


### Install/upgrade the latest Device Tree overlays

```
sudo apt-get upgrade bb-cape-overlays
```

### Load the universal cape

Modify the `/boot/uEnv.txt` file to contain these two lines: 

```
cmdline=coherent_pool=1M net.ifnames=0 quiet cape_universal=enable
```

```
cape_enable=bone_capemgr.enable_partno=cape-universala
```

### Check slots

TBD

## eQEP configuraton

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

Pins: `P8.11, P8.12

```
$ config-pin P8.11 qep 
$ config-pin P8.12 qep
$ cat /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
```

### eQEP2b

Note: alternate pins for eQEP2 (mutually exclusive)
Pins: `P8.11, P8.12

```
$ config-pin P8.41 qep 
$ config-pin P8.42 qep
$ cat /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
```