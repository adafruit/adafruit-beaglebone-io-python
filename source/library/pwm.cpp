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

#include "adafruit/bbio/pwm.h"

extern "C" {
#include "c_pwm.h"
}

namespace adafruit {
namespace bbio {

/*
 * Pwm
 */
Pwm::Pwm(std::string const& key)
    : key_(key)
{}

Pwm::~Pwm()
{
    stop();
}

void Pwm::start(float duty_cycle, float frequency, Polarity polarity)
{
    (CheckError)pwm_start(key_.c_str(), duty_cycle, frequency, (int)polarity);
}

void Pwm::stop()
{
    (CheckError)pwm_disable(key_.c_str());
}

void Pwm::set_duty_cycle(float duty_cycle)
{
    (CheckError)pwm_set_duty_cycle(key_.c_str(), duty_cycle);
}

void Pwm::set_frequency(float frequency)
{
    (CheckError)pwm_set_frequency(key_.c_str(), frequency);
}

void Pwm::set_polarity(Polarity polarity)
{
    (CheckError)pwm_set_polarity(key_.c_str(), (int)polarity);
}

} // namespace bbio
} // namespace adafruit

