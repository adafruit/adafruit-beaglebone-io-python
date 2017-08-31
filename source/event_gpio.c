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

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <syslog.h>
#include <unistd.h>

#include "event_gpio.h"
#include "common.h"

#define GPIO_NOT_EXPORTED 0
#define GPIO_EXPORTED 1
#define GPIO_ALREADY_EXPORTED 2

const char *stredge[4] = {"none", "rising", "falling", "both"};

// file descriptors
struct fdx
{
    int fd;
    unsigned int gpio;
    int initial;
    unsigned int is_evented;
    struct fdx *next;
};
struct fdx *fd_list = NULL;

// event callbacks
struct callback
{
    unsigned int gpio;
    void (*func)(unsigned int gpio);
    struct callback *next;
};
struct callback *callbacks = NULL;

pthread_t threads;
int exported_gpios[120] = { GPIO_NOT_EXPORTED };
int event_occurred[120] = { 0 };
int thread_running = 0;
int epfd = -1;

BBIO_err gpio_export(unsigned int gpio)
{
    int fd = 0, len = 0, ret = BBIO_GEN;
    char str_gpio[10];

    // already exported by us?
    if (exported_gpios[gpio] != GPIO_NOT_EXPORTED) {
        syslog(LOG_DEBUG, "gpio_export: %u already exported", gpio);
        ret = BBIO_OK;
        goto exit;
    }

    // already exported by someone else?
    char gpio_path[64];
    snprintf(gpio_path, sizeof(gpio_path), "/sys/class/gpio/gpio%d", gpio);

    if (access(gpio_path, R_OK|W_OK|X_OK) != -1) {
        exported_gpios[gpio] = GPIO_ALREADY_EXPORTED;
        syslog(LOG_DEBUG, "gpio_export: %u already exported", gpio);
        ret =  BBIO_OK;
        goto exit;
    }

    const char gpio_export[] = "/sys/class/gpio/export";

    if ((fd = open(gpio_export, O_WRONLY)) < 0) {
        syslog(LOG_ERR, "gpio_export: %u couldn't open \"%s\": %i-%s",
               gpio, gpio_export, errno, strerror(errno));
        ret =  BBIO_SYSFS;
        goto exit;
    }

    len = snprintf(str_gpio, sizeof(str_gpio), "%d", gpio);
    if(write(fd, str_gpio, len) < 0) {
        syslog(LOG_ERR, "gpio_export: %u couldn't write \"%s\": %i-%s",
               gpio, gpio_export, errno, strerror(errno));
        ret =  BBIO_SYSFS;
        goto exit;
    }

    // add to list
    exported_gpios[gpio] = GPIO_EXPORTED;

    syslog(LOG_DEBUG, "gpio_export: %u OK", gpio);
    ret = BBIO_OK;

    exit:
    if(fd && (ret = close(fd))) {
        syslog(LOG_ERR, "gpio_export: %u couldn't close \"%s\": %i-%s",
               gpio, gpio_export, errno, strerror(errno));
        ret =  BBIO_SYSFS;
    }
    usleep(100000);      // Hack to wait for newly exported pins to get correct ownership
    return ret;
}

void close_value_fd(unsigned int gpio)
{
    struct fdx *f = fd_list;
    struct fdx *temp;
    struct fdx *prev = NULL;

    while (f != NULL)
    {
        if (f->gpio == gpio)
        {
            close(f->fd);
            if (prev == NULL)
                fd_list = f->next;
            else
                prev->next = f->next;
            temp = f;
            f = f->next;
            free(temp);
        } else {
            prev = f;
            f = f->next;
        }
    }
}

int fd_lookup(unsigned int gpio)
{
    struct fdx *f = fd_list;
    while (f != NULL)
    {
        if (f->gpio == gpio)
            return f->fd;
        f = f->next;
    }
    return 0;
}

