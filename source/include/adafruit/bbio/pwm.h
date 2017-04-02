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

#include <string>

namespace adafruit {
namespace bbio {

class Pwm
{
public:
    enum class Polarity
    {
        Normal = 0,
        Inversed = 1,
    };

    Pwm(std::string const& key);
    ~Pwm();

    void start(float duty_cycle = 0.0, float frequency = 2000.0, Polarity = Polarity::Normal);
    void stop();

    // 0.0 <= duty_cycle <= 100.0
    void set_duty_cycle(float);
    void set_frequency(float);
    void set_polarity(Polarity);

private:
    std::string key_;
};

} // namespace bbio
} // namespace adafruit
