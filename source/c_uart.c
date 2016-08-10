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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "c_uart.h"
#include "common.h"

BBIO_err uart_setup(const char *dt)
{
    return load_device_tree(dt);
}

BBIO_err uart_cleanup(void)
{
    BBIO_err e1, e2, e3, e4, e5;
    e1 = unload_device_tree("ADAFRUIT-UART1");
    e2 = unload_device_tree("ADAFRUIT-UART2");
    e3 = unload_device_tree("ADAFRUIT-UART3");
    e4 = unload_device_tree("ADAFRUIT-UART4");
    e5 = unload_device_tree("ADAFRUIT-UART5");
    if (e1 != BBIO_OK)
        return e1;
    if (e2 != BBIO_OK)
        return e1;
    if (e3 != BBIO_OK)
        return e1;
    if (e4 != BBIO_OK)
        return e1;
    if (e5 != BBIO_OK)
        return e1;

    return BBIO_OK;
}
