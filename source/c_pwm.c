/*
Copyright (c) 2013 Adafruit
Author: Justin Cooper

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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>

#include "c_pwm.h"
#include "common.h"

#ifdef BBBVERSION41
#include "c_pinmux.h"
#endif

#define KEYLEN 7
#define PIN_MODE_LEN 5

int pwm_initialized = 0;

// pwm exports
struct pwm_exp
{
    char key[KEYLEN+1]; /* leave room for terminating NUL byte */
    int period_fd;
    int duty_fd;
    int polarity_fd;
#ifdef BBBVERSION41
    int enable_fd;
#endif
    float duty;
    unsigned long duty_ns;
    unsigned long period_ns;
    struct pwm_exp *next;
};
struct pwm_exp *exported_pwms = NULL;

struct pwm_exp *lookup_exported_pwm(const char *key)
{
    struct pwm_exp *pwm = exported_pwms;

    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0) {
            return pwm;
        }
        pwm = pwm->next;
    }

    syslog(LOG_DEBUG, "Adafruit_BBIO: lookup_exported_pwm: couldn't find '%s'", key);
    return NULL; /* standard for pointers */
}

// Export PWM to the list
void export_pwm(struct pwm_exp *new_pwm)
{
    struct pwm_exp *pwm;

    if (exported_pwms == NULL)
    {
        // create new list
        exported_pwms = new_pwm;
    } else {
        // add to end of existing list
        pwm = exported_pwms;
        while (pwm->next != NULL)
            pwm = pwm->next;
        pwm->next = new_pwm;
    }
}

BBIO_err initialize_pwm(void)
{
#ifdef BBBVERSION41  // don't load overlay in 4.1+
    if (!pwm_initialized) {
        strncpy(ocp_dir, "/sys/devices/platform/ocp", sizeof(ocp_dir));
#else
    BBIO_err err;
    if  (!pwm_initialized && load_device_tree("am33xx_pwm")) {
        err = build_path("/sys/devices", "ocp", ocp_dir, sizeof(ocp_dir));
        if (err != BBIO_OK)
        {
            return BBIO_SYSFS;
        }
#endif
        pwm_initialized = 1;
        syslog(LOG_DEBUG, "Adafruit_BBIO: initialize_pwm: OK");
        return BBIO_OK;
    }

    syslog(LOG_DEBUG, "Adafruit_BBIO: initialize_pwm: OK");
    return BBIO_OK;
}

BBIO_err pwm_set_frequency(const char *key, float freq) {
    int len;
    char buffer[20];
    unsigned long period_ns;
    struct pwm_exp *pwm;

    if (freq <= 0.0) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s freq %f <= 0.0", key, freq);
        return BBIO_INVARG;
    }

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't find key", key);
        return BBIO_GEN;
    }

    period_ns = (unsigned long)(1e9 / freq);

    // If we're going to a shorter period, update the
    // duty cycle first, in order to avoid ever setting
    // the period < duty cycle (which would throw error)
    if (period_ns < pwm->period_ns) {
        pwm->period_ns = period_ns;

        // Update duty ns
        pwm->duty_ns = (unsigned long)(period_ns * (pwm->duty / 100.0));
        len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty_ns);
        lseek(pwm->duty_fd, 0, SEEK_SET); // Seek to beginning of file
        if (write(pwm->duty_fd, buffer, len) < 0) {
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't write duty: %i-%s",
                   key, errno, strerror(errno));
            return BBIO_SYSFS;
        }

        // Update period ns
        len = snprintf(buffer, sizeof(buffer), "%lu", period_ns);
        lseek(pwm->period_fd, 0, SEEK_SET); // Seek to beginning of file
        if (write(pwm->period_fd, buffer, len) < 0) {
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't write period: %i-%s",
                   key, errno, strerror(errno));
            return BBIO_SYSFS;
        }

    } else if (period_ns > pwm->period_ns) {
        pwm->period_ns = period_ns;
        // Ordinarily update the period first,
        // to avoid the opposite bug - kernel won't
        // let us set duty greater than period

        // Update period ns
        len = snprintf(buffer, sizeof(buffer), "%lu", period_ns);
        lseek(pwm->period_fd, 0, SEEK_SET); // Seek to beginning of file
        if (write(pwm->period_fd, buffer, len) < 0) {
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't write period: %i-%s",
                   key, errno, strerror(errno));
            return BBIO_SYSFS;
        }

        // Update duty ns
        pwm->duty_ns = (unsigned long)(period_ns * (pwm->duty / 100.0));
        len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty_ns);
        lseek(pwm->duty_fd, 0, SEEK_SET); // Seek to beginning of file
        if (write(pwm->duty_fd, buffer, len) < 0) {
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_frequency: %s couldn't write duty: %i-%s",
                   key, errno, strerror(errno));
            return BBIO_SYSFS;
        }
    } // else do nothing

    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_set_frequency: %s %f OK", key, freq);
    return BBIO_OK;
}

