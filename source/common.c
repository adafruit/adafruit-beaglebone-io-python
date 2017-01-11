/*
Copyright (c) 2013 Adafruit

Original RPi.GPIO Author Ben Croston
Modified for BBIO Author Justin Cooper
Modified for 4.1+ kernels by Grizmio
Unified for 3.8 and 4.1+ kernels by Peter Lawler <relwalretep@gmail.com>

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

#include "Python.h"
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <glob.h>
#include "common.h"

#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0)
#  ifndef BBBVERSION41
#    define BBBVERSION41
#  endif
#endif

int setup_error = 0;
int module_setup = 0;

typedef struct pins_t { 
    const char *name; 
    const char *key; 
    int gpio;
    int pwm_mux_mode;
    int ain;
} pins_t;

//Table generated based on https://raw.github.com/jadonk/bonescript/master/node_modules/bonescript/bone.js
pins_t table[] = {
  { "USR0", "USR0", 53, -1, -1},
  { "USR1", "USR1", 54, -1, -1},
  { "USR2", "USR2", 55, -1, -1},
  { "USR3", "USR3", 56, -1, -1},
  { "DGND", "P8_1", 0, -1, -1},
  { "DGND", "P8_2", 0, -1, -1},
  { "GPIO1_6", "P8_3", 38, -1, -1},
  { "GPIO1_7", "P8_4", 39, -1, -1},
  { "GPIO1_2", "P8_5", 34, -1, -1},
  { "GPIO1_3", "P8_6", 35, -1, -1},
  { "TIMER4", "P8_7", 66, -1, -1},
  { "TIMER7", "P8_8", 67, -1, -1},
  { "TIMER5", "P8_9", 69, -1, -1},
  { "TIMER6", "P8_10", 68, -1, -1},
  { "GPIO1_13", "P8_11", 45, -1, -1},
  { "GPIO1_12", "P8_12", 44, -1, -1},
  { "EHRPWM2B", "P8_13", 23, 4, -1},
  { "GPIO0_26", "P8_14", 26, -1, -1},
  { "GPIO1_15", "P8_15", 47, -1, -1},
  { "GPIO1_14", "P8_16", 46, -1, -1},
  { "GPIO0_27", "P8_17", 27, -1, -1},
  { "GPIO2_1", "P8_18", 65, -1, -1},
  { "EHRPWM2A", "P8_19", 22, 4, -1},
  { "GPIO1_31", "P8_20", 63, -1, -1},
  { "GPIO1_30", "P8_21", 62, -1, -1},
  { "GPIO1_5", "P8_22", 37, -1, -1},
  { "GPIO1_4", "P8_23", 36, -1, -1},
  { "GPIO1_1", "P8_24", 33, -1, -1},
  { "GPIO1_0", "P8_25", 32, -1, -1},
  { "GPIO1_29", "P8_26", 61, -1, -1},
  { "GPIO2_22", "P8_27", 86, -1, -1},
  { "GPIO2_24", "P8_28", 88, -1, -1},
  { "GPIO2_23", "P8_29", 87, -1, -1},
  { "GPIO2_25", "P8_30", 89, -1, -1},
  { "UART5_CTSN", "P8_31", 10, -1, -1},
  { "UART5_RTSN", "P8_32", 11, -1, -1},
  { "UART4_RTSN", "P8_33", 9, -1, -1},
  { "UART3_RTSN", "P8_34", 81, 2, -1},
  { "UART4_CTSN", "P8_35", 8, -1, -1},
  { "UART3_CTSN", "P8_36", 80, 2, -1},
  { "UART5_TXD", "P8_37", 78, -1, -1},
  { "UART5_RXD", "P8_38", 79, -1, -1},
  { "GPIO2_12", "P8_39", 76, -1, -1},
  { "GPIO2_13", "P8_40", 77, -1, -1},
  { "GPIO2_10", "P8_41", 74, -1, -1},
  { "GPIO2_11", "P8_42", 75, -1, -1},
  { "GPIO2_8", "P8_43", 72, -1, -1},
  { "GPIO2_9", "P8_44", 73, -1, -1},
  { "GPIO2_6", "P8_45", 70, 3, -1},
  { "GPIO2_7", "P8_46", 71, 3, -1},
  { "DGND", "P9_1", 0, -1, -1},
  { "DGND", "P9_2", 0, -1, -1},
  { "VDD_3V3", "P9_3", 0, -1, -1},
  { "VDD_3V3", "P9_4", 0, -1, -1},
  { "VDD_5V", "P9_5", 0, -1, -1},
  { "VDD_5V", "P9_6", 0, -1, -1},
  { "SYS_5V", "P9_7", 0, -1, -1},
  { "SYS_5V", "P9_8", 0, -1, -1},
  { "PWR_BUT", "P9_9", 0, -1, -1},
  { "SYS_RESETn", "P9_10", 0, -1, -1},
  { "UART4_RXD", "P9_11", 30, -1, -1},
  { "GPIO1_28", "P9_12", 60, -1, -1},
  { "UART4_TXD", "P9_13", 31, -1, -1},
  { "EHRPWM1A", "P9_14", 50, 6, -1},
  { "GPIO1_16", "P9_15", 48, -1, -1},
  { "EHRPWM1B", "P9_16", 51, 6, -1},
  { "I2C1_SCL", "P9_17", 5, -1, -1},
  { "I2C1_SDA", "P9_18", 4, -1, -1},
  { "I2C2_SCL", "P9_19", 13, -1, -1},
  { "I2C2_SDA", "P9_20", 12, -1, -1},
  { "UART2_TXD", "P9_21", 3, 3, -1},
  { "UART2_RXD", "P9_22", 2, 3, -1},
  { "GPIO1_17", "P9_23", 49, -1, -1},
  { "UART1_TXD", "P9_24", 15, -1, -1},
  { "GPIO3_21", "P9_25", 117, -1, -1},
  { "UART1_RXD", "P9_26", 14, -1, -1},
  { "GPIO3_19", "P9_27", 115, -1, -1},
  { "SPI1_CS0", "P9_28", 113, 4, -1},
  { "SPI1_D0", "P9_29", 111, 1, -1},
  { "SPI1_D1", "P9_30", 112, -1, -1},
  { "SPI1_SCLK", "P9_31", 110, 1, -1},
  { "VDD_ADC", "P9_32", 0, -1, -1},
  { "AIN4", "P9_33", 0, -1, 4},
  { "GNDA_ADC", "P9_34", 0, -1, -1},
  { "AIN6", "P9_35", 0, -1, 6},
  { "AIN5", "P9_36", 0, -1, 5},
  { "AIN2", "P9_37", 0, -1, 2},
  { "AIN3", "P9_38", 0, -1, 3},
  { "AIN0", "P9_39", 0, -1, 0},
  { "AIN1", "P9_40", 0, -1, 1},
  { "CLKOUT2", "P9_41", 20, -1, -1},
  { "GPIO0_7", "P9_42", 7, 0, -1},
  { "DGND", "P9_43", 0, -1, -1},
  { "DGND", "P9_44", 0, -1, -1},
  { "DGND", "P9_45", 0, -1, -1},
  { "DGND", "P9_46", 0, -1, -1},
    { NULL, NULL, 0 }
};

typedef struct uart_t { 
    const char *name; 
    const char *path;
    const char *dt; 
    const char *rx;
    const char *tx;
} uart_t;

uart_t uart_table[] = {
  { "UART1", "/dev/ttyO1", "ADAFRUIT-UART1", "P9_26", "P9_24"},
  { "UART2", "/dev/ttyO2", "ADAFRUIT-UART2", "P9_22", "P9_21"},
  { "UART3", "/dev/ttyO3", "ADAFRUIT-UART3", "P9_42", ""},
  { "UART4", "/dev/ttyO4", "ADAFRUIT-UART4", "P9_11", "P9_13"},
  { "UART5", "/dev/ttyO5", "ADAFRUIT-UART5", "P8_38", "P8_37"},
  { NULL, NULL, 0 }
};

// Copied from https://github.com/jadonk/bonescript/blob/master/src/bone.js
// See am335x technical manual, p. 183, for more info:
// http://www.ti.com/lit/ug/spruh73n/spruh73n.pdf
pwm_t pwm_table[] = {
  { "ehrpwm2", 6, 1, 4, "ehrpwm.2:1", "EHRPWM2B", "48304000", "48304200", "P8_13"},
  { "ehrpwm2", 5, 0, 4, "ehrpwm.2:0", "EHRPWM2A", "48304000", "48304200", "P8_19"},
  { "ehrpwm1", 4, 1, 2, "ehrpwm.1:1", "EHRPWM1B", "48302000", "48302200", "P8_34"},
  { "ehrpwm1", 3, 0, 2, "ehrpwm.1:0", "EHRPWM1A", "48302000", "48302200", "P8_36"},
  { "ehrpwm2", 5, 0, 3, "ehrpwm.2:0", "EHRPWM2A", "48304000", "48304200", "P8_45"},
  { "ehrpwm2", 6, 1, 3, "ehrpwm.2:1", "EHRPWM2B", "48304000", "48304200", "P8_46"},
  { "ehrpwm1", 3, 0, 6, "ehrpwm.1:0", "EHRPWM1A", "48302000", "48302200", "P9_14"},
  { "ehrpwm1", 4, 1, 6, "ehrpwm.1:1", "EHRPWM1B", "48302000", "48302200", "P9_16"},
  { "ehrpwm0", 1, 1, 3, "ehrpwm.0:1", "EHRPWM0B", "48300000", "48300200", "P9_21"},
  { "ehrpwm0", 0, 0, 3, "ehrpwm.0:0", "EHRPWM0A", "48300000", "48300200", "P9_22"},
  {   "ecap2", 7, 0, 4, "ecap.2",     "ECAPPWM2", "48304000", "48304100", "P9_28"},
  { "ehrpwm0", 1, 1, 1, "ehrpwm.0:1", "EHRPWM0B", "48300000", "48300200", "P9_29"},
  { "ehrpwm0", 0, 0, 1, "ehrpwm.0:0", "EHRPWM0A", "48300000", "48300200", "P9_31"},
  {   "ecap0", 2, 0, 0, "ecap.0",     "ECAPPWM0", "48300000", "48300100", "P9_42"},
  { NULL, 0, 0, 0, NULL, NULL, NULL, NULL, NULL }
};

int lookup_gpio_by_key(const char *key)
{
  pins_t *p;
  for (p = table; p->key != NULL; ++p) {
      if (strcmp(p->key, key) == 0) {
          return p->gpio;
      }
  }
  return 0;
}

int lookup_gpio_by_name(const char *name)
{
  pins_t *p;
  for (p = table; p->name != NULL; ++p) {
      if (strcmp(p->name, name) == 0) {
          return p->gpio;
      }
  }
  return 0;
}

int lookup_ain_by_key(const char *key)
{
  pins_t *p;
  for (p = table; p->key != NULL; ++p) {
      if (strcmp(p->key, key) == 0) {
        if (p->ain == -1) {
          return -1;
        } else {
          return p->ain;
        }
      }
  }
  return -1;
}

int lookup_ain_by_name(const char *name)
{
  pins_t *p;
  for (p = table; p->name != NULL; ++p) {
      if (strcmp(p->name, name) == 0) {
        if (p->ain == -1) {
          return -1;
        } else {
          return p->ain;
        }
      }
  }
  return -1;
}

BBIO_err lookup_uart_by_name(const char *input_name, char *dt)
{
    uart_t *p;
    for (p = uart_table; p->name != NULL; ++p) {
        if (strcmp(p->name, input_name) == 0) {
            strncpy(dt, p->dt, FILENAME_BUFFER_SIZE);
            dt[FILENAME_BUFFER_SIZE - 1] = '\0';
            return BBIO_OK;
        }
    }
    return BBIO_INVARG;
}

BBIO_err copy_pwm_key_by_key(const char *input_key, char *key)
{
    pins_t *p;
    for (p = table; p->key != NULL; ++p) {
        if (strcmp(p->key, input_key) == 0) {
            //validate it's a valid pwm pin
            if (p->pwm_mux_mode == -1)
                return BBIO_INVARG;

            strncpy(key, p->key, 7);
            key[7] = '\0';
            return BBIO_OK;                
        }
    }
    return BBIO_INVARG;
}

BBIO_err get_pwm_key_by_name(const char *name, char *key)
{
    pins_t *p;
    for (p = table; p->name != NULL; ++p) {
        if (strcmp(p->name, name) == 0) {
            //validate it's a valid pwm pin
            if (p->pwm_mux_mode == -1)
                return BBIO_INVARG;

            strncpy(key, p->key, 7);
            key[7] = '\0';
            return BBIO_OK;
        }
    }
    return BBIO_INVARG;
}

BBIO_err get_gpio_number(const char *key, unsigned int *gpio)
{
    *gpio = lookup_gpio_by_key(key);
    
    if (!*gpio) {
        *gpio = lookup_gpio_by_name(key);
    }

    if (!*gpio) {
        return BBIO_INVARG;
    }

    return BBIO_OK;
}

BBIO_err get_pwm_by_key(const char *key, pwm_t **pwm)
{
    pwm_t *p;
    // Loop through the table
    for (p = pwm_table; p->key != NULL; ++p) {
        if (strcmp(p->key, key) == 0) {
            // Return the pwm_t struct
            *pwm = p;
            return BBIO_OK;
        }
    }
    return BBIO_INVARG;
}

BBIO_err get_pwm_key(const char *input, char *key)
{
    BBIO_err err = copy_pwm_key_by_key(input, key);
    if (err == BBIO_OK) {
        return err;
    }

    err = get_pwm_key_by_name(input, key);
    if (err == BBIO_OK) {
        return err;
    }

    return BBIO_INVARG;
}

// TODO: fix warning
// source/common.c:344:14: error: comparison between signed and unsigned integer expressions [-Werror=sign-compare]
BBIO_err get_adc_ain(const char *key, int *ain)
{
    *ain = lookup_ain_by_key(key);
    
    if (*ain == -1) {
        *ain = lookup_ain_by_name(key);

        if (*ain == -1) {
          return BBIO_INVARG;
        }
    }

    return BBIO_OK;
}

BBIO_err get_uart_device_tree_name(const char *name, char *dt)
{
    BBIO_err err;
    err = lookup_uart_by_name(name, dt);
    if (err != BBIO_OK) {
        return err;
    }

    return BBIO_OK;
}

BBIO_err build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len)
{
    glob_t results;
    size_t len = strlen(partial_path) + strlen(prefix) + 5;
    char *pattern = malloc(len);
    snprintf(pattern, len, "%s/%s*", partial_path, prefix);

/*  int glob(const char *pattern, int flags,
                int (*errfunc) (const char *epath, int eerrno),
                glob_t *pglob); */
    int err = glob(pattern, 0, NULL, &results);
    if (err != BBIO_OK) {
        globfree(&results);
        if (err == GLOB_NOSPACE)
            return BBIO_MEM;
        else
            return BBIO_GEN;
    }

    // We will return the first match
    strncpy(full_path, results.gl_pathv[0], full_path_len);

    // Free memory
    globfree(&results);

    return BBIO_OK;
}

