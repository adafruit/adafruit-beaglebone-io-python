I'm using P8.11 and P8.12 for testing a rotary encoder.

Using cape-universal:
```
debian@beaglebone:~$ cat /sys/devices/platform/bone_capemgr/slots
 0: PF----  -1 
 1: PF----  -1 
 2: PF----  -1 
 3: PF----  -1 
 4: P-O-L-   0 Override Board Name,00A0,Override Manuf,univ-all
 5: P-O-L-   1 Override Board Name,00A0,Override Manuf,BB-ADC
```

/boot/uEnv.txt:
```
uname_r=4.4.47-ti-r87
dtb=am335x-boneblack-overlay.dtb
cmdline=coherent_pool=1M quiet cape_universal=enable
```

Configure the pin mode for eQEP:
```
debian@beaglebone:~$ config-pin p8.11 qep && config-pin p8.12 qep

debian@beaglebone:~$ config-pin -q p8.11 && config-pin -q p8.12 
P8_11 Mode: qep
P8_12 Mode: qep
```

System info:
```
debian@beaglebone:~$ uname -a
Linux beaglebone 4.4.47-ti-r87 #1 SMP Mon Feb 6 22:21:49 UTC 2017 armv7l GNU/Linux
debian@beaglebone:~$ cat /etc/debian_version 
8.7
debian@beaglebone:~$ sudo cat /etc/dogtag 
BeagleBoard.org Debian Image 2015-12-07
```

Check the position:
```
debian@beaglebone:~$ find /sys/devices |grep eqep |grep position$
/sys/devices/platform/ocp/48302000.epwmss/48302180.eqep/position
/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
/sys/devices/platform/ocp/48300000.epwmss/48300180.eqep/position
debian@beaglebone:~$ cat `find /sys/devices |grep eqep |grep position$`
0
-3251
0
debian@beaglebone:~$ watch -n 1 cat /sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
```

For P8.11 and P8.12:
```
/sys/devices/platform/ocp/48304000.epwmss/48304180.eqep/position
```