// Only works before chip is enabled
BBIO_err pwm_set_polarity(const char *key, int polarity) {
    int len;
    char buffer[9]; /* allow room for trailing NUL byte */
    struct pwm_exp *pwm;
#ifdef BBBVERSION41
    int enabled; /* Maintain original state */
#endif

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't find key", key);
        return BBIO_GEN;
    }

    // polarity can't be changed with enabled.
#ifdef BBBVERSION41
    // Read the current enabled status
    len = 1;
    memset(buffer, 0, 9);  // Initialize buffer
    lseek(pwm->enable_fd, 0, SEEK_SET);
    if (read(pwm->enable_fd, buffer, len) < 0) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't read enable: %i-%s",
               key, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    // If the PWM is enabled, disable it
    // Can't set the polarity with device enabled
    // It will be reenabled after the parameters are set
    if (buffer[0] == '1') {
        enabled = 1;
        lseek(pwm->enable_fd, 0, SEEK_SET);
        len = snprintf(buffer, sizeof(buffer), "0");
        if (write(pwm->enable_fd, buffer, len) < 0) {
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't write enable: %i-%s",
                   key, errno, strerror(errno));
            return BBIO_SYSFS;
        }
    }

    // Treating 0 as normal, 1 as inversed
    // See documentation of sysfs interface at
    // https://www.kernel.org/doc/Documentation/pwm.txt
    if (polarity == 0) {
        len = snprintf(buffer, sizeof(buffer), "normal");
    } else if (polarity == 1) {
        len = snprintf(buffer, sizeof(buffer), "inversed");
    } else {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s invalid argument value: %i", key, polarity);
        return BBIO_INVARG;
    }
#else
    len = snprintf(buffer, sizeof(buffer), "%d", polarity);
#endif

    lseek(pwm->polarity_fd, 0, SEEK_SET); // Seek to beginning of file
    if (write(pwm->polarity_fd, buffer, len) < 0) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't write polarity: %i-%s",
               key, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    /* If we were enabled before, restore state */
#ifdef BBBVERSION41
    if (enabled) {
        lseek(pwm->enable_fd, 0, SEEK_SET);
        len = snprintf(buffer, sizeof(buffer), "1");
        if (write(pwm->enable_fd, buffer, len) < 0) {
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_polarity: %s couldn't write enable: %i-%s",
                   key, errno, strerror(errno));
            return BBIO_SYSFS;
        }
    }
#endif

    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_set_polarity: %s %i OK", key, polarity);
    return BBIO_OK;
}

BBIO_err pwm_set_duty_cycle(const char *key, float duty) {
    int len;
    char buffer[20];
    struct pwm_exp *pwm;

    if (duty < 0.0 || duty > 100.0)
        return BBIO_INVARG;

    pwm = lookup_exported_pwm(key);

    if (pwm == NULL) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_duty_cycle: %s couldn't find key", key);
        return BBIO_GEN;
    }

    pwm->duty = duty;
    pwm->duty_ns = (unsigned long)(pwm->period_ns * (duty / 100.0));

    len = snprintf(buffer, sizeof(buffer), "%lu", pwm->duty_ns);
    lseek(pwm->duty_fd, 0, SEEK_SET); // Seek to beginning of file
    if (write(pwm->duty_fd, buffer, len) < 0) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_set_duty_cycle: %s couldn't write duty: %i-%s",
               key, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_set_duty_cycle: %s %f OK", key, duty);
    return BBIO_OK;
}