int add_fd_list(unsigned int gpio, int fd)
{
    struct fdx *new_fd;

    new_fd = malloc(sizeof(struct fdx));
    if (new_fd == 0)
        return -1;  // out of memory

    new_fd->fd = fd;
    new_fd->gpio = gpio;
    new_fd->initial = 1;
    new_fd->is_evented = 0;
    if (fd_list == NULL) {
        new_fd->next = NULL;
    } else {
        new_fd->next = fd_list;
    }
    fd_list = new_fd;
    return 0;
}

int open_value_file(unsigned int gpio)
{
    int fd;
    char filename[MAX_FILENAME];

    // create file descriptor of value file
    if ((gpio >= USR_LED_GPIO_MIN) && (gpio <=  USR_LED_GPIO_MAX)) {
        snprintf(filename, sizeof(filename), "/sys/class/leds/beaglebone:green:usr%d/brightness", gpio -  USR_LED_GPIO_MIN);
    } else if(gpio == USR_LED_RED) {     // red LED
        snprintf(filename, sizeof(filename), "/sys/class/leds/red/brightness");
    } else if(gpio == USR_LED_GREEN) {     // green LED
        snprintf(filename, sizeof(filename), "/sys/class/leds/green/brightness");
    } else {
        snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/value", gpio);
    }

    if ((fd = open(filename, O_RDONLY | O_NONBLOCK)) < 0) {
        syslog(LOG_ERR, "gpio open_value_file: %u couldn't open '%s': %i-%s",
               gpio, filename, errno, strerror(errno));
        return -1;
    }
    add_fd_list(gpio, fd);
    return fd;
}

BBIO_err gpio_unexport(unsigned int gpio)
{
    int fd, len;
    char str_gpio[10];

    if (exported_gpios[gpio] != GPIO_EXPORTED)
        return 0;

    close_value_fd(gpio);

#define GPIO_UNEXPORT "/sys/class/gpio/unexport"

    if ((fd = open(GPIO_UNEXPORT, O_WRONLY)) < 0) {
      syslog(LOG_ERR, "gpio_unexport: %u couldn't open '"GPIO_UNEXPORT"': %i-%s",
             gpio, errno, strerror(errno));
      return BBIO_SYSFS;
    }

    len = snprintf(str_gpio, sizeof(str_gpio), "%d", gpio);
    int ret = write(fd, str_gpio, len);
    close(fd);
    if (ret < 0) {
      syslog(LOG_ERR, "gpio_unexport: %u couldn't write '"GPIO_UNEXPORT"': %i-%s",
             gpio, errno, strerror(errno));
      return BBIO_SYSFS;
    }

    // remove from list
    exported_gpios[gpio] = GPIO_NOT_EXPORTED;

    syslog(LOG_DEBUG, "gpio_unexport: %u OK", gpio);
    return BBIO_OK;
}

BBIO_err gpio_set_direction(unsigned int gpio, unsigned int in_flag)
{
        int fd;
        char filename[40];
        char direction[10] = { 0 };

        if (((gpio >= USR_LED_GPIO_MIN) && (gpio <=  USR_LED_GPIO_MAX)) ||
                (gpio == USR_LED_RED) || (gpio == USR_LED_GREEN)) {
            syslog(LOG_DEBUG, "gpio_set_direction: %u not applicable to the USR LED", gpio);
            return BBIO_OK; // direction is not applicable to the USR LED pins
        }

        snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/direction", gpio);
        if ((fd = open(filename, O_WRONLY)) < 0) {
            syslog(LOG_ERR, "gpio_set_direction: %u couldn't open '%s': %i-%s",
                   gpio, filename, errno, strerror(errno));
            return BBIO_SYSFS;
        }

        if (in_flag) {
            strncpy(direction, "out", ARRAY_SIZE(direction) - 1);
        } else {
            strncpy(direction, "in", ARRAY_SIZE(direction) - 1);
        }

        int ret = write(fd, direction, strlen(direction));
        close(fd);
        if (ret < 0) {
            syslog(LOG_ERR, "gpio_set_direction: %u couldn't write '%s': %i-%s",
                   gpio, filename, errno, strerror(errno));
            return BBIO_SYSFS;
        }

        syslog(LOG_DEBUG, "gpio_set_direction: %u OK", gpio);
        return BBIO_OK;
}

