AC_INIT([Adafruit BeagleBone IO Library],
        [1.0.0],
        [https://github.com/adafruit/adafruit-beaglebone-io-python/issues],
        [adafruit-bbio])

AC_PREREQ([2.59])
#AC_CONFIG_SRCDIR([./LICENSE])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile source/Makefile])

AM_INIT_AUTOMAKE([1.9 dist-bzip2 dist-zip foreign subdir-objects -Wall])
AM_PROG_AR

LT_INIT

AC_PROG_CC
AC_PROG_CXX
AC_LANG([C])
AC_LANG([C++])

CHECK_GTEST
CHECK_KERNEL_RELEASE

AC_OUTPUT