BBIO_err pwm_setup(const char *key, __attribute__ ((unused)) float duty, __attribute__ ((unused)) float freq, __attribute__ ((unused)) int polarity)
{
    BBIO_err err;
    struct pwm_exp *new_pwm;

#ifdef BBBVERSION41
    char pwm_dev_path[45]; // "/sys/devices/platform/ocp/48300000.epwmss"
    char pwm_addr_path[60]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm"
    char pwm_chip_path[75]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0"
    char pwm_export_path[80]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/export"
    char pwm_path[85]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/pwm1"
    char pwm_path_udev[85]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/pwm-0:1"
    char ecap_path_udev[85];// "/sys/devices/platform/ocp/48300000.epwmss/48300100.ecap/pwm/pwmchip0/pwm-0:0/"
    char duty_path[95]; // "/sys/devices/platform/ocp/48300000.epwmss/48300200.ehrpwm/pwm/pwmchip0/pwm1/duty_cycle"
    char period_path[95];
    char polarity_path[95];
    char enable_path[90];
    char pin_mode[PIN_MODE_LEN]; // "pwm" or "pwm2"

    int e;
    int period_fd, duty_fd, polarity_fd, enable_fd;
    struct stat s;
    FILE *f = NULL;
    pwm_t *p;

    if (!pwm_initialized) {
        err = initialize_pwm();
        if (err != BBIO_OK) {
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't initialize pwm: %i", key, err);
            return err;
        }
    }

    // Make sure that one of the universal capes is loaded
    if( !uboot_overlay_enabled() // only check kernel overlays if u-boot overlays are not being used
        &&
        !beaglebone_blue() // beaglebone blue has complete dtb file and does not need overlays 
        &&
        !pocketbeagle() // pocketbeagle has complete dtb file and does not need overlays
        &&
        !( device_tree_loaded("cape-univ-audio") // from cdsteinkuehler/beaglebone-universal-io
        || device_tree_loaded("cape-univ-emmc")  // ""
        || device_tree_loaded("cape-univ-hdmi")  // ""
        || device_tree_loaded("cape-universal")  // ""
        || device_tree_loaded("cape-universala") // ""
        || device_tree_loaded("cape-universaln") // ""
        || device_tree_loaded("univ-all")        // from latest BeagleBone Debian 8 images
        || device_tree_loaded("univ-bbgw")       // ""
        || device_tree_loaded("univ-emmc")       // ""
        || device_tree_loaded("univ-hdmi")       // ""
        || device_tree_loaded("univ-nhdmi")))    // ""
    {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s no suitable cape loaded", key);
        return BBIO_CAPE;
    }
    // Do pinmuxing
    if(!strcmp(key, "P9_28")) {
        // ecap2 (P9_28) requires mode pwm2
        // based on bonescript commit 23bf443 by Matthew West
        strncpy(pin_mode, "pwm2", PIN_MODE_LEN);
    } else {
        strncpy(pin_mode, "pwm", PIN_MODE_LEN);
    }
    set_pin_mode(key, pin_mode);

    // Get info for pwm
    err = get_pwm_by_key(key, &p);
    if (err != BBIO_OK) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't get pwm: %i", key, err);
        return err;
    }

    err = build_path(ocp_dir, p->chip, pwm_dev_path, sizeof(pwm_dev_path));
    if (err != BBIO_OK) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't build pwm_dev_path: %i", key, err);
        return err;
    }

    err = build_path(pwm_dev_path, p->addr, pwm_addr_path, sizeof(pwm_addr_path));
    if (err != BBIO_OK) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't build pwm_addr_path: %i", key, err);
        return err;
    }

    err = build_path(pwm_addr_path, "pwm/pwmchip", pwm_chip_path, sizeof(pwm_chip_path));
    if (err != BBIO_OK) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't build pwm_chip_path: %i", key, err);
        return err;
    }

    snprintf(pwm_path, sizeof(pwm_path), "%s/pwm%d", pwm_chip_path, p->index);
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: key: %s, pwm_path: %s", key, pwm_path);

    //pwm with udev patch
    snprintf(pwm_path_udev, sizeof(pwm_path_udev), "%s/pwm-%c:%d", pwm_chip_path, pwm_path[66], p->index);
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: key: %s, pwm_path_udev: %s", key, pwm_path_udev);
    //ecap output with udev patch
    snprintf(ecap_path_udev, sizeof(ecap_path_udev), "%s/pwm-%c:%d", pwm_chip_path, pwm_path[67], p->index);
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: key: %s, ecap_path_udev: %s", key, ecap_path_udev);

    // Export PWM if hasn't already been
    e = stat(pwm_path, &s);
    if (-1 == e) {
        if (ENOENT == errno) { // directory does not exist
            snprintf(pwm_export_path, sizeof(pwm_export_path), "%s/export", pwm_chip_path);
            f = fopen(pwm_export_path, "w");
            if (f == NULL) { // Can't open the export file
                syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
                       key, pwm_export_path, errno, strerror(errno));
                return BBIO_ACCESS;
            }
            fprintf(f, "%d", p->index);
            fclose(f);
            /* sleep to avoid race condition as udev needs the
               opportunity to set group ownership and permission */
            syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: sleep 100 ms after export to avoid udev race condition");
            usleep(100 * 1000); /* 100 ms */
        } else {
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't stat %s: %i-%s",
                   key, pwm_path, errno, strerror(errno));
            perror("stat");
            return BBIO_GEN;
        }
    } else {
        if (S_ISDIR(s.st_mode)) {
            /* It is a directory. Already exported */
        } else {
            /* It's a file. Shouldn't ever happen */
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s %s is not a directory", key, pwm_path);
            return BBIO_GEN;
        }
    }

    e = stat(pwm_path, &s);
    if (-1 == e) {
        if (ENOENT == errno) {
            // Directory still doesn't exist, try the new udev pwm path format in 4.14 kernel
            syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: key: %s pwm_path: %s doesn't exist", key, pwm_path);

            e = stat(pwm_path_udev, &s);
            if (-1 == e) {
                if (ENOENT == errno) {
                    // Directory still doesn't exist, try the new udev ecap path format in 4.14 kernel
                    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: key: %s pwm_path_udev: %s doesn't exist", key, pwm_path_udev);
                    e = stat(ecap_path_udev, &s);
                    if (-1 == e) {
                        if (ENOENT == errno) {
                            // Directory still doesn't exist, exit with error
                            syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: key: %s ecap_path_udev: %s doesn't exist", key, ecap_path_udev);
                            syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: path for %s doesn't exist", key);
                            return BBIO_GEN;
                        }
                    } else {
                        strncpy(pwm_path, ecap_path_udev, sizeof(ecap_path_udev));
                    }
                }
            } else {
              strncpy(pwm_path, pwm_path_udev, sizeof(pwm_path_udev));
	      usleep(100*1000);
            }
        }
    }
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: pwm_path=%s\n", pwm_path);

    snprintf(duty_path, sizeof(duty_path), "%s/duty_cycle", pwm_path);
    snprintf(enable_path, sizeof(enable_path), "%s/enable", pwm_path);