BBIO_err gpio_get_direction(unsigned int gpio, unsigned int *value)
{
    int fd;
    char direction[4] = { 0 };
    char filename[40];

    snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/direction", gpio);
    if ((fd = open(filename, O_RDONLY | O_NONBLOCK)) < 0) {
        syslog(LOG_ERR, "gpio_get_direction: %u couldn't open '%s': %i-%s",
               gpio, filename, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    lseek(fd, 0, SEEK_SET);
    int ret = read(fd, &direction, sizeof(direction) - 1);
    close(fd);
    if (ret < 0) {
        syslog(LOG_ERR, "gpio_get_direction: %u couldn't read '%s': %i-%s",
               gpio, filename, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    if (strcmp(direction, "out") == 0) {
        *value = OUTPUT;
    } else {
        *value = INPUT;
    }

    syslog(LOG_DEBUG, "gpio_get_direction: %u OK", gpio);
    return BBIO_OK;
}

BBIO_err gpio_set_value(unsigned int gpio, unsigned int value)
{
    int fd;
    char filename[MAX_FILENAME];
    char vstr[10];

    if ((gpio >= USR_LED_GPIO_MIN) && (gpio <=  USR_LED_GPIO_MAX)) {

        char *usr_led_trigger[] = { "heartbeat", "mmc0", "cpu0", "mmc1" }; 
        int led = gpio -  USR_LED_GPIO_MIN;

        snprintf(filename, sizeof(filename), "/sys/class/leds/beaglebone:green:usr%d/brightness", led);
        if (access(filename, W_OK) < 0) {
           snprintf(filename, sizeof(filename), "/sys/class/leds/beaglebone:green:%s/brightness", usr_led_trigger[led]);
        }
    
    } else if(gpio == USR_LED_RED) {
        snprintf(filename, sizeof(filename), "/sys/class/leds/red/brightness");
    } else if(gpio == USR_LED_GREEN) {
        snprintf(filename, sizeof(filename), "/sys/class/leds/green/brightness");
    } else {
        snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/value", gpio);
    }

    fd = open(filename, O_WRONLY);
    if (fd < 0) {
        syslog(LOG_ERR, "gpio_set_value: %u couldn't open '%s': %i-%s",
               gpio, filename, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    if (value) {
        strncpy(vstr, "1", ARRAY_SIZE(vstr) - 1);
    } else {
        strncpy(vstr, "0", ARRAY_SIZE(vstr) - 1);
    }

    int ret = write(fd, vstr, strlen(vstr));
    close(fd);
    if (ret < 0) {
        syslog(LOG_ERR, "gpio_set_value: %u couldn't write '%s': %i-%s",
               gpio, filename, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    syslog(LOG_DEBUG, "gpio_set_value: %u %u OK", gpio, value);
    return BBIO_OK;
}

BBIO_err gpio_get_value(unsigned int gpio, unsigned int *value)
{
    int fd = fd_lookup(gpio);
    char ch;

    if (!fd)
    {
        if ((fd = open_value_file(gpio)) == -1) {
            syslog(LOG_ERR, "gpio_set_value: %u couldn't open value file: %i-%s",
                   gpio, errno, strerror(errno));
            return BBIO_SYSFS;
        }
    }

    lseek(fd, 0, SEEK_SET);
    int ret = read(fd, &ch, sizeof(ch));
    if (ret < 0) {
        syslog(LOG_ERR, "gpio_set_value: %u couldn't read value file: %i-%s",
               gpio, errno, strerror(errno));
        return BBIO_SYSFS;
    }

    if (ch != '0') {
        *value = 1;
    } else {
        *value = 0;
    }

    syslog(LOG_DEBUG, "gpio_get_value: %u OK", gpio);
    return BBIO_OK;
}

int gpio_set_edge(unsigned int gpio, unsigned int edge)
{
        int fd;
        char filename[40];

        snprintf(filename, sizeof(filename), "/sys/class/gpio/gpio%d/edge", gpio);

        if ((fd = open(filename, O_WRONLY)) < 0)
        return -1;

        int ret = write(fd, stredge[edge], strlen(stredge[edge]) + 1);
        close(fd);
        if (ret < 0) {
        	return ret;
        }

        return 0;
}

unsigned int gpio_lookup(int fd)
{
    struct fdx *f = fd_list;
    while (f != NULL)
    {
        if (f->fd == fd)
            return f->gpio;
        f = f->next;
    }
    return 0;
}

void exports_cleanup(void)
{
    int i;
    // unexport everything
    for (i = 0; i < 120; ++i)
        gpio_unexport(i);
}

int add_edge_callback(unsigned int gpio, void (*func)(unsigned int gpio))
{
    struct callback *cb = callbacks;
    struct callback *new_cb;

    new_cb = malloc(sizeof(struct callback));
    if (new_cb == 0)
        return -1;  // out of memory

    new_cb->gpio = gpio;
    new_cb->func = func;
    new_cb->next = NULL;

    if (callbacks == NULL) {
        // start new list
        callbacks = new_cb;
    } else {
        // add to end of list
        while (cb->next != NULL)
            cb = cb->next;
        cb->next = new_cb;
    }
    return 0;
}

void run_callbacks(unsigned int gpio)
{
    struct callback *cb = callbacks;
    while (cb != NULL)
    {
        if (cb->gpio == gpio)
            cb->func(cb->gpio);
        cb = cb->next;
    }
}

void remove_callbacks(unsigned int gpio)
{
    struct callback *cb = callbacks;
    struct callback *temp;
    struct callback *prev = NULL;

    while (cb != NULL)
    {
        if (cb->gpio == gpio)
        {
            if (prev == NULL)
                callbacks = cb->next;
            else
                prev->next = cb->next;
            temp = cb;
            cb = cb->next;
            free(temp);
        } else {
            prev = cb;
            cb = cb->next;
        }
    }
}

void set_initial_false(unsigned int gpio)
{
    struct fdx *f = fd_list;

    while (f != NULL)
    {
        if (f->gpio == gpio)
            f->initial = 0;
        f = f->next;
    }
}

int gpio_initial(unsigned int gpio)
{
    struct fdx *f = fd_list;

    while (f != NULL)
    {
        if ((f->gpio == gpio) && f->initial)
            return 1;
        f = f->next;
    }
    return 0;
}

void *poll_thread(__attribute__ ((unused)) void *threadarg)
{
    struct epoll_event events;
    char buf;
    unsigned int gpio;
    int n;

    thread_running = 1;
    while (thread_running)
    {
        if ((n = epoll_wait(epfd, &events, 1, -1)) == -1)
        {
            thread_running = 0;
            pthread_exit(NULL);
        }
        if (n > 0) {
            lseek(events.data.fd, 0, SEEK_SET);
            if (read(events.data.fd, &buf, 1) != 1)
            {
                thread_running = 0;
                pthread_exit(NULL);
            }
            gpio = gpio_lookup(events.data.fd);
            if (gpio_initial(gpio)) {     // ignore first epoll trigger
                set_initial_false(gpio);
            } else {
                event_occurred[gpio] = 1;
                run_callbacks(gpio);
            }
        }
    }
    thread_running = 0;
    pthread_exit(NULL);
}

int gpio_is_evented(unsigned int gpio)
{
    struct fdx *f = fd_list;
    while (f != NULL)
    {
        if (f->gpio == gpio)
            return 1;
        f = f->next;
    }
    return 0;
}

int gpio_event_add(unsigned int gpio)
{
    struct fdx *f = fd_list;
    while (f != NULL)
    {
        if (f->gpio == gpio)
        {
            if (f->is_evented)
                return 1;

            f->is_evented = 1;
            return 0;
        }
        f = f->next;
    }
    return 0;
}

int gpio_event_remove(unsigned int gpio)
{
    struct fdx *f = fd_list;
    while (f != NULL)
    {
        if (f->gpio == gpio)
        {
            f->is_evented = 0;
            return 0;
        }
        f = f->next;
    }
    return 0;
}

int add_edge_detect(unsigned int gpio, unsigned int edge)
// return values:
// 0 - Success
// 1 - Edge detection already added
// 2 - Other error
{
    int fd = fd_lookup(gpio);
    pthread_t threads;
    struct epoll_event ev;
    long t = 0;

    // check to see if this gpio has been added already
    if (gpio_event_add(gpio) != 0)
        return 1;

    // export /sys/class/gpio interface
    gpio_export(gpio);
    gpio_set_direction(gpio, 0); // 0=input
    gpio_set_edge(gpio, edge);

    if (!fd)
    {
        if ((fd = open_value_file(gpio)) == -1)
            return 2;
    }

    // create epfd if not already open
    if ((epfd == -1) && ((epfd = epoll_create(1)) == -1))
        return 2;

    // add to epoll fd
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
        return 2;

    // start poll thread if it is not already running
    if (!thread_running)
    {
        if (pthread_create(&threads, NULL, poll_thread, (void *)t) != 0)
            return 2;
    }

    return 0;
}

void remove_edge_detect(unsigned int gpio)
{
    struct epoll_event ev;
    int fd = fd_lookup(gpio);

    // delete callbacks for gpio
    remove_callbacks(gpio);

    // delete epoll of fd
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);

    // set edge to none
    gpio_set_edge(gpio, NO_EDGE);

    // unexport gpio
    gpio_event_remove(gpio);

    // clear detected flag
    event_occurred[gpio] = 0;
}

int event_detected(unsigned int gpio)
{
    if (event_occurred[gpio]) {
        event_occurred[gpio] = 0;
        return 1;
    } else {
        return 0;
    }
}

void event_cleanup(void)
{
    close(epfd);
    thread_running = 0;
    exports_cleanup();
}

int blocking_wait_for_edge(unsigned int gpio, unsigned int edge, int timeout)
// standalone from all the event functions above
{
    int fd = fd_lookup(gpio);
    int epfd, n, i;
    struct epoll_event events, ev;
    char buf;

    if ((epfd = epoll_create(1)) == -1)
        return 1;

    // check to see if this gpio has been added already, if not, mark as added
    if (gpio_event_add(gpio) != 0)
        return 2;

    // export /sys/class/gpio interface
    gpio_export(gpio);
    gpio_set_direction(gpio, 0); // 0=input
    gpio_set_edge(gpio, edge);

    if (!fd)
    {
        if ((fd = open_value_file(gpio)) == -1)
            return 3;
    }

    // add to epoll fd
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
    ev.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        gpio_event_remove(gpio);
        return 4;
    }

    // epoll for event
    for (i = 0; i<2; i++) // first time triggers with current state, so ignore
       if ((n = epoll_wait(epfd, &events, 1, timeout)) == -1)
       {
           gpio_event_remove(gpio);
           return 5;
       }

    if (n > 0)
    {
        lseek(events.data.fd, 0, SEEK_SET);
        if (read(events.data.fd, &buf, sizeof(buf)) != 1)
        {
            gpio_event_remove(gpio);
            return 6;
        }
        if (events.data.fd != fd)
        {
            gpio_event_remove(gpio);
            return 7;
        }
    }

    gpio_event_remove(gpio);
    close(epfd);
    return (n == 1) ? 0 : -1;
}
