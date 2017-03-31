################################################################################
# Copyright (c) 2017 Adafruit
# Copyright (c) 2017 Nikolay Semenov
################################################################################

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
# libadafruit-bbio1 - c++11 ABI
# libadafruit-bbio0 - c++98 ABI
lib_LTLIBRARIES = \
	libadafruit-bbio1.la \
	libadafruit-bbio0.la

adafruitdir = $(includedir)/adafruit
adafruit_HEADERS = \
	include/adafruit/bbio.h

adafruit_bbiodir = $(adafruitdir)/bbio
adafruit_bbio_HEADERS = \
	include/adafruit/bbio/error.h \
	include/adafruit/bbio/gpio.h \
	include/adafruit/bbio/pwm.h

LIBSOURCES = \
	c_adc.c \
	c_pinmux.c \
	c_pwm.c \
	c_uart.c \
	common.c \
	event_gpio.c \
	library/bbio.cpp \
	library/gpio.cpp \
	library/pwm.cpp

libadafruit_bbio1_la_SOURCES = $(LIBSOURCES)
libadafruit_bbio0_la_SOURCES = $(LIBSOURCES)

libadafruit_bbio1_la_CXXFLAGS = $(AM_CXXFLAGS) -D_GLIBCXX_USE_CXX11_ABI=1
libadafruit_bbio0_la_CXXFLAGS = $(AM_CXXFLAGS) -D_GLIBCXX_USE_CXX11_ABI=0

libadafruit_bbio1_la_LDFLAGS = $(AM_LDFLAGS) -shared
libadafruit_bbio0_la_LDFLAGS = $(AM_LDFLAGS) -shared

################################################################################
# Examples
################################################################################
noinst_PROGRAMS = examples/cpp/pwm

examples_cpp_pwm_SOURCES = examples/cpp/pwm.cpp
examples_cpp_pwm_LDADD   = libadafruit-bbio1.la
examples_cpp_pwm_LDFLAGS = $(AM_LDFLAGS) -lpthread -static

noinst_PROGRAMS += examples/cpp/gpio

examples_cpp_gpio_SOURCES = examples/cpp/gpio.cpp
examples_cpp_gpio_LDADD   = libadafruit-bbio1.la
examples_cpp_gpio_LDFLAGS = $(AM_LDFLAGS) -lpthread -static

################################################################################
# Tests
################################################################################
check_PROGRAMS =
TESTS = $(check_PROGRAMS)

check_PROGRAMS += test/cpp/abi-old

test_cpp_abi_old_SOURCES  = test/cpp/abi.cpp
test_cpp_abi_old_CXXFLAGS = $(AM_CXXFLAGS) -pthread -D_GLIBCXX_USE_CXX11_ABI=0
test_cpp_abi_old_LDADD    = libadafruit-bbio0.la
test_cpp_abi_old_LDFLAGS  = $(AM_LDFLAGS) -lpthread -static

check_PROGRAMS += test/cpp/abi-new

test_cpp_abi_new_SOURCES  = test/cpp/abi.cpp
test_cpp_abi_new_CXXFLAGS = $(AM_CXXFLAGS) -pthread -D_GLIBCXX_USE_CXX11_ABI=1
test_cpp_abi_new_LDADD    = libadafruit-bbio1.la
test_cpp_abi_new_LDFLAGS  = $(AM_LDFLAGS) -lpthread -static

if HAVE_GTEST

check_PROGRAMS += test/cpp/unittest

gtest/src/gtest-all.cc: /usr/src/gtest/src
	mkdir -p gtest && cp -Rvf $< gtest

gtest/src/gtest_main.cc: gtest/src/gtest-all.cc

BUILT_SOURCES = \
	gtest/src/gtest-all.cc \
	gtest/src/gtest_main.cc

test_cpp_unittest_SOURCES = \
	test/cpp/pwm.cpp \
	gtest/src/gtest-all.cc \
	gtest/src/gtest_main.cc

test_cpp_unittest_CPPFLAGS = $(AM_CPPFLAGS) $(GTEST_CPPFLAGS) -isystem gtest
test_cpp_unittest_CXXFLAGS = $(AM_CXXFLAGS) -pthread -D_GLIBCXX_USE_CXX11_ABI=1
test_cpp_unittest_LDADD    = libadafruit-bbio1.la
test_cpp_unittest_LDFLAGS  = $(AM_LDFLAGS) -lpthread -static

endif # HAVE_GTEST
