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
namespace adafruit {
namespace bbio {
#endif

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
} // namespace bbio
} // namespace adafruit
#endif
