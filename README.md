# Adafruit BeagleBone I/O Python Library (Adafruit_BBIO)

* Adafruit_BBIO is a set of Python tools to allow [GPIO](README.md#gpio-setup), [PWM](README.md#pwm), [ADC](README.md#adc) and [UART](README.md#uart) access on the BeagleBone

* It is recommended to use an [official BeagleBoard.org Debian image](https://beagleboard.org/latest-images)
  * **Currently recommended image: [Debian 9.2 "Stretch" iot (2017-10-29)](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#microSD.2FStandalone:_.28stretch-iot.29_.28All_BeagleBone_Variants_.26_PocketBeagle.29)**
    * Install [Linux kernel](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#Kernel_Options) [4.14.x](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#Mainline_.284.14.x_lts.29) to enable [non-root control of GPIO](https://github.com/rcn-ee/repos/blob/master/bb-customizations/suite/stretch/debian/80-gpio-noroot.rules) and [PWM](https://github.com/rcn-ee/repos/blob/master/bb-customizations/suite/stretch/debian/81-pwm-noroot.rules) [_(commit)_](https://github.com/adafruit/adafruit-beaglebone-io-python/commit/b65cbf8e41b444bad7c4ef6cfd4f88a30210fd78)

* Adafruit_BBIO supports Linux kernels 3.8 through 4.14

* New versions of Adafruit_BBIO may break backwards compatibility. Please read the [changelog](CHANGELOG.md).

## Installation on Debian

Easiest:
```
sudo ntpdate pool.ntp.org
sudo apt-get update
sudo apt-get install build-essential python-dev python-pip -y
sudo pip install Adafruit_BBIO
```
    
Manual:
```
sudo ntpdate pool.ntp.org
sudo apt-get update
sudo apt-get install build-essential python-dev python-pip -y
git clone git://github.com/adafruit/adafruit-beaglebone-io-python.git
cd adafruit-beaglebone-io-python
sudo python setup.py install
```

Upgrade Adafruit_BBIO to latest version on [PyPI](https://pypi.python.org/pypi/Adafruit_BBIO):
```
sudo pip install --upgrade Adafruit_BBIO
```
    
## Usage

Using the library is very similar to the excellent RPi.GPIO library used on the Raspberry Pi. Below are some examples.

### config-pin

[config-pin](https://github.com/beagleboard/bb.org-overlays/tree/master/tools/beaglebone-universal-io) is now used on the official BeagleBoard.org Debian Jessie and Stretch images to control pin mode (e.g. pin mux).

```
debian@beaglebone:~$ config-pin -q P9_14
P9_14 Mode: pwm

debian@beaglebone:~$ config-pin -l P9_14
default gpio gpio_pu gpio_pd pwm

debian@beaglebone:~$ config-pin P9_14 gpio

debian@beaglebone:~$ config-pin -q P9_14
P9_14 Mode: gpio Direction: in Value: 0

debian@beaglebone:~$ config-pin P9_14 pwm

debian@beaglebone:~$ config-pin -q P9_14
P9_14 Mode: pwm
```

### GPIO Setup

Import the library, and setup as GPIO.OUT or GPIO.IN::

    import Adafruit_BBIO.GPIO as GPIO
    GPIO.setup("P8_14", GPIO.OUT)

You can also refer to the pin names::

    GPIO.setup("GPIO0_26", GPIO.OUT)

### GPIO Output

Setup the pin for output, and write GPIO.HIGH or GPIO.LOW. Or you can use 1 or 0.::

    import Adafruit_BBIO.GPIO as GPIO
    GPIO.setup("P8_14", GPIO.OUT)
    GPIO.output("P8_14", GPIO.HIGH)

### On-Board LEDs

On-board LEDs (USR0-USR3) are handled by LED class driver rather than the GPIO pin driver.

They have a different path in the /sys/ filesystem.

Setup the pin for output and write GPIO.HIGH or GPIO.LOW::

    import Adafruit_BBIO.GPIO as GPIO
    import time
    
    for i in range(4):
        GPIO.setup("USR%d" % i, GPIO.OUT)

    while True:
        for i in range(4):
            GPIO.output("USR%d" % i, GPIO.HIGH)
            time.sleep(1)
        for i in range(4):
            GPIO.output("USR%d" % i, GPIO.LOW)
            time.sleep(1)
    
### GPIO Input

Inputs work similarly to outputs.:

    import Adafruit_BBIO.GPIO as GPIO
    GPIO.setup("P8_14", GPIO.IN)
    
Polling inputs:
    
    if GPIO.input("P8_14"):
      print("HIGH")
    else:
      print("LOW")

Waiting for an edge (GPIO.RISING, GPIO.FALLING, or GPIO.BOTH:

    GPIO.wait_for_edge(channel, GPIO.RISING)

    or
    
    GPIO.wait_for_edge(channel, GPIO.RISING, timeout)

Detecting events:

    GPIO.add_event_detect("P9_12", GPIO.FALLING) 
    #your amazing code here 
    #detect wherever: 
    if GPIO.event_detected("P9_12"):
      print "event detected!"

### PWM
**The PWM Duty Cycle range was reversed in 0.0.15 from 100(off)-0(on) to 0(off)-100(on).  Please update your code accordingly.**

    import Adafruit_BBIO.PWM as PWM 
    #PWM.start(channel, duty, freq=2000, polarity=0) 
    #duty values are valid 0 (off) to 100 (on) 
    PWM.start("P9_14", 50)
    PWM.set_duty_cycle("P9_14", 25.5) 
    PWM.set_frequency("P9_14", 10)

    PWM.stop("P9_14")
    PWM.cleanup()
    
    #set polarity to 1 on start:
    PWM.start("P9_14", 50, 2000, 1)

### ADC

    import Adafruit_BBIO.ADC as ADC
    ADC.setup()

    #read returns values 0-1.0 
    value = ADC.read("P9_40")

    #read_raw returns non-normalized value 
    value = ADC.read_raw("P9_40")

### [UART](https://learn.adafruit.com/setting-up-io-python-library-on-beaglebone-black/uart)
* Use [`config-pin` to set pin mode](https://github.com/beagleboard/bb.org-overlays/tree/master/tools/beaglebone-universal-io) for [UART1 and UART2 pins](http://beagleboard.org/static/images/cape-headers-serial.png)
```
config-pin P9.21 uart  # UART2_TXD
config-pin P9.22 uart  # UART2_RXD
config-pin P9.24 uart  # UART1_TXD
config-pin P9.26 uart  # UART1_RXD
```
* [Install pyserial](https://learn.adafruit.com/setting-up-io-python-library-on-beaglebone-black/uart#using-uart-with-python)
```
sudo pip install pyserial
```
* [Test UART1](https://learn.adafruit.com/setting-up-io-python-library-on-beaglebone-black/uart#using-uart-with-python)
```
import Adafruit_BBIO.UART as UART
import serial

UART.setup("UART1")

ser = serial.Serial(port = "/dev/ttyO1", baudrate=9600)
ser.close()
ser.open()
if ser.isOpen():
	print "Serial is open!"
    ser.write("Hello World!")
ser.close()
```
* [Loopback test with UART1 and UART2](https://learn.adafruit.com/setting-up-io-python-library-on-beaglebone-black/uart#testing-and-using-the-uart)

## Running tests

Install py.test to run the tests. You'll also need the python compiler package for pytest:
```
sudo pip install pytest
```
Execute the following in the root of the project:
```
sudo pytest
```
NOTE: `sudo` should not be required when running [Debian 9.2 "Stretch" iot (2017-10-29)](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#microSD.2FStandalone:_.28stretch-iot.29_.28All_BeagleBone_Variants_.26_PocketBeagle.29) with [Linux kernel](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#Kernel_Options) [4.14.x](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#Mainline_.284.14.x_lts.29) as udev configures group ownership and permission for [GPIO](https://github.com/rcn-ee/repos/blob/master/bb-customizations/suite/stretch/debian/80-gpio-noroot.rules) and [PWM](https://github.com/rcn-ee/repos/blob/master/bb-customizations/suite/stretch/debian/81-pwm-noroot.rules)

## Credits

The BeagleBone IO Python library was originally forked from the excellent MIT Licensed [RPi.GPIO](https://code.google.com/p/raspberry-gpio-python) library written by Ben Croston.

## License

Written by Justin Cooper, Adafruit Industries. BeagleBone IO Python library is released under the MIT License.
