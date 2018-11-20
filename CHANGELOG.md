1.1.1
---
Attempt upload to PyPI again to avoid
error reported in issue #293

1.1.0
---
Aaron Marburg (1):
 * Added usleep after successfully enabling PWM via udev.

Drew Fustini (16):
 * Merge pull request #233 from zsserg/fixed_segfault_in_event_detection
 * Merge pull request #257 from zsserg/develop
 * Merge pull request #251 from amarburg/master
 * Merge pull request #271 from fcooper/documentation-updates
 * Update ADC.rst
 * Update Encoder.rst
 * Update ADC.rst
 * Add UART entries for the PocketBeagle (issue #242)
 * update install and test shell scripts
 * update UART section in README
 * Merge pull request #282 from erikwelsh/master
 * do not load overlays for the beaglebone blue #283
 * Merge pull request #284 from sam-bristow/py3-docs
 * Merge pull request #285 from sam-bristow/bugfix/uart-error-reporting
 * fix pwm on pocketbeagle and beaglebone blue #286
 * remove debug logging

Erik Welsh (1):
 * Fixed GPIO export problem; Leaves GPIO in bad state on latest BeagleBone image on PocketBeagle

Franklin S Cooper Jr (3):
 * docs/SPI.rst: Fix bus numbering in examples
 * docs/GPIO.rst: Add information on blinking led
 * docs/GPIO.rst Make documentation a bit newbie friendly

Sam Bristow (3):
 * Use print() function in all code and docs
 * Use new python-serial API
 * Return error-code for failing interface

zserg (5):
 * Fixed  SEGFAULT when calling remove_event_detect() inside python callback function.
 * Fixed  SEGFAULT when calling remove_event_detect() inside python callback function.
 * Fixed SEGFAULT in event_gpio,c run_callbacks() * Added more elaborate epoll() error logging
 * Minor style fixes

1.0.10
----
**features**
* automatically set pin modes for UART (PR #158)
* Encoder: README.md: added note about eqep group change (PR #214)
* deprecate out of date Adafruit_I2C.py (PR #215)
* Add Encoder module info to main README.md (PR #217)
* Add automatic API documentation generation (PR #219)
* Separate API docs into modules (PR #221)

**shortlog**
* David Planella (46):
  * Encoder: README.md: added note about eqep group change
  * Add Encoder module info to main README.md
  * Added docstrings using Google syntax and Sphinx support to generate the API documentation for the Encoder and PWM modules for now.
  * Made kernel version check to happen only if running on a beaglebone. The readthedocs builders that import the Encoder module have an old 3.3 kernel and the autodoc build fails
  * Use the default readthedocs theme
  * Use readthedocs theme if building docs there, remove redundand search link
  * Readthedocs theme tweaks
  * Removed redundant TOC, added global description
  * Added UART documentation
  * Added documentation badge
  * Added ADC API docs, fixed UART module definition
  * API docs: added SPI module
  * Added SPI module attribute docs
  * Added Python badges to README file
  * Added SPI pins table and first shot at GPIO module. Functions still need to be documented
  * Merge branch 'readthedocs' of https://github.com/dplanella/adafruit-beaglebone-io-python into readthedocs
  * Documented the API docs build process
  * Added docstrings using Google syntax and Sphinx support to generate the API documentation for the Encoder and PWM modules for now.
  * Made kernel version check to happen only if running on a beaglebone. The readthedocs builders that import the Encoder module have an old 3.3 kernel and the autodoc build fails
  * Use the default readthedocs theme
  * Use readthedocs theme if building docs there, remove redundand search link
  * Readthedocs theme tweaks
  * Removed redundant TOC, added global description
  * Added UART documentation
  * Added documentation badge
  * Added ADC API docs, fixed UART module definition
  * API docs: added SPI module
  * Added SPI module attribute docs
  * Added Python badges to README file
  * Added SPI pins table and first shot at GPIO module. Functions still need to be documented
  * Documented the API docs build process
  * Merge branch 'readthedocs' of https://github.com/dplanella/adafruit-beaglebone-io-python into readthedocs
  * Update README.md
  * Added some more API doc content
  * Sync from upstream master
  * Minor documentation and configuration improvements
  * Finished documenting GPIO
  * rST fixes
  * Update README.md
  * Minor API doc improvements
  * Merge branch 'readthedocs' of https://github.com/dplanella/adafruit-beaglebone-io-python into readthedocs
  * Generate the API documentation from a master index and a separate file for each module
  * Sync from upstream master
  * Improvements to the API docs output config
  * Update docs generation description to reflect new separate modules
  * Updated ADC API docs

* Drew Fustini (10):
  * use set_pin_mode() to set uart pinmux (#158)
  * Add SPI instructions to README (#158)
  * Update README.md
  * Fix spidev path mismatch (#216)
  * Merge pull request #217 from dplanella/patch-2
  * Merge pull request #214 from dplanella/patch-1
  * Deprecate Adafruit_BBIO.I2C in favor of Adafruit_GPIO.I2C (#215)
  * Merge pull request #219 from dplanella/readthedocs
  * relocate doc dir to avoid confusion (#218)
  * Merge pull request #221 from dplanella/readthedocs


1.0.9
----
**Features:**
* Issue #194: Encoder position cannot be set
* PR #205: Encoder: add support for reading/writing sysfs attributes

**Fixes:**
* Issue #198: use https for DEFAULT_URL in distribute_setup.py
* Issue #197: Fix leak of pwm enable file descriptor
* Issue #189: Fix seg fault of PWM in Python 3.6
* Issue #180: Clarify there is no 0 prefix for pin lables
* PR #201: Encoder: do kernel check, PEP8 cleanup
* PR #202: Encoder: corrected kernel check logic
* PR #207: Encoder: improved usage documentation
* PR #210: Encoder: fix sysfs import, make code Python 3 compatible
* PR #212: Encoder: fix Python 3 compatibility
* PR #213: Encoder: fix frequency calculation from period

**shortlog:**
* David Planella (18):
  * Encoder: initialize only the given channel
  * Sync from master
  * Encoder: do kernel check, PEP8 cleanup
  * Encoder: added sysfs module
  * Encoder: use sysfs to write QEP attributes
  * Encoder: corrected kernel check logic
  * Merge pull request #2 from adafruit/master
  * Encoder: convert get/set methods to properties, update apidoc strings
  * Encoder: updated README
  * Encoder: add README apt install clarification
  * Encoder: copyright assignment note, updated comments
  * Encoder: added usage notes
  * Encoder: improved usage documentation
  * Encoder: minor fix to usage example
  * Encoder: added a note about permissions
  * Encoder: switched sysfs to be a relative import compatible with Python 2 and 3
  * Encoder: use items() instead of iteritems() to be Python 3 compatible
  * Encoder: fix frequency getter

* Drew Fustini (18):
  * use https for DEFAULT_URL in distribute_setup.py (#198)
  * fix except syntax for Python 3
  * use dict.items() instead of dict.iteritems() for Python 3
  * fix error in set_brightness()
  * close enable_fd when stopping PWM output (#197)
  * Merge pull request #199 from dplanella/patch-1
  * Fix leak of pwm enable file descriptor (#197)
  * Merge pull request #201 from dplanella/encoder-cleanup
  * remove test_rotary.py as not valid for pytest
  * Fix seg fault of PWM in Python 3.6 (#189)
  * Merge pull request #202 from dplanella/patch-2
  * Clarify there is no 0 prefix for pin lables (#180)
  * Merge pull request #205 from dplanella/encoder-sysfs
  * assign copyright for new file to Adafruit Industries
  * Add bash scripts to help install and test
  * Merge pull request #212 from dplanella/patch-4
  * Merge pull request #207 from dplanella/patch-3
  * Merge pull request #213 from dplanella/fix-encoder-frequency

1.0.8
----
**Fixes:**
* Issue #196: cache board type to avoid poor performance
* Issue #192: fix PocketBeagle PWM pin typo
* Issue #191: turn off RotaryEncoder's debug output by default
* Issue #188: GPIO is extremely slow (20ms to toggle)
* Issue #186: problems with UART

**shortlog:**
* David Planella (12):
  * Copy Encoder module comments to README.md
  * Formatted Encoder README in markdown
  * Fixed Encoder README formatting
  * Removed QEP instructions from Encoder module
  * Fixes to Encoder README
  * Updated Encoder README
  * Encoder README: added info on dedicated overlays
  * Encoder README: updated info on pre-requisites
  * Encoder README update
  * Encoder README update
  * Add logging support, turn off unconditional debug output
  * Encoder: remove unused logging code

* Drew Fustini (3):
  * Merge pull request #195 from dplanella/master
  * Fix PocketBeagle PWM pin typo (#192)
  * cache board type to avoid poor performance (#196)

1.0.7
----
**Fixes:**
* Issue #188:  GPIO is extremely slow (20ms to toggle)

**shortlog:**
* Drew Fustini (4):
  * Update README.md
  * add config-pin example to README
  * Filter DEBUG syslog to avoid poor performance #188
  * Change log level from INFO to DEBUG #188

1.0.6
----
* Currently recommended image: [Debian 9.2 "Stretch" iot (2017-10-29)](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#microSD.2FStandalone:_.28stretch-iot.29_.28All_BeagleBone_Variants_.26_PocketBeagle.29)
  * Install [Linux kernel](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#Kernel_Options) [4.14.x](https://elinux.org/Beagleboard:BeagleBoneBlack_Debian#Mainline_.284.14.x_lts.29) to enable [non-root control of GPIO](https://github.com/rcn-ee/repos/blob/master/bb-customizations/suite/stretch/debian/80-gpio-noroot.rules) and [PWM](https://github.com/rcn-ee/repos/blob/master/bb-customizations/suite/stretch/debian/81-pwm-noroot.rules) [_(commit)_](https://github.com/adafruit/adafruit-beaglebone-io-python/commit/b65cbf8e41b444bad7c4ef6cfd4f88a30210fd78)

**Features:**
* Add support for Linux 4.14 kernel including new "udev" style for PWM entries in /sys
* Fix GPIO regression due to BeagleBone Blue LED support (issue #178)
* Add support for the PocketBeagle (issue #172)

**shortlog:**
* Drew Fustini (39):
  * Add -Wno-unit_address_vs_reg to avoid dtc warning
  * check if board is BeagleBone Blue or PocketBeagle
  * check if BeagleBone Blue before accessing non-standard LEDs
  * Add test for GPIO regression #178
  * change syslog mask level to DEBUG
  * add "Adafruit_BBIO" to syslog()
  * update test for issue #178
  * remove polarity "hack" for PWM #170
  * move pwm_set_polarity() after period is set
  * add test for issue #170
  * only check kernel overlays if u-boot overlays are not being used
  * Attempt to use udev ecap path for pwm path
  * add test script for all BeagleBone PWM outputs
  * update PWM test for 4.14 kernel udev paths
  * minor change to pwm debug logging
  * sleep to avoid udev race condition #185
    
* Mark A. Yoder (1):
  * Added BAT25, BAT50, BAT75, BAT100 and WIFI LEDs

* Peter Lawler (1):
  * Missing CR/LF
    
* Robert Nelson (10):
  * source/common.c: add initial PocketBeagle values
  * source/common.c: PocketBeagle, no slots file, everything built-in
  * source/common.c: PocketBeagle, no slots file disable here too
  * source/c_pwm.c: HACK: U-Boot pre-setup everything, dont search for specific overlay
  * source/c_pwm.c: HACK: PocketBeagle: v4.14.x
  * source/c_pwm.c: debug pwm_path/pwm_path_udev
  * source/c_pwm.c: pwm: add support for pwmchipX/pwm-X:Y syntax
  * source/c_pwm.c: disable pwm_set_polarity (broken in v4.9.x/v4.14.x)
  * source/common.c: Blue Fix GP0_3 id
  * source/common.c: PocketBeagle Fix P2.24

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
