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

#define MODE_UNKNOWN -1
#define BOARD        10
#define BCM          11

#define ARRAY_SIZE(a)  (sizeof(a) / sizeof(a[0]))

#define FILENAME_BUFFER_SIZE 128

int gpio_mode;
int gpio_direction[120];

char ctrl_dir[35];
char ocp_dir[25];

int get_gpio_number(const char *key, unsigned int *gpio);
int get_pwm_key(const char *input, char *key);
int get_adc_ain(const char *key, unsigned int *ain);
int get_uart_device_tree_name(const char *name, char *dt);
int build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len);
int get_spi_bus_path_number(unsigned int spi);
int load_device_tree(const char *name);
int unload_device_tree(const char *name);
int setup_error;
int module_setup;
