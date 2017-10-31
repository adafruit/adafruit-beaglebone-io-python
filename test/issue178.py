# These are all the LEDs defined in source/event_gpio.h:
#   #define USR_LED_GPIO_MIN 53
#   #define USR_LED_GPIO_MAX 56
#   #define USR_LED_RED      66
#   #define USR_LED_GREEN    67
#   #define BAT25   27
#   #define BAT50   11
#   #define BAT75   61
#   #define BAT100  10000       // Placeholder until I find the real number
#   #define WIFI    10001       // Ditto
#
# which map to entries in pins_t table[] in source/common.c
#
#   66
#       BeagleBone (not Blue): { "TIMER4", "P8_7", 66, -1, -1},
#       BeagleBone Blue: { "RED_LED", "RED", 66, -1, -1}, // LEDs
#
#   67
#       BeagleBone (not Blue): { "TIMER7", "P8_8", 67, -1, -1},
#       BeagleBone Blue: { "GREEN_LED", "GREEN", 67, -1, -1},
#
#   27
#       BeagleBone (not Blue): { "GPIO0_27", "P8_17", 27, -1, -1},
#       BeagleBone Blue: { "BAT25", "BAT25", 27, -1, -1},
#       PocketBeagle: { "GPIO0_27", "P2_19", 27, -1, -1},
#
#   11
#       BeagleBone (not Blue): { "UART5_RTSN", "P8_32", 11, -1, -1},
#       BeagleBone Blue: { "BAT50", "BAT50", 11, -1, -1},
#
#   61
#       BeagleBone (not Blue): { "GPIO1_29", "P8_26", 61, -1, -1},
#       BeagleBone Blue: { "BAT75", "BAT75", 61, -1, -1},
#

import Adafruit_BBIO.GPIO as GPIO

test_pins = [
              "USR0", "USR1", "USR2", "USR3",
              "RED_LED", "GREEN_LED", "BAT25", "BAT50", "BAT75",
              "P8_7", "P8_8", "P8_17", "P8_32", "P8_26"
            ]

for pin in test_pins:
  print("========================")
  print("test GPIO.setup(): {0}".format(pin))
  GPIO.setup(pin, GPIO.OUT)
  print("test GPIO.output(): {0}".format(pin))
  GPIO.output(pin, GPIO.HIGH)
  value = GPIO.input(pin);
  print("test GPIO.input(): {0}={1}".format(pin, value));

GPIO.cleanup()
