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

#include <gtest/gtest.h>

#include <fstream>

#include <glob.h>

#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,1,0)
#  ifndef BBBVERSION41
#    define BBBVERSION41
#  endif
#endif

using adafruit::bbio::Pwm;
using adafruit::bbio::BBIOError;

class PwmTest : public testing::Test
{
public:
  PwmTest()
  {
    adafruit::bbio::init({ LOG_DEBUG, nullptr, LOG_PERROR });
  }

  void SetUp() override {}
  void TearDown() override {}

  static std::string get_pwm_dir()
  {
    struct auto_glob
    {
      glob_t value;
      ~auto_glob()
      {
        globfree(&value);
      }
    };

#ifdef BBBVERSION41
    // On 4.1+, the pwm subdirectory sometimes takes different names:
    // .pwm or .ehrpwm, etc.

    std::string pattern = "/sys/devices/platform/ocp/48302000.*/"
        "48302200.*/pwm/pwmchip?/pwm0";
#else
    std::string pattern = "/sys/devices/ocp.*/pwm_test_P9_14*";
#endif

    auto_glob results;
    int err = glob(pattern.c_str(), 0, NULL, &results.value);
    if (err) {
      return NULL;
    }

    // We will return the first match
    return results.value.gl_pathv[0];
  }

  static std::string read_file(std::string const& path)
  {
    std::ifstream fstream(path);
    std::stringstream buffer;
    buffer << fstream.rdbuf();
    std::string str = buffer.str();
    if (!str.empty() && str.back() == '\n') {
      str.resize(str.length()-1);
    }
    return str;
  }

  static std::string read_duty_cycle(std::string const& pwm_dir)
  {
#ifdef BBBVERSION41
    return read_file(pwm_dir + "/duty_cycle");
#else
    return read_file(pwm_dir + "/duty");
#endif
  }

  static std::string read_period(std::string const& pwm_dir)
  {
    return read_file(pwm_dir + "/period");
  }

  static std::string read_polarity(std::string const& pwm_dir)
  {
    return read_file(pwm_dir + "/polarity");
  }

  static char const* polarity_str(Pwm::Polarity value)
  {
#ifdef BBBVERSION41
    return value == Pwm::Polarity::Normal ? "normal" : "inversed";
#else
    return value == Pwm::Polarity::Normal ? "0" : "1";
#endif
  }
};

TEST_F(PwmTest, start_pwm)
{
  Pwm pwm("P9_14");
  pwm.start();

  auto pwm_dir = get_pwm_dir();
  ASSERT_FALSE(pwm_dir.empty());

  auto duty = read_duty_cycle(pwm_dir);
  auto period = read_period(pwm_dir);
  ASSERT_EQ(duty, "0");
  ASSERT_EQ(period, "500000");
}

TEST_F(PwmTest, start_pwm_ecap0)
{
  Pwm pwm("P9_42");
  pwm.start(0);

  auto pwm_dir = get_pwm_dir();
  ASSERT_FALSE(pwm_dir.empty());

  auto duty = read_duty_cycle(pwm_dir);
  auto period = read_period(pwm_dir);

  ASSERT_EQ(duty, "0");
  ASSERT_EQ(period, "500000");
}

TEST_F(PwmTest, start_pwm_with_polarity_one)
{
  Pwm pwm("P9_14");
  pwm.start(0, 2000, Pwm::Polarity::Inversed);

  auto pwm_dir = get_pwm_dir();
  ASSERT_FALSE(pwm_dir.empty());

  auto duty = read_duty_cycle(pwm_dir);
  auto period = read_period(pwm_dir);
  auto polarity = read_polarity(pwm_dir);

  ASSERT_EQ(duty, "0");
  ASSERT_EQ(period, "500000");
  ASSERT_EQ(polarity, polarity_str(Pwm::Polarity::Inversed));
}

TEST_F(PwmTest, start_pwm_with_polarity_default)
{
  Pwm pwm("P9_14");
  pwm.start(0, 2000);

  auto pwm_dir = get_pwm_dir();
  ASSERT_FALSE(pwm_dir.empty());

  auto duty = read_duty_cycle(pwm_dir);
  auto period = read_period(pwm_dir);
  auto polarity = read_polarity(pwm_dir);

  ASSERT_EQ(duty, "0");
  ASSERT_EQ(period, "500000");
  ASSERT_EQ(polarity, polarity_str(Pwm::Polarity::Normal));
}

