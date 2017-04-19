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

#include "adafruit/bbio/error.h"
#ifdef __cplusplus
using adafruit::bbio::BBIO_err;
#endif

#define MODE_UNKNOWN -1
#define BOARD        10
#define BCM          11

#define CTRL_DIR_MAX 50
#define OCP_DIR_MAX  50

#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))

#define FILENAME_BUFFER_SIZE 128
#define MAX_PATH 256

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

extern int gpio_mode;
extern int gpio_direction[120];

extern char ctrl_dir[CTRL_DIR_MAX];
extern char ocp_dir[OCP_DIR_MAX];

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

#define BBIO_LOG_OPTION LOG_CONS | LOG_PID | LOG_NDELAY
void initlog(int level, const char* ident, int option);

extern int setup_error;
extern int module_setup;

#endif