#else
    char fragment[18];
    char pwm_fragment[20];
    char pwm_path[45];
    char duty_path[56];
    char period_path[50];
    char polarity_path[55];
    int period_fd, duty_fd, polarity_fd;

    if (!pwm_initialized) {
        err = initialize_pwm();
        if (err != BBIO_OK) {
            return err;
        }
    }

    // load tree
    snprintf(fragment, sizeof(fragment), "bone_pwm_%s", key);
    err = load_device_tree(fragment);
    if (err != BBIO_OK) {
        //error enabling pin for pwm
        return err;
    }

    //creates the fragment in order to build the pwm_test_filename, such as "pwm_test_P9_13"
    snprintf(pwm_fragment, sizeof(pwm_fragment), "pwm_test_%s", key);

    // Initialize the ocp_dir
    err = build_path("/sys/devices", "ocp", ocp_dir, sizeof(ocp_dir));
    if (err != BBIO_OK) {
        return err;
    }

    //finds and builds the pwm_path, as it can be variable...
    err = build_path(ocp_dir, pwm_fragment, pwm_path, sizeof(pwm_path));
    if (err != BBIO_OK) {
        return err;
    }

    //create the path for duty
    snprintf(duty_path, sizeof(duty_path), "%s/duty", pwm_path);
#endif

    // create the path for period and polarity
    snprintf(period_path, sizeof(period_path), "%s/period", pwm_path);
    snprintf(polarity_path, sizeof(polarity_path), "%s/polarity", pwm_path);

    //add period and duty fd to pwm list
    if ((period_fd = open(period_path, O_RDWR)) < 0) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
               key, period_path, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    if ((duty_fd = open(duty_path, O_RDWR)) < 0) {
        //error, close already opened period_fd.
        close(period_fd);
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
               key, duty_path, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    if ((polarity_fd = open(polarity_path, O_RDWR)) < 0) {
        //error, close already opened period_fd and duty_fd.
        close(period_fd);
        close(duty_fd);
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
               key, polarity_path, errno, strerror(errno));
        return BBIO_SYSFS;
    }

