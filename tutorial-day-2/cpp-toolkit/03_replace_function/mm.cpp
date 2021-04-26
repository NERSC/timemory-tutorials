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

#include <timemory/timemory.hpp>

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <random>

TIMEMORY_DEFINE_API(mm)

#if defined(DISABLE_MM_API)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_available, api::mm, false_type)
#endif

namespace quirk = tim::quirk;
namespace comp  = tim::component;
namespace api   = tim::api;
using bundle_t =
    tim::component_bundle<api::mm, comp::wall_clock, quirk::config<quirk::auto_start>,
                          comp::user_global_bundle>;
using stop_last_config_t = quirk::config<quirk::stop_last_bundle>;

namespace
{
std::default_random_engine rng{};
}  // namespace

// returns number of instrumentations triggered
void
mm(int64_t s, double* a, double* b, double* c)
{
    for(int64_t i = 0; i < s; ++i)
    {
        for(int64_t j = 0; j < s; ++j)
        {
            for(int64_t k = 0; k < s; ++k)
            {
                a[i * s + j] += b[i * s + k] * c[k * s + j];
            }
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

namespace foo
{
double
get_other_random()
{
    return get_random();
}
}  // namespace foo

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

    bundle_t _bundle{ __FUNCTION__ };

    auto _a = std::vector<double>(s * s, 0.0);
    auto _b = std::vector<double>(s * s, 0.0);
    auto _c = std::vector<double>(s * s, 0.0);

    auto a = _a.data();
    auto b = _b.data();
    auto c = _c.data();

    double sum = 0.0;
    for(int64_t i = 0; i < nitr; ++i)
    {
        bundle_t _ibundle{ "reset" };
        mm_reset(s, a, b, c);

        bundle_t _mbundle{ "mm", stop_last_config_t{} };
        for(int64_t iter = 0; iter < imax; iter++)
            mm(s, a, b, c);

        bundle_t _sbundle{ "sum", stop_last_config_t{} };
        sum += mm_sum(s, a);
    }

    return sum;
}
