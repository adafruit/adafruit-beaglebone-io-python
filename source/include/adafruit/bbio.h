/*
Copyright (c) 2017 Adafruit
Copyright (c) 2017 Nikolay Semenov

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
#pragma once

#include <adafruit/bbio/error.h>
#include <adafruit/bbio/gpio.h>
#include <adafruit/bbio/pwm.h>

#include <syslog.h>

namespace adafruit {
namespace bbio {

struct lib_options
{
    int syslog_level;

    // If NULL then syslog uses process name.
    char const* syslog_identifier;

    // Add LOG_PERROR to print to console
    int syslog_option;

    lib_options(int syslog_level = LOG_INFO,
                char const* syslog_identifier = nullptr,
                int syslog_option = LOG_CONS | LOG_PID | LOG_NDELAY)
        : syslog_level(syslog_level)
        , syslog_identifier(syslog_identifier)
        , syslog_option(syslog_option)
    {}
};

/*
 * Initialize the library.
 */
void init(lib_options = lib_options());

} // namespace bbio
} // namespace adafruit
