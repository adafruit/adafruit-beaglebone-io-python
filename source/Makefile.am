################################################################################
# Copyright (c) 2017 Adafruit
# Copyright (c) 2017 Nikolay Semenov
################################################################################

lib_LTLIBRARIES =
noinst_PROGRAMS =

################################################################################
# Global flags
################################################################################
AM_CPPFLAGS =

AM_CPPFLAGS += \
	-DNDEBUG \
	-g -O2 \
	-fstack-protector-strong \
	-fPIC

AM_CPPFLAGS += \
	-Wall \
	-Wextra \
	-Werror

AM_CPPFLAGS += \
	-DNO_PYTHON

AM_CPPFLAGS += \
	-I$(srcdir)/include/

AM_CXXFLAGS = \
	-std=c++11

################################################################################
# AdafruitBBIO library
################################################################################
lib_LTLIBRARIES += libadafruit-bbio.la

libadafruit_bbio_la_SOURCES = \
	c_adc.c \
	c_pinmux.c \
	c_pwm.c \
	c_uart.c \
	common.c \
	event_gpio.c \
	library/bbio.cpp \
	library/gpio.cpp \
	library/pwm.cpp

libadafruit_bbio_la_LDFLAGS = $(AM_LDFLAGS) -shared

################################################################################
# Examples
################################################################################
noinst_PROGRAMS += examples/cpp/pwm

examples_cpp_pwm_SOURCES = examples/cpp/pwm.cpp
examples_cpp_pwm_LDADD = libadafruit-bbio.la
examples_cpp_pwm_LDFLAGS = $(AM_LDFLAGS) -lpthread -static

noinst_PROGRAMS += examples/cpp/gpio

examples_cpp_gpio_SOURCES = examples/cpp/gpio.cpp
examples_cpp_gpio_LDADD = libadafruit-bbio.la
examples_cpp_gpio_LDFLAGS = $(AM_LDFLAGS) -lpthread -static
