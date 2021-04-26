// MIT License
//
// Copyright (c) 2019 NERSC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "mm.hpp"

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <random>

#include <timemory/timemory.hpp>

using lwtimer_t = tim::lightweight_tuple<tim::component::wall_clock>;

namespace
{
std::default_random_engine rng{};
lwtimer_t                  ntimer{ "MM average" };
}  // namespace

// returns number of instrumentations triggered
void
mm(int64_t s, double* a, double* b, double* c)
{
    for(int64_t i = 0; i < s; ++i)
    {
        for(int64_t j = 0; j < s; ++j)
        {
            lwtimer_t _timer{};
            _timer.start();
            for(int64_t k = 0; k < s; ++k)
            {
                a[i * s + j] += b[i * s + k] * c[k * s + j];
            }
            ntimer += _timer.stop();
        }
    }
}

// returns sum of a
double
mm_sum(int64_t s, double* a)
{
    double sum = 0.0;
    for(int64_t i = 0; i < s; ++i)
    {
        for(int64_t j = 0; j < s; ++j)
        {
            sum += a[i * s + j];
        }
    }
    return sum;
}

extern "C" void
set_seed(int64_t v)
{
    rng.seed(v);
}

double
get_random()
{
    return std::generate_canonical<double, 16>(rng);
}

void
mm_reset(int64_t s, double* a, double* b, double* c)
{
    for(int64_t i = 0; i < s; ++i)
    {
        for(int64_t j = 0; j < s; ++j)
        {
            a[i * s + j] = 0.0;
            b[i * s + j] = 1.0 * get_random() - 0.5;
            c[i * s + j] = 2.0 * get_random() - 1.0;
        }
    }
}

double
execute_matmul(int64_t s, int64_t imax, int64_t nitr)
{
    printf("\nRunning %" PRId64 " MM on %" PRId64 " x %" PRId64 "\n", imax, s, s);

    auto _a = std::vector<double>(s * s, 0.0);
    auto _b = std::vector<double>(s * s, 0.0);
    auto _c = std::vector<double>(s * s, 0.0);

    auto a = _a.data();
    auto b = _b.data();
    auto c = _c.data();

    double sum = 0.0;
    for(int64_t i = 0; i < nitr; ++i)
    {
        mm_reset(s, a, b, c);
        for(int64_t iter = 0; iter < imax; iter++)
            mm(s, a, b, c);
        sum += mm_sum(s, a);
    }

    std::cout << ntimer / ntimer.laps() << std::endl;

    return sum;
}
