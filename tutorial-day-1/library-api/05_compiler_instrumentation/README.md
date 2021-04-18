# Compiler Instrumentation

Timemory offers an option of compiler instrumentation without modifying your application code. This example demonstrates the usage of timemory compiler instrumentation
([documentation](https://timemory.readthedocs.io/en/develop/tools/timemory-compiler-instrument/README.html)).

To take advantage of compiler instrumentation you will only need to rebuild your application with timemory compiler instrumentation component and enable the instrumentation using environmental controls.

## Starting Code

[example.cpp](example.cpp) will be used to build the `compiler_instrumentation` executable. This
mini-app takes a value ("nfib") value, computes the fibonacci of this value ("nwait"),
and then consumes "nwait" milliseconds of CPU time +/- some randomness. Each iteration
is executed in parallel on a separate thread. In the compiler instrumentation, you
will find all the functions defined below.

```cpp
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <random>

inline long
fibonacci(long n)
{
    return (n < 2) ? n : (fibonacci(n - 1) + fibonacci(n - 2));
}

inline void
consume(long n)
{
    using mutex_t = std::mutex;
    using lock_t  = std::unique_lock<mutex_t>;
    // get current time
    auto now = std::chrono::steady_clock::now();
    // a mutex held by one lock
    mutex_t mutex{};
    // acquire lock
    lock_t hold_lk{ mutex };
    // defer trying to lock the same mutex
    lock_t try_lk{ mutex, std::defer_lock };
    // keep trying to lock until time point is reach (lock will never succeed)
    while(std::chrono::steady_clock::now() < (now + std::chrono::milliseconds{ n }))
        try_lk.try_lock();
}

int
main(int argc, char** argv)
{
    // input arguments
    long nfib  = (argc > 1) ? atol(argv[1]) : 15;
    int  nitr  = (argc > 2) ? atoi(argv[2]) : 10;
    long range = (argc > 3) ? atol(argv[3]) : 50;
    auto fwait = fibonacci(nfib);
    printf("running %i iteration of consume(%lu +/- %lu)...\n", nitr, fwait, range);

    auto rng     = std::default_random_engine{};
    auto dist    = std::uniform_int_distribution<long>{ -range, range };
    auto threads = std::vector<std::thread>{};

    threads.reserve(nitr);
    for(int i = 0; i < nitr; ++i)
    {
        auto nwait = fwait + dist(rng);
        threads.emplace_back(consume, nwait);
    }

    // join threads
    for(auto& itr : threads)
    {
        itr.join();
    }

    puts("done");
    return 0;
}
```

## Exercise

### Build with Compiler Instrumentation

modify the `CMakeLists.txt` so that the code in [example.cpp](example.cpp) can be rebuilt with timemory compiler instrumentation

```cmake
find_package(timemory REQUIRED COMPONENTS compiler-instrument)

# ... etc.

target_link_libraries(compiler_instrumentation PRIVATE timemory::timemory-compiler-instrument)
```

Compiler instrumentation settings can be controlled using environmental variables. For instance to turn compiler instrumentation on and for collecting `peak_rss` and `thread_cpu_clock` components use:

### 2. Modify Compiler Instrumentation Settings

```console
export TIMEMORY_COMPILER_COMPONENTS="peak_rss, thread_cpu_clock"
export TIMEMORY_COMPILER_ENABLE=true
```

You can build and run the code using:

```console
cmake -B build .
cmake --build build --target all
cd build
./compiler_instrumentation 12
```

Expected output:

```console
./compiler_instrumentation 12
[17588]> timemory-compiler-instrument will close after 'main' returns
running 10 iteration of consume(144 +/- 50)...
[pid=17588][tid=1]> timemory-compiler-instrument: 120 results
[pid=17588][tid=3]> timemory-compiler-instrument: 118 results
[pid=17588][tid=4]> timemory-compiler-instrument: 118 results
[pid=17588][tid=2]> timemory-compiler-instrument: 118 results
[pid=17588][tid=8]> timemory-compiler-instrument: 118 results
[pid=17588][tid=6]> timemory-compiler-instrument: 118 results
[pid=17588][tid=5]> timemory-compiler-instrument: 118 results
[pid=17588][tid=10]> timemory-compiler-instrument: 118 results
[pid=17588][tid=7]> timemory-compiler-instrument: 118 results
[pid=17588][tid=9]> timemory-compiler-instrument: 118 results
done
[pid=17588][tid=0]> timemory-compiler-instrument: 1484 results
[thread_cpu]|0> Outputting 'timemory-compiler-instrumentation-output/thread_cpu.flamegraph.json'...
[thread_cpu]|0> Outputting 'timemory-compiler-instrumentation-output/thread_cpu.json'...
[thread_cpu]|0> Outputting 'timemory-compiler-instrumentation-output/thread_cpu.tree.json'...
[thread_cpu]|0> Outputting 'timemory-compiler-instrumentation-output/thread_cpu.txt'...

[peak_rss]|0> Outputting 'timemory-compiler-instrumentation-output/peak_rss.json'...
[peak_rss]|0> Outputting 'timemory-compiler-instrumentation-output/peak_rss.tree.json'...
[peak_rss]|0> Outputting 'timemory-compiler-instrumentation-output/peak_rss.txt'...

[pid=17588][tid=0]> timemory-compiler-instrument: finalization required 0.0522458 sec and 2.84 MB
```

Timemory instrumentation output can be found in `timemory-compiler-instrumentation-output` directory.
