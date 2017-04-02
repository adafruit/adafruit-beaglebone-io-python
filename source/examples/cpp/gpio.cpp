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

#include "adafruit/bbio.h"

#include <chrono>
#include <thread>

int main(int /*argc*/, char** /*argv*/)
{
    using adafruit::bbio::lib_options;
    using adafruit::bbio::Gpio;

    adafruit::bbio::init(lib_options(LOG_DEBUG, nullptr, LOG_PERROR));

    Gpio gpio("P8_10", Gpio::Direction::Output);

    for (int i = 0; i < 100; ++i) {
        gpio.set_value((i % 2 == 1) ? Gpio::Value::High : Gpio::Value::Low);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
