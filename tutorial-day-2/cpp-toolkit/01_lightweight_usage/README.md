# Lightweight Usage

Consider the following timing implementation in [mm.cpp](mm.cpp):

```cpp
namespace
{
uint64_t nseconds = 0;
uint64_t ncount = 0;
}

void
mm(int64_t s, double* a, double* b, double* c)
{
    for(int64_t i = 0; i < s; ++i)
    {
        for(int64_t j = 0; j < s; ++j)
        {
            // START TIME-STAMP
            auto _start = std::chrono::steady_clock::now();
            for(int64_t k = 0; k < s; ++k)
            {
                a[i * s + j] += b[i * s + k] * c[k * s + j];
            }
            // COMPUTE ELAPSED AND TRACK COUNT
            nseconds += (std::chrono::steady_clock::now() - _start).count();
            ++ncount;
        }
    }
}

double
execute_matmul(int64_t s, int64_t imax, int64_t nitr)
{
    // ...

    std::cout << ">>> MM average: " << (nseconds / ncount) << " ns [laps: " << ncount
              << "]" << std::endl;

    return sum;
}
```

Benchmark this runtime with:

```console
$ timem -o using_raw -- example-01

Running 50 MM on 250 x 250
>>> MM average: 281 ns [laps: 31250000]
MM sum = 32345.8

[example-01]> Outputting 'using_raw.json'...
[example-01]> Measurement totals:
           10.247416 sec wall
            9.890000 sec user
            0.040000 sec sys
            9.930000 sec cpu
           96.901335 % cpu_util
           11.288576 MB peak_rss
                   0 major_page_flts
                3012 minor_page_flts
                 746 prio_cxt_swch
                   3 vol_cxt_swch
```

You can verify that timemory can achieve almost the identical amount of minimal overhead by substituting:

```cpp
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

double
execute_matmul(int64_t s, int64_t imax, int64_t nitr)
{
    // ...

    std::cout << ntimer / ntimer.laps() << std::endl;

    return sum;
}
```

Benchmark this runtime with:

```console
$ timem -o using_timemory -- example-01

Running 50 MM on 250 x 250
>>>  MM average :  293.000 nsec wall [laps: 31250000]
MM sum = 32345.8

[example-01]> Outputting 'using_timemory.json'...
[example-01]> Measurement totals:
           10.836328 sec wall
           10.730000 sec user
            0.060000 sec sys
           10.790000 sec cpu
           99.570943 % cpu_util
           11.296768 MB peak_rss
                   0 major_page_flts
                3012 minor_page_flts
                3824 prio_cxt_swch
                   1 vol_cxt_swch
```

## Minor Overhead, Major Benefits

The overhead of timemory in this situation is approximately 19 nanoseconds (`(10.8-10.2)/31250000`). The benefit is, naturally, that `lightweight_tuple<wall_clock>` can be trivially changed to use any other component (or include additional components), e.g. `lightweight_tuple<voluntary_context_switch>`.
