# Adafruit_BBIO SPI test

## Loopback test with SPI0
* Connect wire from P9_18 [SPI0_D1] and P9_21 [SPI0_D0]
* Reference: [P9 header pins](http://beagleboard.org/static/images/cape-headers-spi.png)

## System Info
### uname -r
`4.4.39-ti-r79`
### /etc/debian_version 
`8.6`
### /etc/dogtag 
`BeagleBoard.org Debian Image 2016-11-27`
### /boot/uEnv.txt
```
uname_r=4.4.39-ti-r79
cmdline=coherent_pool=1M quiet cape_universal=enable
```
### /sys/devices/platform/bone_capemgr/slots
```
 0: PF----  -1 
 1: PF----  -1 
 2: PF----  -1 
 3: PF----  -1 
 4: P-O-L-   0 Override Board Name,00A0,Override Manuf,cape-universaln
```

### config-pin
```
config-pin P9_17 spi
config-pin P9_18 spi
config-pin P9_21 spi
config-pin P9_22 spi
```

## test-spi.py
```
from Adafruit_BBIO.SPI import SPI

# From Adafruit Learning System:
# https://learn.adafruit.com/setting-up-io-python-library-on-beaglebone-black/spi     
#spi = SPI(bus, device) #/dev/spidev<bus>.<device>
#spi = SPI(0,0)	#/dev/spidev1.0
#spi = SPI(0,1)	#/dev/spidev1.1
#spi = SPI(1,0)	#/dev/spidev2.0
#spi = SPI(1,1)	#/dev/spidev2.1

spi = SPI(0,0) 
print(spi.xfer2([32, 11, 110, 22, 220]))
spi.close() 
```

### output
```
debian@beaglebone:~/spi$ sudo python test-spi.py 
[32, 11, 110, 22, 220]
```

### strace
```
debian@beaglebone:~/spi$ sudo strace -e open -f python ./test-spi.py 2>&1 |grep /dev/spidev
open("/dev/spidev1.0", O_RDWR|O_LARGEFILE) = 3
```