int get_spi_bus_path_number(unsigned int spi)
{
  char path[MAX_PATH];

#ifdef BBBVERSION41
  strncpy(ocp_dir, "/sys/devices/platform/ocp", sizeof(ocp_dir));
#else
  build_path("/sys/devices", "ocp", ocp_dir, sizeof(ocp_dir));
#endif

  if (spi == 0) {
      snprintf(path, sizeof(path), "%s/48030000.spi/spi_master/spi1", ocp_dir);
  } else {
      snprintf(path, sizeof(path), "%s/481a0000.spi/spi_master/spi1", ocp_dir);
  }
  
  DIR* dir = opendir(path);
  if (dir) {
      closedir(dir);
      //device is using /dev/spidev1.x
      return 1;
  } else if (ENOENT == errno) {
      //device is using /dev/spidev2.x
      return 2;
  } else {
      return -1;
  }
}


BBIO_err load_device_tree(const char *name)
{
    FILE *file = NULL;
#ifdef BBBVERSION41
    char slots[41];
    snprintf(ctrl_dir, sizeof(ctrl_dir), "/sys/devices/platform/bone_capemgr");
#else
     char slots[40];
     build_path("/sys/devices", "bone_capemgr", ctrl_dir, sizeof(ctrl_dir));
#endif

    char line[256];

    snprintf(slots, sizeof(slots), "%s/slots", ctrl_dir);

    file = fopen(slots, "r+");
    if (!file) {
        PyErr_SetFromErrnoWithFilename(PyExc_IOError, slots);
        return BBIO_CAPE;
    }

    while (fgets(line, sizeof(line), file)) {
        //the device is already loaded, return 1
        if (strstr(line, name)) {
            fclose(file);
            return BBIO_OK;
        }
    }

    //if the device isn't already loaded, load it, and return
    fprintf(file, "%s", name);
    fclose(file);

    //0.2 second delay
    nanosleep((struct timespec[]){{0, 200000000}}, NULL);

    return BBIO_OK;
}

