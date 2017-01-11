/*
Copyright (c) 2013 Adafruit

Original RPi.GPIO Author Ben Croston
Modified for BBIO Author Justin Cooper

This file incorporates work covered by the following copyright and 
permission notice, all modified code adopts the original license:

Copyright (c) 2013 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#define MODE_UNKNOWN -1
#define BOARD        10
#define BCM          11

#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))

#define FILENAME_BUFFER_SIZE 128
#define MAX_PATH 256

typedef enum {
	BBIO_OK, // No error
	BBIO_ACCESS, // Error accessing a file
	BBIO_SYSFS, // Some error with Sysfs files
	BBIO_CAPE, // some error with capes
	BBIO_INVARG, // Invalid argument
	BBIO_MEM,
	BBIO_GEN // General error
} BBIO_err;

// Modeled after "pwm": submap in bone.js from bonescript
// https://github.com/jadonk/bonescript/blob/master/src/bone.js#L680
typedef struct pwm_t {
  const char *module;
  const int sysfs;
  const int index;
  const int muxmode;
  const char *path;
  const char *name;
  const char *chip;
  const char *addr;
  const char *key;  // Pin name eg P9_21
} pwm_t;

int gpio_mode;
int gpio_direction[120];

#ifdef BBBVERSION41
    char ctrl_dir[43];
    char ocp_dir[33];
#else
    char ctrl_dir[35];
    char ocp_dir[25];
#endif
 
BBIO_err get_gpio_number(const char *key, unsigned int *gpio);
BBIO_err get_pwm_key(const char *input, char *key);
BBIO_err get_adc_ain(const char *key, int *ain);
BBIO_err get_uart_device_tree_name(const char *name, char *dt);
BBIO_err build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len);
int get_spi_bus_path_number(unsigned int spi);
BBIO_err load_device_tree(const char *name);
BBIO_err unload_device_tree(const char *name);
int device_tree_loaded(const char *name);
BBIO_err get_pwm_by_key(const char *key, pwm_t **pwm);


int setup_error;
int module_setup;

#endif
