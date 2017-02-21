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

#ifdef __cplusplus

#include <stdexcept>

namespace adafruit {
namespace bbio {

#endif // __cplusplus

typedef enum {
	BBIO_OK, // No error
	BBIO_ACCESS, // Error accessing a file
	BBIO_SYSFS, // Some error with Sysfs files
	BBIO_CAPE, // some error with capes
	BBIO_INVARG, // Invalid argument
	BBIO_MEM,
	BBIO_GEN // General error
} BBIO_err;

#ifdef __cplusplus

class BBIOError : public std::runtime_error
{
public:
    BBIOError(BBIO_err code, std::string const& message = "")
        : std::runtime_error(
                std::to_string(code) + "-" + to_c_str(code) + " " + message)
    {}

    static char const* to_c_str(BBIO_err err)
    {
        switch (err) {
            case BBIO_OK:     return "OK";
            case BBIO_ACCESS: return "ACCESS";
            case BBIO_SYSFS:  return "SYSFS";
            case BBIO_CAPE:   return "CAPE";
            case BBIO_INVARG: return "INVARG";
            case BBIO_MEM:    return "MEMORY";
            case BBIO_GEN:    return "GENERAL";
            default:          return "<INVALID>";
        }
    }
};

class CheckError
{
public:
    CheckError(BBIO_err code)
    {
        if (code != BBIO_OK) {
            throw BBIOError(code);
        }
    }
    CheckError(int code)
    {
        if (code != BBIO_OK) {
            throw BBIOError((BBIO_err)code);
        }
    }
};

} // namespace bbio
} // namespace adafruit

#endif // __cplusplus