// Find whether a device tree is loaded.
// Returns 1 if so, 0 if not, and <0 if error
int device_tree_loaded(const char *name)
{
    FILE *file = NULL;
#ifdef BBBVERSION41
    char slots[41];
    snprintf(ctrl_dir, sizeof(ctrl_dir), "/sys/devices/platform/bone_capemgr");
#else
    char slots[40];
    build_path("/sys/devices", "bone_capemgr", ctrl_dir, sizeof(ctrl_dir));
#endif
    char line[256];

    snprintf(slots, sizeof(slots), "%s/slots", ctrl_dir);


    file = fopen(slots, "r+");
    if (!file) {
        PyErr_SetFromErrnoWithFilename(PyExc_IOError, slots);
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        //the device is loaded, close file and return true
        if (strstr(line, name)) {
            fclose(file);
            return 1;
        }
    }

    //not loaded, close file
    fclose(file);

    return 0;
}

BBIO_err unload_device_tree(const char *name)
{
    FILE *file = NULL;
#ifdef BBBVERSION41
    char slots[41];
    snprintf(ctrl_dir, sizeof(ctrl_dir), "/sys/devices/platform/bone_capemgr");
#else
    char slots[40];
    build_path("/sys/devices", "bone_capemgr", ctrl_dir, sizeof(ctrl_dir));
#endif
    char line[256];
    char *slot_line;

    snprintf(slots, sizeof(slots), "%s/slots", ctrl_dir);
    file = fopen(slots, "r+");
    if (!file) {
        PyErr_SetFromErrnoWithFilename(PyExc_IOError, slots);
        return BBIO_SYSFS;
    }

    while (fgets(line, sizeof(line), file)) {
        //the device is loaded, let's unload it
        if (strstr(line, name)) {
            slot_line = strtok(line, ":");
            //remove leading spaces
            while(*slot_line == ' ')
                slot_line++;

            fprintf(file, "-%s", slot_line);
            fclose(file);
            return BBIO_OK;
        }
    }

    //not loaded, close file
    fclose(file);

    return BBIO_OK;
}
