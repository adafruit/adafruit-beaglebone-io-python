**PLEASE NOTE:  This library may have breaking changes as development continues.  Please read the changelog anytime you update the library!**

**The PWM Duty Cycle range was reversed in 0.0.15 from 100(off)-0(on) to 0(off)-100(on).  Please update your code accordingly.**

**Adafruit's BeagleBone IO Python Library**

This is a set of Python tools to allow GPIO, PWM, and ADC access on the BeagleBone using the Linux 3.8 Kernel and above (latest releases).

It has been tested on the 5-20 and 6-6 Angstrom image on the BeagleBone Black.

**Note: BBIO has been renamed to Adafruit_BBIO.**

**Installation on Angstrom**

Easiest::

    /usr/bin/ntpdate -b -s -u pool.ntp.org
    opkg update && opkg install python-pip python-setuptools
    pip install Adafruit_BBIO
    
Manual::

    git clone git://github.com/adafruit/adafruit-beaglebone-io-python.git 
    #set the date and time 
    /usr/bin/ntpdate -b -s -u pool.ntp.org 
    #install dependency 
    opkg update && opkg install python-distutils 
    cd adafruit-beaglebone-io-python 
    python setup.py install

**Installation on Ubuntu/Debian**

Easiest::

    sudo ntpdate pool.ntp.org
    sudo apt-get update
    sudo apt-get install build-essential python-dev python-pip -y
    #easy_install -U distribute  //debian only
    sudo pip install Adafruit_BBIO
    
Manual::

    sudo ntpdate pool.ntp.org
    sudo apt-get update
    sudo apt-get install build-essential python-dev python-pip -y
    git clone git://github.com/adafruit/adafruit-beaglebone-io-python.git
    cd adafruit-beaglebone-io-python
    sudo python setup.py install
    cd ..
    sudo rm -rf adafruit-beaglebone-io-python
    
**Usage**

Using the library is very similar to the excellent RPi.GPIO library used on the Raspberry Pi. Below are some examples.

**GPIO Setup** 

Import the library, and setup as GPIO.OUT or GPIO.IN::

    import Adafruit_BBIO.GPIO as GPIO
    GPIO.setup("P8_14", GPIO.OUT)

You can also refer to the pin names::

    GPIO.setup("GPIO0_26", GPIO.OUT)

**GPIO Output** 

Setup the pin for output, and write GPIO.HIGH or GPIO.LOW. Or you can use 1 or 0.::

    import Adafruit_BBIO.GPIO as GPIO
    GPIO.setup("P8_14", GPIO.OUT) GPIO.output("P8_14", GPIO.HIGH)
    
**GPIO Input**

Inputs work similarly to outputs.::

    import Adafruit_BBIO.GPIO as GPIO
    GPIO.setup("P8_14", GPIO.IN)
    
Polling inputs::
    
    if GPIO.input("P8_14"):
      print("HIGH")
    else:
      print("LOW")

Waiting for an edge (GPIO.RISING, GPIO.FALLING, or GPIO.BOTH::

    GPIO.wait_for_edge(channel, GPIO.RISING)

Detecting events::

    GPIO.add_event_detect("P9_12", GPIO.FALLING) 
    #your amazing code here 
    #detect wherever: 
    if GPIO.event_detected("P9_12"):
      print "event detected!"

**PWM**::

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

**ADC**::

    import Adafruit_BBIO.ADC as ADC
    ADC.setup()

    #read returns values 0-1.0 
    value = ADC.read("P9_40")

    #read_raw returns non-normalized value 
    value = ADC.read_raw("P9_40")

**Running tests**

Install py.test to run the tests. You'll also need the python compiler package for py.test.::

    opkg update && opkg install python-compiler 
    #Either pip or easy_install 
    pip install -U pytest 
    easy_install -U pytest

Execute the following in the root of the project::

    py.test
    
**Credits**

The BeagleBone IO Python library was originally forked from the excellent MIT Licensed [RPi.GPIO](https://code.google.com/p/raspberry-gpio-python) library written by Ben Croston.

**License**

Written by Justin Cooper, Adafruit Industries. BeagleBone IO Python library is released under the MIT License.
