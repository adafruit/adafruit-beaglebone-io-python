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