#ifdef BBBVERSION41
    if ((enable_fd = open(enable_path, O_RDWR)) < 0) {
        // error, close already opened files
        close(period_fd);
        close(duty_fd);
        close(polarity_fd);
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't open %s: %i-%s",
               key, enable_path, errno, strerror(errno));
        return BBIO_SYSFS;
    }
#endif

    // add to list
    new_pwm = malloc(sizeof(struct pwm_exp));
    if (new_pwm == NULL) {
        close(period_fd);
        close(duty_fd);
        close(polarity_fd);
#ifdef BBBVERSION41
        close(enable_fd);
#endif
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_setup: %s couldn't malloc pwm_exp: %i-%s",
               key, errno, strerror(errno));
        return BBIO_MEM; // out of memory
    }

    strncpy(new_pwm->key, key, KEYLEN);  /* can leave string unterminated */
    new_pwm->key[KEYLEN] = '\0'; /* terminate string */
    new_pwm->period_fd = period_fd;
    new_pwm->duty_fd = duty_fd;
    new_pwm->polarity_fd = polarity_fd;

#ifdef BBBVERSION41
    new_pwm->enable_fd = enable_fd;
#endif

    new_pwm->next = NULL;

    export_pwm(new_pwm);

    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_setup: %s OK", key);
    return BBIO_OK;
}

