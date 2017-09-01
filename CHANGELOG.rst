1.0.5
----
* @pdp7 (5):
  * Merge pull request #153 from MarkAYoder/master
  * Fix print syntax to avoid python3 errors
  * Merge pull request #160 from MarkAYoder/master
  * document how to read QEP1
  * Update rotary-encoder-eqep-test.md

* @MarkAYoder (20):
  * Have GP0_1 working
  * Removed --force to speed things up
  * Added GP0 1, 2 and 3
  * Flashes 4 LEDs
  * Works with button
  * Blinks red and gree LEDs
  * Blinks all 6 GPIOs
  * Added red and green LEDs
  * i2c works
  * PWD isn't working, yet
  * Added port setup
  * Switched to apt install
  * Added tmp101 to name
  * Added LED matrix example
  * Removed newline from print
  * Added fade
  * Adding GPIO defs for uart1
  * Testing UT1_0, not working yet
  * Switched GP0_0 to GP0_3, etc.
  * Added PAUSE and MODE buttons.

1.0.4
----
* @djsutton (1):
  * fix TypeError: function takes exactly 3 arguments (2 given) from wait_for_edge

* @pdp7 (29):
  * Instruct users to open GitHub issue instead email
  * add udev rules and script for non-root access to gpio
  * fix file descriptor leak in gpio_set_value()
  * document how to test read and write to all GPIO pins
  * reduce ADC reads in pytest from 10,000 to 1,000
  * close file descriptor to avoid leak
  * remove conditional logic for ctrl_dir and ocp_dir size
  * increase size of ctrl_dir and ocp_dir for future use
  * Document how to run config-pin at boot
  * Document how to test eQEP with Rotary Encoder
  * Add skeleton for Encoder module to read eQEP
  * Add code to Encoder.QEP from PyBBIO.RotaryEncoder
  * Adapt code from PyBBIO.RotaryEncoder
  * add test for rotary encoder
  * read from eqep position file
  * return position from getPosition()
  * document howo to enable all the eqep pins
  * Document how to test eqep pins with rotary encoder
  * run config-pin to set pin mux for qep
  * update QEP test
  * update QEP test for issue #122
  * Test if kernel booted wit u-boot overlays
  * check if kernel cmdline for uboot overlay
  * Add documentation about u-boot overlays
  * Return BBIO_OK when u-boot overlays ared enabled
  * remove debug printing
  * Skip check for device tree loaded if u-boot overlays enabled
  * Sleep after loading ADC overlay to allow driver load
  * Workaround test failure until TIMERn bug is fixed

* @ltjax (3):
  * Use lookup table to prevent duplicate pin export
  * Handle already exported pins
  * Fix build_path memory leak

* @Vadim-Stupakov (1):
  * Fixed issue #145 GPIO library doesn't free GPIO file descriptor. File descriptor leak.     Made a little bit refactoring

* @cocasema (8):
  * Declare PHONY targets in root Makefile
  * Extract BBIO_err into a separate header
  * Add syslog and debugging messages
  * Add libadafruit-bbio with C++ wrappers for PWM/GPIO
  * Add 2 versions of library with c++98 and c++11 abi
  * Install header files properly
  * Add default values to pwm::start() method.
  * Add PWM c++ tests

* @zsserg (2):
  * Added error checking for setting pin direction in gpio.setup() (Python)
  * Added debug output to set_pin_mode()

1.0.3
----
* Add -Wno-strict-aliasing to CFLAGS to ignore gcc warning
   * Resolves GitHub issue #133 by @archey

1.0.2
----
* Merge pull request #130 from adafruit/issue129-usr-leds [1439133]
   * Add support for alternate USR LED labels
* Merge pull request #131 from adafruit/fix-gcc-warnings [f0ee018]
   * Fix gcc warnings
* Merge pull request #132 from buckket/setup_unicode_fix [4c67dfc]
   * Make use of io.open() with explicit file encoding in setup.py