TEST_F(PwmTest, start_pwm_with_polarity_zero)
{
  Pwm pwm("P9_14");
  pwm.start(0, 2000, Pwm::Polarity::Normal);

  auto pwm_dir = get_pwm_dir();
  ASSERT_FALSE(pwm_dir.empty());

  auto duty = read_duty_cycle(pwm_dir);
  auto period = read_period(pwm_dir);
  auto polarity = read_polarity(pwm_dir);

  ASSERT_EQ(duty, "0");
  ASSERT_EQ(period, "500000");
  ASSERT_EQ(polarity, polarity_str(Pwm::Polarity::Normal));
}

TEST_F(PwmTest, pwm_start_invalid_pwm_key)
{
  Pwm pwm("P8_25");
  ASSERT_THROW(pwm.start(), BBIOError);
}

TEST_F(PwmTest, pwm_start_invalid_duty_cycle_negative)
{
  Pwm pwm("P9_14");
  ASSERT_THROW(pwm.start(-1), BBIOError);
}

TEST_F(PwmTest, pwm_start_valid_duty_cycle_min)
{
  // testing an exception isn't thrown
  Pwm pwm("P9_14");
  pwm.start(0);
}

TEST_F(PwmTest, pwm_start_valid_duty_cycle_max)
{
  // testing an exception isn't thrown
  Pwm pwm("P9_14");
  pwm.start(100);
}

TEST_F(PwmTest, pwm_start_invalid_duty_cycle_high)
{
  Pwm pwm("P9_14");
  ASSERT_THROW(pwm.start(101), BBIOError);
}

TEST_F(PwmTest, pwm_start_invalid_frequency_negative)
{
  Pwm pwm("P9_14");
  ASSERT_THROW(pwm.start(0, -1), BBIOError);
}

TEST_F(PwmTest, pwm_start_negative_polarity)
{
  Pwm pwm("P9_14");
  ASSERT_THROW(pwm.start(0, 100, static_cast<Pwm::Polarity>(-1)), BBIOError);
}

TEST_F(PwmTest, pwm_start_invalid_positive_polarity)
{
  Pwm pwm("P9_14");
  ASSERT_THROW(pwm.start(0, 100, static_cast<Pwm::Polarity>(2)), BBIOError);
}

TEST_F(PwmTest, pwm_duty_modified)
{
  Pwm pwm("P9_14");
  pwm.start(0);

  auto pwm_dir = get_pwm_dir();
  ASSERT_FALSE(pwm_dir.empty());

  {
    auto duty = read_duty_cycle(pwm_dir);
    auto period = read_period(pwm_dir);
    ASSERT_EQ(duty, "0");
    ASSERT_EQ(period, "500000");
  }
  {
    pwm.set_duty_cycle(100);
    auto duty = read_duty_cycle(pwm_dir);
    auto period = read_period(pwm_dir);
    ASSERT_EQ(duty, "500000");
    ASSERT_EQ(period, "500000");
  }
}

TEST_F(PwmTest, pwm_duty_cycle_non_setup_key)
{
  Pwm pwm("P9_14");
  ASSERT_THROW(pwm.set_duty_cycle(100), BBIOError);
}

TEST_F(PwmTest, pwm_duty_cycle_invalid_key)
{
  Pwm pwm("P9_15");
  ASSERT_THROW(pwm.set_duty_cycle(100), BBIOError);
}

TEST_F(PwmTest, pwm_duty_cycle_invalid_value_high)
{
  Pwm pwm("P9_14");
  pwm.start(0);
  ASSERT_THROW(pwm.set_duty_cycle(101), BBIOError);
}

TEST_F(PwmTest, pwm_duty_cycle_invalid_value_negative)
{
  Pwm pwm("P9_14");
  pwm.start(0);
  ASSERT_THROW(pwm.set_duty_cycle(-1), BBIOError);
}

TEST_F(PwmTest, pwm_frequency_invalid_value_negative)
{
  Pwm pwm("P9_14");
  pwm.start(0);
  ASSERT_THROW(pwm.set_frequency(-1), BBIOError);
}

TEST_F(PwmTest, pwm_freq_non_setup_key)
{
  Pwm pwm("P9_14");
  ASSERT_THROW(pwm.set_frequency(100), BBIOError);
}

TEST_F(PwmTest, pwm_freq_non_setup_invalid_key)
{
  Pwm pwm("P9_15");
  ASSERT_THROW(pwm.set_frequency(100), BBIOError);
}