BBIO_err pwm_start(const char *key, float duty, float freq, int polarity)
{
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: %s, %f, %f, %i", key, duty, freq, polarity);
    //fprintf(stderr, "Adafruit_BBIO: pwm_start: %s, %f, %f, %i\n", key, duty, freq, polarity);

    BBIO_err err;
    char buffer[20];
    ssize_t len;

    struct pwm_exp *pwm = lookup_exported_pwm(key);
    if (pwm == NULL) {
        err = pwm_setup(key, duty, freq, polarity);
        if (err != BBIO_OK) {
            syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s pwm setup failed: %i", key, err);
            return err;
        }

        pwm = lookup_exported_pwm(key);
    }

    // If we somehow didn't start successfully
    if (pwm == NULL) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s pwm is NULL", key);
        return BBIO_GEN;
    }

    // Read out current period_ns from the file, in order for it to behave
    // properly
    memset(buffer, 0, sizeof(buffer));  // Initialize buffer
    lseek(pwm->period_fd, 0, SEEK_SET);
    len = read(pwm->period_fd, buffer, sizeof(buffer));
    if (len < 0) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't read period: %i-%s",
               key, errno, strerror(errno));
        return BBIO_SYSFS;
    } else if (len >= (ssize_t)sizeof(buffer)) {
        // If this is the case, there's more in the file.
        // This should never happen, as it would mean that
        // the period is 10^8 seconds
        return BBIO_SYSFS;
    }
    // Set the period_ns from the file
    sscanf(buffer, "%lu", &(pwm->period_ns));

    // Read out the current duty_ns from the file, in order for it to
    // behave properly
    memset(buffer, 0, sizeof(buffer));
    lseek(pwm->duty_fd, 0, SEEK_SET);
    len = read(pwm->duty_fd, buffer, sizeof(buffer));
    if (len < 0) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't read duty: %i-%s",
               key, errno, strerror(errno));
        return BBIO_SYSFS;
    } else if (len >= (ssize_t)sizeof(buffer)) {
        // If this is the case, there's more in the file.
        // This should never happen, as it would mean that
        // the period is 10^8 seconds
        return BBIO_SYSFS;
    }
    // Set the duty_ns from the file
    sscanf(buffer, "%lu", &(pwm->duty_ns));

    // Initialize pwm->duty to avoid weirdness
    pwm->duty = duty;
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: call pwm_set_frequency(key=%s freq=%f)", key, freq);
    err = pwm_set_frequency(key, freq);
    if (err != BBIO_OK) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't set duty frequency: %i", key, err);
        return err;
    }

    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: call pwm_set_duty_cycle(key=%s duty=%f)", key, duty);
    err = pwm_set_duty_cycle(key, duty);
    if (err != BBIO_OK) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't set duty cycle: %i", key, err);
        return err;
    }

    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: call pwm_set_polarity(key=%s polarity=%d)", key, polarity);
    err = pwm_set_polarity(key, polarity);
    if (err != BBIO_OK) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't set polarity: %i", key, err);
        return err;
    }

#ifdef BBBVERSION41  // Enable the PWM (don't think it's necessary before 4.1+)
    if (pwm == NULL) {
        return BBIO_GEN;
    }
    len = snprintf(buffer, sizeof(buffer), "1");
    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: write 1 to pwm->enable_fd\n");
    lseek(pwm->enable_fd, 0, SEEK_SET);
    if (write(pwm->enable_fd, buffer, len) < 0) {
        syslog(LOG_ERR, "Adafruit_BBIO: pwm_start: %s couldn't write enable: %i-%s",
               key, errno, strerror(errno));
        return BBIO_SYSFS;
    }
#endif

    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_start: %s OK", key);
    return BBIO_OK;
}


BBIO_err pwm_disable(const char *key)
{
    struct pwm_exp *pwm, *temp, *prev_pwm = NULL;

#ifndef BBBVERSION41
    BBIO_err err;
    char fragment[18];
    snprintf(fragment, sizeof(fragment), "bone_pwm_%s", key);
    err = unload_device_tree(fragment);
    if (err != BBIO_OK)
        return err;
#endif

    // remove from list
    pwm = exported_pwms;
    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0)
        {

#ifdef BBBVERSION41
	        char buffer[2];
	        size_t len;

	        // Disable the PWM
	        lseek(pwm->enable_fd, 0, SEEK_SET);
	        len = snprintf(buffer, sizeof(buffer), "0");
	        if (write(pwm->enable_fd, buffer, len) < 0) {
	          syslog(LOG_ERR, "Adafruit_BBIO: pwm_disable: %s couldn't write enable: %i-%s",
	                 key, errno, strerror(errno));
		        return BBIO_SYSFS;
	        }

	        // Unexport the PWM
	        // TODO later
#endif

            //close the fd
#ifdef BBBVERSION41
            close(pwm->enable_fd);
#endif
            close(pwm->period_fd);
            close(pwm->duty_fd);
            close(pwm->polarity_fd);

            if (prev_pwm == NULL)
            {
                exported_pwms = pwm->next;
                prev_pwm = pwm;
            } else {
                prev_pwm->next = pwm->next;
            }

            temp = pwm;
            pwm = pwm->next;
            free(temp);
        } else {
            prev_pwm = pwm;
            pwm = pwm->next;
        }
    }

    syslog(LOG_DEBUG, "Adafruit_BBIO: pwm_disable: %s OK", key);
    return BBIO_OK;
}

void pwm_cleanup(void)
{
    while (exported_pwms != NULL) {
        pwm_disable(exported_pwms->key);
    }
}
