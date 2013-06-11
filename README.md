## BeagleBone IO Python Library

This is a set of Python tools to allow GPIO, PWM, and ADC access on the BeagleBone using the Linux 3.8 Kernel and above (latest releases).

It has been tested on the 5-20 angstrom image on the BeagleBone Black.

## Installation

    git clone
    opkg update && opkg install python-dev python3-dev
    python setup.py install

## Usage

Using the library is very similar to the excellent RPi.GPIO library used on the Raspberry Pi.  Below are some examples.

#### GPIO Setup
Import the library, and setup as GPIO.OUT or GPIO.IN

    import BBIO.GPIO as GPIO

    GPIO.setup("P8_14", GPIO.OUT)
    
You can also refer to the pin names:

    GPIO.setup("GPIO0_26", GPIO.OUT)

#### GPIO Output
Setup the pin for output, and write GPIO.HIGH or GPIO.LOW.  Or you can use 1 or 0.

    import BBIO.GPIO as GPIO

    GPIO.setup("P8_14", GPIO.OUT)
    GPIO.output("P8_14", GPIO.HIGH)

#### GPIO Input
Inputs work similarly to outputs.

    import BBIO.GPIO as GPIO

    GPIO.setup("P8_14", GPIO.IN)
    
Polling inputs:
    
    if GPIO.input("P8_14"):
        print("HIGH")
    else:
        print("LOW")
        
## Running tests

Install py.test to run the tests.  You'll also need the python compiler package for py.test.
  
    opkg update && opkg install python-compiler
    #Either pip or easy_install
    pip install -U pytest
    easy_install -U pytest

Execute the following in the root of the project:

    py.test