1.0.1
----
* Merge pull request #124 from jpbarraca/master [cf9771a]
   * Timeout support for wait_for_edge (replaces PR #62)
* Merge pull request #123 from bubbapizza/master [8b4f7f2]
   * Added a delay parameter for GPIO.setup() for udev permissions
* Merge pull request #121 from dengber/master [50e8883]
   * ADC.read() returns wrong value
* Merge pull request #64 from olegantonyan/master [d1e8dc1]
   * Wait until GPIO file appears on the /sys filesystem (issue #36)
* Merge pull request #106 from cocasema/master [12b79d7]
   * Treat warnings as errors
* Merge pull request #119 from JesseMcL/pr [e7e987a]
   * Add GPIO pullup configurations and fix PWM Segfault on kernel 4.1+
* Merge pull request #116 from kozga/master [1b04cdf]
   * Fix SPI: IOError: [Errno 22] Invalid argument in xfer and xfer2 funct…

1.0.0
----
* Merge pull request #108 from MatthewWest for PWM support in Linux kernel 4.1+
* Merge pull request #96 from  PeteLawler for ADC support in Linux kernel 4.1+
* Finally publish new version to PyPi 
* Bump major version number to signify long duration since last release

0.0.30
-----
* Merge Python 3 compatibility fixes from Github user westphahl.
* Moved old Angstrom build fix for missing py_compile from setup.py to separate file.

0.0.20
----
* Fix for SPI not loading spidevX.X correctly based on load order
* Initialize ctrl_dir in unload_device_tree #63
* Clean up unused/dead code

0.0.19
----
* Fix for SPI.xfer crashes python after 3 calls
* Added a retry to reading for the analog inputs to avoid a bug where reading back and forth between two analog inputs would cause the resource to be unavailable every 16 scans (zthorson)
* Updated the build_path to be more selective over what paths it chooses (zthorson)
* Update Debian installation instructions in README (justinledwards)
* Increase the size of the buffer used for storing device tree names (SaintGimp)

0.0.18
----
* UART - Include UART overlays, and compile upon installation
* UART - Rename UART overlays
* Adafruit_I2C - Remove readU16Rev and readS16Rev
* Adafruit_I2C - Updated readU16/readS16 for correct 16-bit reads

0.0.17
----
* Fix SPI memory leaks
* Clean up of PWM code (bit-hacker, jwcooper)
* Remove UART debug statements

0.0.16
----
* Add polarity as optional fourth parameter to PWM.start().  Valid values are 0 and 1.  Default is still 0.
* Fix for actually setting the polarity in start.
* Add new unit tests to check that the polarity is being set properly, and valid values passed in.

0.0.15
----
* Fix PWM duty cycle so 0 is off and 100 is on.  Set polarity to 0 by default.
* Give extra buffer space in export, and unexport functions for gpio that are more than 2 digits (Chris Desjardins)
* Add new test case for 3 digit gpio (Chris Desjardins)
* Fix for test_direction_readback. gpio_get_direction wasn't properly null terminating the direction string (Chris Desjardins)

0.0.14
----
* Fix GPIO.gpio_function to work with the IO name (zthorson)
* Fix IOErrors not getting raised when fopen fails while loading overlays into device tree (bradfordboyle, jwcooper)
* Add new UART tests

0.0.13
----
* Remove the gpio parameter from callbacks (cdesjardins)

0.0.12
----
* Bump version due to pypi issues

0.0.11
----
* New UART module to export UART overlays
* Alpha support for SPI
* Add small delay after loading any device tree overlays

0.0.10
____
* Fix direction for event detection code
* Fix for segmentation faults on add_event_detect

0.0.9
____
* Fix for ADC Segmentation Faults

0.0.8
____
* Temp remove overlay compilation.  Ubuntu failures.

0.0.7
____
* Refactor and clean up adc and pwm
* Fix tests for Adafruit_BBIO rename

0.0.6
____
* Include Adafruit_I2C.py as top-level module

0.0.5
----
* Rename from BBIO to Adafruit_BBIO to reduce library conflicts and confusion.

0.0.4
----
* Support for pip and easy_install

0.0.3
____
* ADC enabled

0.0.2
____
* PWM enabled

0.0.1
____
* Initial Commit
* GPIO mostly working
* Initial GPIO unit tests
* PWM in progress
