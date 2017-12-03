I have  documented how-to to enable all the eqep pins:
https://github.com/adafruit/adafruit-beaglebone-io-python/commit/c418cdae9a2a2c0d52412561c0125b0d227af4eb

BeagleBone must boot with cape-universal enabled and load the `cape-universala` overlay in order to
use all the eQEP pins:

### Install the latest Device Tree overlays:
```
sudo apt-get upgrade bb-cape-overlays
```

### File: /boot/uEnv.txt
```
uname_r=4.4.62-ti-r99
cmdline=coherent_pool=1M quiet cape_universal=enable
cape_enable=bone_capemgr.enable_partno=cape-universala
```

### File: /sys/devices/platform/bone_capemgr/slots
```
0: PF----  -1 
1: PF----  -1 
2: PF----  -1 
3: PF----  -1 
4: P-O-L-   0 Override Board Name,00A0,Override Manuf,cape-universala
```

### eqep0: P9_27, P9_92
```
config-pin P9_27 qep
config-pin P9_92 qep # alias for P9_42.1
cat /sys/devices/platform/ocp/48300000.epwmss/48300180.eqep/position
```

### eqep1: P8.33, P8.35
```
config-pin P8.33 qep 
config-pin P8.35 qep
cat /sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/position
```

### eqep2: P8.11, P8.12
```
config-pin P8.11 qep 
config-pin P8.12 qep 
cat /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
```

### eqep2b: P8.41, P8.42
_alternate pins for eqep2 (mutually exclusive)_
```
config-pin P8.41 qep 
config-pin P8.42 qep 
cat /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
```

### TODO: implement in corresponding methods in `Encoder.py`


## Read QEP1
* Updated: 2017-08-31
* I was able to enable QEP1 with the following:

#### /boot/uEnv.txt
```
uname_r=4.9.35-ti-r44
enable_uboot_overlays=1
disable_uboot_overlay_video=1
cmdline=coherent_pool=1M net.ifnames=0 cape_universal=enable
cape_enable=bone_capemgr.enable_partno=cape-universala
```

#### Read QEP1:
```
$ config-pin P8.33 qep
$ config-pin P8.35 qep
$ cat /sys/devices/platform/ocp/ocp\:P8_33_pinmux/state
qep
$ cat /sys/devices/platform/ocp/ocp\:P8_35_pinmux/state
qep
$ cat /sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/position
0
```

#### Version info:
```
$ sudo /opt/scripts/tools/version.sh
git:/opt/scripts/:[2ce750d881941c5189db9e189af90517e11c079f]
eeprom:[A335BNLT000C3014BBBK1316]
dogtag:[BeagleBoard.org Debian Image 2017-06-18]
bootloader:[microSD-(push-button)]:[/dev/mmcblk0]:[U-Boot
2017.09-rc2-00002-g7c9353]
bootloader:[eMMC-(default)]:[/dev/mmcblk1]:[U-Boot 2017.09-rc2-00002-g84a7f2]
kernel:[4.9.35-ti-r44]
nodejs:[v4.8.4]
uboot_overlay_options:[enable_uboot_overlays=1]
uboot_overlay_options:[disable_uboot_overlay_video=1]
pkg:[bb-cape-overlays]:[4.4.20170728.0-0rcnee1~jessie+20170728]
pkg:[bb-wl18xx-firmware]:[1.20170612-0rcnee0~jessie+20170612]
WARNING:pkg:[firmware-ti-connectivity]:[NOT_INSTALLED]
```
