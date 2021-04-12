# Compiler Instrumentation

These examples demonstrate the usage of timemory compiler instrumentation
([documentation](https://timemory.readthedocs.io/en/develop/tools/timemory-compiler-instrument/README.html)).

## Build

```console
cmake -B build -DCMAKE_BUILD_TYPE=Release .
cmake --build build --target all
cd build
./compiler_instrumentation
```

## Starting Code

`example.cpp` will be used to build the `compiler_instrumentation` executable. This
mini-app takes a value ("nfib") value, computes the fibonacci of this value ("nwait"),
and then consumes "nwait" milliseconds of CPU time +/- some randomness. Each iteration
is executed in parallel on a separate thread. In the compiler instrumentation, you
will find all the functions defined below along with all the C++ standard library
template instantiations.

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

### 1. Build with Compiler Instrumentation

```cmake
find_package(timemory REQUIRED COMPONENTS compiler-instrument)

# ... etc.

target_link_libraries(compiler_instrumentation PRIVATE timemory::timemory-compiler-instrument)
```

### 2. Modify Compiler Instrumentation Settings

```console
export TIMEMORY_COMPILER_COMPONENTS="wall_clock, thread_cpu_clock"
export TIMEMORY_COMPILER_COLLAPSE_THREADS=OFF
```

### 3. Add Timemory Instrumentation to Target Code

```cmake
# add "c" to components to find the C compatible library
find_package(timemory REQUIRED COMPONENTS compiler-instrument c)

# preprocessor definition
target_compile_definitions(compiler_instrumentation_with_timemory PRIVATE USE_TIMEMORY)

# link to the main timemory target
target_link_libraries(compiler_instrumentation_with_timemory PRIVATE timemory::timemory)
```

```cpp
// include library header
#if defined(USE_TIMEMORY)
#    include <timemory/library.h>
#endif

// HINT: wrap all timemory-based code with this macro, e.g.
//      func() -> TIMEMORY_CODE(func())
#if !defined(TIMEMORY_CODE)
#    define TIMEMORY_CODE(...)
#endif
```

Place all these bits of code into `example.cpp`:

```cpp
// belongs in consume
char     _label[512];
uint64_t _idx = 0;
snprintf(_label, 512, "%s(%li)", __FUNCTION__, n);
timemory_begin_record(_label, &_idx);
timemory_end_record(_idx);

// belongs in main
timemory_set_default("wall_clock, thread_cpu_clock");
timemory_init_library(argc, argv);
timemory_finalize_library();
timemory_push_region("main");
timemory_pop_region("main");

// belongs in loops in main
auto record = timemory_scoped_record{ "launch_consume" };
auto record = timemory_scoped_record{ "wait_for_consume" };
```

### 4. Add Timemory Template API Instrumentation to Target Code (Advanced)

```cmake
# add "cxx" to components to find the C++ library
find_package(timemory REQUIRED COMPONENTS compiler-instrument cxx)

# preprocessor definition
target_compile_definitions(compiler_instrumentation_with_timemory PRIVATE USE_TIMEMORY)

# link to the main timemory target
target_link_libraries(compiler_instrumentation_with_timemory PRIVATE timemory::timemory)
```

Replace:

```cpp
uint64_t _idx = 0;
timemory_begin_record(_label, &_idx);
timemory_end_record(_idx);
```

with:

```cpp
#include <timemory/timemory.hpp>

using bundle_t =
    tim::auto_tuple<tim::component::wall_clock, tim::component::thread_cpu_clock>;

bundle_t _bundle{ _label };
```

> NOTE: Inspect the output: timemory's compiler instrumentation will fully instrument the timemory template API

## Example Output

### Executable Output without Timemory

```console
$ ./compiler_instrumentation
[90513]> timemory-compiler-instrument will close after 'main' returns
running 10 iteration of consume(610 +/- 50)...
[pid=90513][tid=1]> timemory-compiler-instrument: 60 results
[pid=90513][tid=7]> timemory-compiler-instrument: 59 results
[pid=90513][tid=2]> timemory-compiler-instrument: 59 results
[pid=90513][tid=6]> timemory-compiler-instrument: 59 results
[pid=90513][tid=4]> timemory-compiler-instrument: 59 results
[pid=90513][tid=5]> timemory-compiler-instrument: 59 results
[pid=90513][tid=8]> timemory-compiler-instrument: 59 results
[pid=90513][tid=9]> timemory-compiler-instrument: 59 results
[pid=90513][tid=3]> timemory-compiler-instrument: 60 results
[pid=90513][tid=10]> timemory-compiler-instrument: 59 results
done
[pid=90513][tid=0]> timemory-compiler-instrument: 741 results
[wall]|0> Outputting 'timemory-compiler-instrumentation-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-compiler-instrumentation-output/wall.json'...
[wall]|0> Outputting 'timemory-compiler-instrumentation-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-compiler-instrumentation-output/wall.txt'...

[pid=90513][tid=0]> timemory-compiler-instrument: finalization required 0.049933 sec and 2.85491 MB
```

### Executable Output with Timemory

```console
$ export TIMEMORY_COLLAPSE_THREADS=OFF
$ export TIMEMORY_COMPILER_COLLAPSE_THREADS=OFF
$ ./compiler_instrumentation_with_timemory
[3119]> timemory-compiler-instrument will close after 'main' returns
running 10 iteration of consume(610 +/- 50)...
[pid=3119][tid=1]> timemory-compiler-instrument: 61 results
[pid=3119][tid=7]> timemory-compiler-instrument: 59 results
[pid=3119][tid=2]> timemory-compiler-instrument: 61 results
[pid=3119][tid=6]> timemory-compiler-instrument: 59 results
[pid=3119][tid=4]> timemory-compiler-instrument: 59 results
[pid=3119][tid=5]> timemory-compiler-instrument: 59 results
[pid=3119][tid=8]> timemory-compiler-instrument: 59 results
[pid=3119][tid=9]> timemory-compiler-instrument: 59 results
[pid=3119][tid=3]> timemory-compiler-instrument: 61 results
[pid=3119][tid=10]> timemory-compiler-instrument: 59 results
[thread_cpu]|0> Outputting 'timemory-compiler-instrumentation-with-timemory-output/thread_cpu.flamegraph.json'...
[thread_cpu]|0> Outputting 'timemory-compiler-instrumentation-with-timemory-output/thread_cpu.json'...
[thread_cpu]|0> Outputting 'timemory-compiler-instrumentation-with-timemory-output/thread_cpu.tree.json'...
[thread_cpu]|0> Outputting 'timemory-compiler-instrumentation-with-timemory-output/thread_cpu.txt'...

|-------------------------------------------------------------------------------------------------------------------------|
|                                          CPU-CLOCK TIMER FOR THE CALLING THREAD                                         |
|-------------------------------------------------------------------------------------------------------------------------|
|          LABEL            | COUNT  | DEPTH  |   METRIC   | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|---------------------------|--------|--------|------------|--------|--------|--------|--------|--------|--------|--------|
| |00>>> main               |      1 |      0 | thread_cpu | sec    |  0.007 |  0.007 |  0.007 |  0.007 |  0.000 |   63.0 |
| |00>>> |_launch_consume   |     10 |      1 | thread_cpu | sec    |  0.002 |  0.000 |  0.000 |  0.001 |  0.000 |    0.0 |
| |01>>>   |_consume(560)   |      1 |      2 | thread_cpu | sec    |  0.557 |  0.557 |  0.557 |  0.557 |  0.000 |  100.0 |
| |02>>>   |_consume(573)   |      1 |      2 | thread_cpu | sec    |  0.569 |  0.569 |  0.569 |  0.569 |  0.000 |  100.0 |
| |00>>> |_wait_for_consume |     10 |      1 | thread_cpu | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| |06>>>   |_consume(564)   |      1 |      2 | thread_cpu | sec    |  0.560 |  0.560 |  0.560 |  0.560 |  0.000 |  100.0 |
| |08>>>   |_consume(582)   |      1 |      2 | thread_cpu | sec    |  0.579 |  0.579 |  0.579 |  0.579 |  0.000 |  100.0 |
| |05>>>   |_consume(606)   |      1 |      2 | thread_cpu | sec    |  0.601 |  0.601 |  0.601 |  0.601 |  0.000 |  100.0 |
| |04>>>   |_consume(613)   |      1 |      2 | thread_cpu | sec    |  0.609 |  0.609 |  0.609 |  0.609 |  0.000 |  100.0 |
| |07>>>   |_consume(628)   |      1 |      2 | thread_cpu | sec    |  0.621 |  0.621 |  0.621 |  0.621 |  0.000 |  100.0 |
| |09>>>   |_consume(628)   |      1 |      2 | thread_cpu | sec    |  0.623 |  0.623 |  0.623 |  0.623 |  0.000 |  100.0 |
| |03>>>   |_consume(636)   |      1 |      2 | thread_cpu | sec    |  0.630 |  0.630 |  0.630 |  0.630 |  0.000 |  100.0 |
| |10>>>   |_consume(654)   |      1 |      2 | thread_cpu | sec    |  0.649 |  0.649 |  0.649 |  0.649 |  0.000 |  100.0 |
|-------------------------------------------------------------------------------------------------------------------------|

[wall]|0> Outputting 'timemory-compiler-instrumentation-with-timemory-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-compiler-instrumentation-with-timemory-output/wall.json'...
[wall]|0> Outputting 'timemory-compiler-instrumentation-with-timemory-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-compiler-instrumentation-with-timemory-output/wall.txt'...

|---------------------------------------------------------------------------------------------------------------------|
|                                       REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                      |
|---------------------------------------------------------------------------------------------------------------------|
|          LABEL            | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|---------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| |00>>> main               |      1 |      0 | wall   | sec    |  0.663 |  0.663 |  0.663 |  0.663 |  0.000 |    0.7 |
| |00>>> |_launch_consume   |     10 |      1 | wall   | sec    |  0.002 |  0.000 |  0.000 |  0.001 |  0.000 |    0.0 |
| |01>>>   |_consume(560)   |      1 |      2 | wall   | sec    |  0.560 |  0.560 |  0.560 |  0.560 |  0.000 |  100.0 |
| |02>>>   |_consume(573)   |      1 |      2 | wall   | sec    |  0.573 |  0.573 |  0.573 |  0.573 |  0.000 |  100.0 |
| |00>>> |_wait_for_consume |     10 |      1 | wall   | sec    |  0.656 |  0.066 |  0.000 |  0.561 |  0.175 |    0.0 |
| |06>>>   |_consume(564)   |      1 |      2 | wall   | sec    |  0.564 |  0.564 |  0.564 |  0.564 |  0.000 |  100.0 |
| |08>>>   |_consume(582)   |      1 |      2 | wall   | sec    |  0.582 |  0.582 |  0.582 |  0.582 |  0.000 |  100.0 |
| |05>>>   |_consume(606)   |      1 |      2 | wall   | sec    |  0.606 |  0.606 |  0.606 |  0.606 |  0.000 |  100.0 |
| |04>>>   |_consume(613)   |      1 |      2 | wall   | sec    |  0.613 |  0.613 |  0.613 |  0.613 |  0.000 |  100.0 |
| |07>>>   |_consume(628)   |      1 |      2 | wall   | sec    |  0.628 |  0.628 |  0.628 |  0.628 |  0.000 |  100.0 |
| |09>>>   |_consume(628)   |      1 |      2 | wall   | sec    |  0.628 |  0.628 |  0.628 |  0.628 |  0.000 |  100.0 |
| |03>>>   |_consume(636)   |      1 |      2 | wall   | sec    |  0.636 |  0.636 |  0.636 |  0.636 |  0.000 |  100.0 |
| |10>>>   |_consume(654)   |      1 |      2 | wall   | sec    |  0.654 |  0.654 |  0.654 |  0.654 |  0.000 |  100.0 |
|---------------------------------------------------------------------------------------------------------------------|

done
[pid=3119][tid=0]> timemory-compiler-instrument: 749 results
[wall]|0> Outputting 'timemory-compiler-instrumentation-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-compiler-instrumentation-output/wall.json'...
[wall]|0> Outputting 'timemory-compiler-instrumentation-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-compiler-instrumentation-output/wall.txt'...

[pid=3119][tid=0]> timemory-compiler-instrument: finalization required 0.069543 sec and 3.53075 MB
```

> NOTE: the built-in timemory instrumentation includes

### Wall-Clock Output

```console
$ cat timemory-compiler-instrumentation-output/wall.txt
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                                                      REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                                                                      |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                          LABEL                                                           | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|--------------------------------------------------------------------------------------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> [_GLOBAL__sub_I_example.cpp][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler_... |      1 |      0 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   89.6 |
| >>> |_[__static_initialization_and_destruction_0(int, int)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day... |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> [main][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler_instrumentation/soluti... |      1 |      0 | wall   | sec    |  0.664 |  0.664 |  0.664 |  0.664 |  0.000 |    0.0 |
| >>> |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler_instrumen... |      1 |      1 | wall   | sec    |  0.003 |  0.003 |  0.003 |  0.003 |  0.000 |    0.3 |
| >>>   |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler_instrum... |      2 |      2 | wall   | sec    |  0.003 |  0.002 |  0.001 |  0.002 |  0.001 |    0.2 |
| >>>     |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler_instr... |      4 |      3 | wall   | sec    |  0.003 |  0.001 |  0.000 |  0.001 |  0.000 |    0.5 |
| >>>       |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler_ins... |      8 |      4 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.001 |  0.000 |    0.8 |
| >>>         |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler_i... |     16 |      5 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.001 |  0.000 |    1.6 |
| >>>           |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler... |     32 |      6 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |    2.8 |
| >>>             |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compil... |     64 |      7 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |    6.5 |
| >>>               |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_comp... |    128 |      8 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |   13.1 |
| >>>                 |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_co... |    254 |      9 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |   27.4 |
| >>>                   |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_... |    438 |     10 | wall   | sec    |  0.002 |  0.000 |  0.000 |  0.000 |  0.000 |   43.5 |
| >>>                     |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/0... |    512 |     11 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   65.1 |
| >>>                       |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api... |    352 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   78.0 |
| >>>                         |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-a... |    134 |     13 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   78.0 |
| >>>                           |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library... |     26 |     14 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   94.4 |
| >>>                             |_[fibonacci(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/libra... |      2 |     15 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[std::linear_congruential_engine<unsigned int, 16807u, 0u, 2147483647u>::linear_congruential_engine()][/Users/j... |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   22.9 |
| >>>   |_[std::linear_congruential_engine<unsigned int, 16807u, 0u, 2147483647u>::linear_congruential_engine(unsigned ... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   25.9 |
| >>>     |_[std::linear_congruential_engine<unsigned int, 16807u, 0u, 2147483647u>::seed(unsigned int)][/Users/jrmadse... |      1 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   50.0 |
| >>>       |_[unsigned int std::__detail::__mod<unsigned int, 2147483647u, 1u, 0u>(unsigned int)][/Users/jrmadsen/deve... |      3 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   80.0 |
| >>>         |_[std::__detail::_Mod<unsigned int, 2147483647u, 1u, 0u, true, true>::__calc(unsigned int)][/Users/jrmad... |      3 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[std::uniform_int_distribution<long>::uniform_int_distribution(long, long)][/Users/jrmadsen/devel/c++/timemory-... |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   85.7 |
| >>>   |_[std::uniform_int_distribution<long>::param_type::param_type(long, long)][/Users/jrmadsen/devel/c++/timemory-... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[std::vector<std::thread, std::allocator<std::thread> >::vector()][/Users/jrmadsen/devel/c++/timemory-tutorials... |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   20.6 |
| >>>   |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_Vector_base()][/Users/jrmadsen/devel/c++/time... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   22.2 |
| >>>     |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_Vector_impl::_Vector_impl()][/Users/jrmadse... |      1 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   57.1 |
| >>>       |_[std::allocator<std::thread>::allocator()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/li... |      1 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   87.5 |
| >>>         |_[__gnu_cxx::new_allocator<std::thread>::new_allocator()][/Users/jrmadsen/devel/c++/timemory-tutorials/t... |      1 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>       |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_Vector_impl_data::_Vector_impl_data()][/U... |      1 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[std::vector<std::thread, std::allocator<std::thread> >::reserve(unsigned long)][/Users/jrmadsen/devel/c++/time... |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   35.2 |
| >>>   |_[std::vector<std::thread, std::allocator<std::thread> >::max_size() const][/Users/jrmadsen/devel/c++/timemory... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   46.9 |
| >>>     |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_M_get_Tp_allocator() const][/Users/jrmadsen... |      1 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     |_[std::vector<std::thread, std::allocator<std::thread> >::_S_max_size(std::allocator<std::thread> const&)][/... |      1 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   58.8 |
| >>>       |_[std::allocator_traits<std::allocator<std::thread> >::max_size(std::allocator<std::thread> const&)][/User... |      1 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[__gnu_cxx::new_allocator<std::thread>::max_size() const][/Users/jrmadsen/devel/c++/timemory-tutorials/... |      1 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>       |_[unsigned long const& std::min<unsigned long>(unsigned long const&, unsigned long const&)][/Users/jrmadse... |      1 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>   |_[std::vector<std::thread, std::allocator<std::thread> >::capacity() const][/Users/jrmadsen/devel/c++/timemory... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_[std::vector<std::thread, std::allocator<std::thread> >::size() const][/Users/jrmadsen/devel/c++/timemory-tut... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_M_allocate(unsigned long)][/Users/jrmadsen/de... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   43.8 |
| >>>     |_[std::allocator_traits<std::allocator<std::thread> >::allocate(std::allocator<std::thread>&, unsigned long)... |      1 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   66.7 |
| >>>       |_[__gnu_cxx::new_allocator<std::thread>::allocate(unsigned long, void const*)][/Users/jrmadsen/devel/c++/t... |      1 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[__gnu_cxx::new_allocator<std::thread>::max_size() const][/Users/jrmadsen/devel/c++/timemory-tutorials/... |      1 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_M_get_Tp_allocator()][/Users/jrmadsen/devel/c... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>   |_[std::vector<std::thread, std::allocator<std::thread> >::_S_relocate(std::thread*, std::thread*, std::thread*... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   26.5 |
| >>>     |_[std::vector<std::thread, std::allocator<std::thread> >::_S_do_relocate(std::thread*, std::thread*, std::th... |      1 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   24.0 |
| >>>       |_[std::thread* std::__relocate_a<std::thread*, std::thread*, std::allocator<std::thread> >(std::thread*, s... |      1 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   73.7 |
| >>>         |_[std::thread* std::__niter_base<std::thread*>(std::thread*)][/Users/jrmadsen/devel/c++/timemory-tutoria... |      3 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[std::thread* std::__relocate_a_1<std::thread*, std::thread*, std::allocator<std::thread> >(std::thread... |      1 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>   |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_M_deallocate(std::thread*, unsigned long)][/U... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_[long std::uniform_int_distribution<long>::operator()<std::linear_congruential_engine<unsigned int, 16807u, 0u,... |     10 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   31.4 |
| >>>   |_[long std::uniform_int_distribution<long>::operator()<std::linear_congruential_engine<unsigned int, 16807u, 0... |     10 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   58.1 |
| >>>     |_[std::uniform_int_distribution<long>::param_type::b() const][/Users/jrmadsen/devel/c++/timemory-tutorials/t... |     10 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>     |_[std::uniform_int_distribution<long>::param_type::a() const][/Users/jrmadsen/devel/c++/timemory-tutorials/t... |     20 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>     |_[std::linear_congruential_engine<unsigned int, 16807u, 0u, 2147483647u>::operator()()][/Users/jrmadsen/deve... |     10 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   48.4 |
| >>>       |_[unsigned int std::__detail::__mod<unsigned int, 2147483647u, 16807u, 0u>(unsigned int)][/Users/jrmadsen/... |     10 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   90.6 |
| >>>         |_[std::__detail::_Mod<unsigned int, 2147483647u, 16807u, 0u, false, true>::__calc(unsigned int)][/Users/... |     10 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[void std::vector<std::thread, std::allocator<std::thread> >::emplace_back<void (&)(long), long&>(void (&)(long... |     10 |      1 | wall   | sec    |  0.002 |  0.000 |  0.000 |  0.000 |  0.000 |    4.1 |
| >>>   |_[long& std::forward<long&>(std::remove_reference<long&>::type&)][/Users/jrmadsen/devel/c++/timemory-tutorials... |     10 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>   |_[void (&std::forward<void (&)(long)>(std::remove_reference<void (&)(long)>::type&))(long)][/Users/jrmadsen/de... |     10 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>   |_[void std::allocator_traits<std::allocator<std::thread> >::construct<std::thread, void (&)(long), long&>(std:... |     10 |      2 | wall   | sec    |  0.002 |  0.000 |  0.000 |  0.000 |  0.000 |    3.8 |
| >>>     |_[long& std::forward<long&>(std::remove_reference<long&>::type&)][/Users/jrmadsen/devel/c++/timemory-tutoria... |     10 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>     |_[void (&std::forward<void (&)(long)>(std::remove_reference<void (&)(long)>::type&))(long)][/Users/jrmadsen/... |     10 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>     |_[void __gnu_cxx::new_allocator<std::thread>::construct<std::thread, void (&)(long), long&>(std::thread*, vo... |     10 |      3 | wall   | sec    |  0.002 |  0.000 |  0.000 |  0.000 |  0.000 |    5.1 |
| >>>       |_[void (&std::forward<void (&)(long)>(std::remove_reference<void (&)(long)>::type&))(long)][/Users/jrmadse... |     10 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>       |_[long& std::forward<long&>(std::remove_reference<long&>::type&)][/Users/jrmadsen/devel/c++/timemory-tutor... |     10 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>       |_[operator new(unsigned long, void*)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-... |     10 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>       |_[std::thread::thread<void (&)(long), long&, void>(void (&)(long), long&)][/Users/jrmadsen/devel/c++/timem... |     10 |      4 | wall   | sec    |  0.002 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>         |_[std::thread::id::id()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_comp... |     10 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[long& std::forward<long&>(std::remove_reference<long&>::type&)][/Users/jrmadsen/devel/c++/timemory-tut... |     10 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[void (&std::forward<void (&)(long)>(std::remove_reference<void (&)(long)>::type&))(long)][/Users/jrmad... |     10 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[std::thread::_Invoker<std::tuple<std::decay<void (&)(long)>::type, std::decay<long&>::type> > std::thr... |     10 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   23.2 |
| >>>           |_[void (&std::forward<void (&)(long)>(std::remove_reference<void (&)(long)>::type&))(long)][/Users/jrm... |     10 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>           |_[long& std::forward<long&>(std::remove_reference<long&>::type&)][/Users/jrmadsen/devel/c++/timemory-t... |     10 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>           |_[std::tuple<void (*)(long), long>::tuple<void (&)(long), long&, true>(void (&)(long), long&)][/Users/... |     10 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   27.5 |
| >>>             |_[long& std::forward<long&>(std::remove_reference<long&>::type&)][/Users/jrmadsen/devel/c++/timemory... |     10 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[void (&std::forward<void (&)(long)>(std::remove_reference<void (&)(long)>::type&))(long)][/Users/j... |     10 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[std::_Tuple_impl<0ul, void (*)(long), long>::_Tuple_impl<void (&)(long), long&, void>(void (&)(lon... |     10 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   48.6 |
| >>>               |_[long& std::forward<long&>(std::remove_reference<long&>::type&)][/Users/jrmadsen/devel/c++/timemo... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::_Tuple_impl<1ul, long>::_Tuple_impl<long&>(long&)][/Users/jrmadsen/devel/c++/timemory-tutor... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   55.0 |
| >>>                 |_[long& std::forward<long&>(std::remove_reference<long&>::type&)][/Users/jrmadsen/devel/c++/time... |     10 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[std::_Head_base<1ul, long, false>::_Head_base<long&>(long&)][/Users/jrmadsen/devel/c++/timemor... |     10 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   86.2 |
| >>>                   |_[long& std::forward<long&>(std::remove_reference<long&>::type&)][/Users/jrmadsen/devel/c++/ti... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[void (&std::forward<void (&)(long)>(std::remove_reference<void (&)(long)>::type&))(long)][/Users... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::_Head_base<0ul, void (*)(long), false>::_Head_base(void (* const&)(long))][/Users/jrmadsen/... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> > std::thread::_S_make_s... |     10 |      5 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   20.9 |
| >>>           |_[std::thread::_Invoker<std::tuple<void (*)(long), long> >&& std::forward<std::thread::_Invoker<std::t... |     10 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>           |_[std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)(long), long> > >::_State_impl(std... |     10 |      6 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   16.3 |
| >>>             |_[std::thread::_State::_State()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library... |     10 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[std::thread::_Invoker<std::tuple<void (*)(long), long> >&& std::forward<std::thread::_Invoker<std:... |     10 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[std::thread::_Invoker<std::tuple<void (*)(long), long> >::_Invoker(std::thread::_Invoker<std::tupl... |     10 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    6.5 |
| >>>               |_[std::tuple<void (*)(long), long>::tuple(std::tuple<void (*)(long), long>&&)][/Users/jrmadsen/dev... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    6.4 |
| >>>                 |_[std::_Tuple_impl<0ul, void (*)(long), long>::_Tuple_impl(std::_Tuple_impl<0ul, void (*)(long),... |     10 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   42.3 |
| >>>                   |_[std::_Tuple_impl<0ul, void (*)(long), long>::_M_tail(std::_Tuple_impl<0ul, void (*)(long), l... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                   |_[std::remove_reference<std::_Tuple_impl<1ul, long>&>::type&& std::move<std::_Tuple_impl<1ul, ... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                   |_[std::_Tuple_impl<1ul, long>::_Tuple_impl(std::_Tuple_impl<1ul, long>&&)][/Users/jrmadsen/dev... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   58.7 |
| >>>                     |_[std::_Tuple_impl<1ul, long>::_M_head(std::_Tuple_impl<1ul, long>&)][/Users/jrmadsen/devel/... |     10 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   82.8 |
| >>>                       |_[std::_Head_base<1ul, long, false>::_M_head(std::_Head_base<1ul, long, false>&)][/Users/j... |     10 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                     |_[long&& std::forward<long>(std::remove_reference<long>::type&)][/Users/jrmadsen/devel/c++/t... |     10 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                     |_[std::_Head_base<1ul, long, false>::_Head_base<long>(long&&)][/Users/jrmadsen/devel/c++/tim... |     10 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   80.8 |
| >>>                       |_[long&& std::forward<long>(std::remove_reference<long>::type&)][/Users/jrmadsen/devel/c++... |     10 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                   |_[std::_Tuple_impl<0ul, void (*)(long), long>::_M_head(std::_Tuple_impl<0ul, void (*)(long), l... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   85.7 |
| >>>                     |_[std::_Head_base<0ul, void (*)(long), false>::_M_head(std::_Head_base<0ul, void (*)(long), ... |     10 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                   |_[void (*&&std::forward<void (*)(long)>(std::remove_reference<void (*)(long)>::type&))(long)][... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                   |_[std::_Head_base<0ul, void (*)(long), false>::_Head_base<void (*)(long)>(void (*&&)(long))][/... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   96.2 |
| >>>                     |_[void (*&&std::forward<void (*)(long)>(std::remove_reference<void (*)(long)>::type&))(long)... |     10 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>           |_[std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >::unique_ptr<std::def... |     10 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    8.1 |
| >>>             |_[std::__uniq_ptr_impl<std::thread::_State, std::default_delete<std::thread::_State> >::__uniq_ptr_i... |     10 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   18.8 |
| >>>               |_[std::tuple<std::thread::_State*, std::default_delete<std::thread::_State> >::tuple<std::thread::... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   23.6 |
| >>>                 |_[std::_Tuple_impl<0ul, std::thread::_State*, std::default_delete<std::thread::_State> >::_Tuple... |     10 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   60.0 |
| >>>                   |_[std::_Tuple_impl<1ul, std::default_delete<std::thread::_State> >::_Tuple_impl()][/Users/jrma... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   92.5 |
| >>>                     |_[std::_Head_base<1ul, std::default_delete<std::thread::_State>, true>::_Head_base()][/Users... |     10 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                   |_[std::_Head_base<0ul, std::thread::_State*, false>::_Head_base()][/Users/jrmadsen/devel/c++/t... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::__uniq_ptr_impl<std::thread::_State, std::default_delete<std::thread::_State> >::_M_ptr()][... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   25.2 |
| >>>                 |_[std::tuple_element<0ul, std::tuple<std::thread::_State*, std::default_delete<std::thread::_Sta... |     10 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   30.4 |
| >>>                   |_[std::thread::_State*& std::__get_helper<0ul, std::thread::_State*, std::default_delete<std::... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   48.4 |
| >>>                     |_[std::_Tuple_impl<0ul, std::thread::_State*, std::default_delete<std::thread::_State> >::_M... |     10 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   84.8 |
| >>>                       |_[std::_Head_base<0ul, std::thread::_State*, false>::_M_head(std::_Head_base<0ul, std::thr... |     10 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >::~unique_ptr()][/Users... |     10 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   19.4 |
| >>>           |_[std::__uniq_ptr_impl<std::thread::_State, std::default_delete<std::thread::_State> >::_M_ptr()][/Use... |     10 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   23.2 |
| >>>             |_[std::tuple_element<0ul, std::tuple<std::thread::_State*, std::default_delete<std::thread::_State> ... |     10 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   28.1 |
| >>>               |_[std::thread::_State*& std::__get_helper<0ul, std::thread::_State*, std::default_delete<std::thre... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   44.9 |
| >>>                 |_[std::_Tuple_impl<0ul, std::thread::_State*, std::default_delete<std::thread::_State> >::_M_hea... |     10 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   78.9 |
| >>>                   |_[std::_Head_base<0ul, std::thread::_State*, false>::_M_head(std::_Head_base<0ul, std::thread:... |     10 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)(long), long> > >::_M_run()][/Users/... |      1 |      5 | wall   | sec    |  0.560 |  0.560 |  0.560 |  0.560 |  0.000 |    0.0 |
| >>>           |_[std::thread::_Invoker<std::tuple<void (*)(long), long> >::operator()()][/Users/jrmadsen/devel/c++/ti... |      1 |      6 | wall   | sec    |  0.560 |  0.560 |  0.560 |  0.560 |  0.000 |    0.0 |
| >>>             |_[void std::thread::_Invoker<std::tuple<void (*)(long), long> >::_M_invoke<0ul, 1ul>(std::_Index_tup... |      1 |      7 | wall   | sec    |  0.560 |  0.560 |  0.560 |  0.560 |  0.000 |    0.0 |
| >>>               |_[std::remove_reference<std::tuple<void (*)(long), long>&>::type&& std::move<std::tuple<void (*)(l... |      2 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::tuple_element<1ul, std::tuple<void (*)(long), long> >::type&& std::get<1ul, void (*)(long),... |      1 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   38.9 |
| >>>                 |_[std::tuple_element<1ul, std::tuple<void (*)(long), long> >::type& std::get<1ul, void (*)(long)... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   34.4 |
| >>>                   |_[long& std::__get_helper<1ul, long>(std::_Tuple_impl<1ul, long>&)][/Users/jrmadsen/devel/c++/... |      1 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   47.6 |
| >>>                     |_[std::_Tuple_impl<1ul, long>::_M_head(std::_Tuple_impl<1ul, long>&)][/Users/jrmadsen/devel/... |      1 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   90.9 |
| >>>                       |_[std::_Head_base<1ul, long, false>::_M_head(std::_Head_base<1ul, long, false>&)][/Users/j... |      1 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[long&& std::forward<long&&>(std::remove_reference<long&&>::type&)][/Users/jrmadsen/devel/c++/t... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::tuple_element<0ul, std::tuple<void (*)(long), long> >::type&& std::get<0ul, void (*)(long),... |      1 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   35.8 |
| >>>                 |_[std::tuple_element<0ul, std::tuple<void (*)(long), long> >::type& std::get<0ul, void (*)(long)... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   30.3 |
| >>>                   |_[void (*&std::__get_helper<0ul, void (*)(long), long>(std::_Tuple_impl<0ul, void (*)(long), l... |      1 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   47.8 |
| >>>                     |_[std::_Tuple_impl<0ul, void (*)(long), long>::_M_head(std::_Tuple_impl<0ul, void (*)(long),... |      1 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   91.7 |
| >>>                       |_[std::_Head_base<0ul, void (*)(long), false>::_M_head(std::_Head_base<0ul, void (*)(long)... |      1 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[void (*&&std::forward<void (*&&)(long)>(std::remove_reference<void (*&&)(long)>::type&))(long)... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::__invoke_result<void (*)(long), long>::type std::__invoke<void (*)(long), long>(void (*&&)(... |      1 |      8 | wall   | sec    |  0.560 |  0.560 |  0.560 |  0.560 |  0.000 |    0.0 |
| >>>                 |_[long&& std::forward<long>(std::remove_reference<long>::type&)][/Users/jrmadsen/devel/c++/timem... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[void (*&&std::forward<void (*)(long)>(std::remove_reference<void (*)(long)>::type&))(long)][/U... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                 |_[void std::__invoke_impl<void, void (*)(long), long>(std::__invoke_other, void (*&&)(long), lon... |      1 |      9 | wall   | sec    |  0.560 |  0.560 |  0.560 |  0.560 |  0.000 |    0.0 |
| >>>                   |_[void (*&&std::forward<void (*)(long)>(std::remove_reference<void (*)(long)>::type&))(long)][... |      1 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                   |_[long&& std::forward<long>(std::remove_reference<long>::type&)][/Users/jrmadsen/devel/c++/tim... |      1 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                   |_[consume(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_co... |      1 |     10 | wall   | sec    |  0.560 |  0.560 |  0.560 |  0.560 |  0.000 |   14.5 |
| >>>                     |_[std::unique_lock<std::mutex>::unique_lock(std::mutex&)][/Users/jrmadsen/devel/c++/timemory... |      1 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   24.1 |
| >>>                       |_[std::mutex* std::__addressof<std::mutex>(std::mutex&)][/Users/jrmadsen/devel/c++/timemor... |      1 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                       |_[std::unique_lock<std::mutex>::lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutor... |      1 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   18.6 |
| >>>                         |_[std::mutex::lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/librar... |      1 |     13 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   22.9 |
| >>>                           |_[__gthread_mutex_lock(_opaque_pthread_mutex_t*)][/Users/jrmadsen/devel/c++/timemory-t... |      1 |     14 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   97.3 |
| >>>                             |_[__gthread_active_p()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/... |      1 |     15 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                     |_[std::unique_lock<std::mutex>::unique_lock(std::mutex&, std::defer_lock_t)][/Users/jrmadsen... |      1 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                       |_[std::mutex* std::__addressof<std::mutex>(std::mutex&)][/Users/jrmadsen/devel/c++/timemor... |      1 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                     |_[std::chrono::duration<long long, std::ratio<1l, 1000l> >::duration<long, void>(long const&... |  10847 |     11 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                     |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::common_type<std::chrono::dura... |  10847 |     11 | wall   | sec    |  0.283 |  0.000 |  0.000 |  0.001 |  0.000 |   22.2 |
| >>>                       |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long long,... |  10847 |     12 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                       |_[std::common_type<std::chrono::duration<long long, std::ratio<1l, 1000000000l> >, std::ch... |  10847 |     12 | wall   | sec    |  0.214 |  0.000 |  0.000 |  0.001 |  0.000 |   38.7 |
| >>>                         |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::count() const][/Users/... |  21694 |     13 | wall   | sec    |  0.006 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                         |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::duration<long long, st... |  10847 |     13 | wall   | sec    |  0.122 |  0.000 |  0.000 |  0.001 |  0.000 |   36.6 |
| >>>                           |_[std::enable_if<std::chrono::__is_duration<std::chrono::duration<long long, std::rati... |  10847 |     14 | wall   | sec    |  0.075 |  0.000 |  0.000 |  0.001 |  0.000 |   32.4 |
| >>>                             |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> > std::chrono::__dura... |  10847 |     15 | wall   | sec    |  0.050 |  0.000 |  0.000 |  0.001 |  0.000 |   87.5 |
| >>>                               |_[std::chrono::duration<long long, std::ratio<1l, 1000l> >::count() const][/Users/... |  10847 |     16 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                               |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::duration<long lo... |  10847 |     16 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                           |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::count() const][/User... |  10847 |     14 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                         |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::duration<long long, vo... |  10847 |     13 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                       |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long long,... |  10847 |     12 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                     |_[bool std::chrono::operator< <std::chrono::_V2::steady_clock, std::chrono::duration<long lo... |  10847 |     11 | wall   | sec    |  0.118 |  0.000 |  0.000 |  0.000 |  0.000 |   53.6 |
| >>>                       |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long long,... |  21694 |     12 | wall   | sec    |  0.006 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                       |_[bool std::chrono::operator< <long long, std::ratio<1l, 1000000000l>, long long, std::rat... |  10847 |     12 | wall   | sec    |  0.049 |  0.000 |  0.000 |  0.000 |  0.000 |   87.5 |
| >>>                         |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::count() const][/Users/... |  21694 |     13 | wall   | sec    |  0.006 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                     |_[std::unique_lock<std::mutex>::try_lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tut... |  10846 |     11 | wall   | sec    |  0.074 |  0.000 |  0.000 |  0.000 |  0.000 |   30.4 |
| >>>                       |_[std::mutex::try_lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/libr... |  10846 |     12 | wall   | sec    |  0.052 |  0.000 |  0.000 |  0.000 |  0.000 |   45.0 |
| >>>                         |_[__gthread_mutex_trylock(_opaque_pthread_mutex_t*)][/Users/jrmadsen/devel/c++/timemory-... |  10846 |     13 | wall   | sec    |  0.028 |  0.000 |  0.000 |  0.000 |  0.000 |   85.7 |
| >>>                           |_[__gthread_active_p()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/li... |  10846 |     14 | wall   | sec    |  0.004 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                     |_[std::unique_lock<std::mutex>::~unique_lock()][/Users/jrmadsen/devel/c++/timemory-tutorials... |      2 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   19.2 |
| >>>                       |_[std::unique_lock<std::mutex>::unlock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tut... |      1 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   21.4 |
| >>>                         |_[std::mutex::unlock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/libr... |      1 |     13 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   30.3 |
| >>>                           |_[__gthread_mutex_unlock(_opaque_pthread_mutex_t*)][/Users/jrmadsen/devel/c++/timemory... |      1 |     14 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                             |_[__gthread_active_p()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/... |      1 |     15 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_[std::vector<std::thread, std::allocator<std::thread> >::begin()][/Users/jrmadsen/devel/c++/timemory-tutorials/... |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   93.3 |
| >>>   |_[__gnu_cxx::__normal_iterator<std::thread*, std::vector<std::thread, std::allocator<std::thread> > >::__norma... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[std::vector<std::thread, std::allocator<std::thread> >::end()][/Users/jrmadsen/devel/c++/timemory-tutorials/tu... |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   93.3 |
| >>>   |_[__gnu_cxx::__normal_iterator<std::thread*, std::vector<std::thread, std::allocator<std::thread> > >::__norma... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[bool __gnu_cxx::operator!=<std::thread*, std::vector<std::thread, std::allocator<std::thread> > >(__gnu_cxx::_... |     11 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_[__gnu_cxx::__normal_iterator<std::thread*, std::vector<std::thread, std::allocator<std::thread> > >::base() ... |     22 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>   |_[std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)(long), long> > >::_M_run()][/Users/jrmads... |      1 |      2 | wall   | sec    |  0.637 |  0.637 |  0.637 |  0.637 |  0.000 |    0.0 |
| >>>     |_[std::thread::_Invoker<std::tuple<void (*)(long), long> >::operator()()][/Users/jrmadsen/devel/c++/timemory... |      1 |      3 | wall   | sec    |  0.637 |  0.637 |  0.637 |  0.637 |  0.000 |    0.0 |
| >>>       |_[void std::thread::_Invoker<std::tuple<void (*)(long), long> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul... |      1 |      4 | wall   | sec    |  0.636 |  0.636 |  0.636 |  0.636 |  0.000 |    0.0 |
| >>>         |_[std::remove_reference<std::tuple<void (*)(long), long>&>::type&& std::move<std::tuple<void (*)(long), ... |      2 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[std::tuple_element<1ul, std::tuple<void (*)(long), long> >::type&& std::get<1ul, void (*)(long), long>... |      1 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   46.9 |
| >>>           |_[std::tuple_element<1ul, std::tuple<void (*)(long), long> >::type& std::get<1ul, void (*)(long), long... |      1 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   15.9 |
| >>>             |_[long& std::__get_helper<1ul, long>(std::_Tuple_impl<1ul, long>&)][/Users/jrmadsen/devel/c++/timemo... |      1 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   82.8 |
| >>>               |_[std::_Tuple_impl<1ul, long>::_M_head(std::_Tuple_impl<1ul, long>&)][/Users/jrmadsen/devel/c++/ti... |      1 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   90.0 |
| >>>                 |_[std::_Head_base<1ul, long, false>::_M_head(std::_Head_base<1ul, long, false>&)][/Users/jrmadse... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>           |_[long&& std::forward<long&&>(std::remove_reference<long&&>::type&)][/Users/jrmadsen/devel/c++/timemor... |      1 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>         |_[std::tuple_element<0ul, std::tuple<void (*)(long), long> >::type&& std::get<0ul, void (*)(long), long>... |      1 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   26.0 |
| >>>           |_[std::tuple_element<0ul, std::tuple<void (*)(long), long> >::type& std::get<0ul, void (*)(long), long... |      1 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   59.2 |
| >>>             |_[void (*&std::__get_helper<0ul, void (*)(long), long>(std::_Tuple_impl<0ul, void (*)(long), long>&)... |      1 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   58.1 |
| >>>               |_[std::_Tuple_impl<0ul, void (*)(long), long>::_M_head(std::_Tuple_impl<0ul, void (*)(long), long>... |      1 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   92.3 |
| >>>                 |_[std::_Head_base<0ul, void (*)(long), false>::_M_head(std::_Head_base<0ul, void (*)(long), fals... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>           |_[void (*&&std::forward<void (*&&)(long)>(std::remove_reference<void (*&&)(long)>::type&))(long)][/Use... |      1 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[std::__invoke_result<void (*)(long), long>::type std::__invoke<void (*)(long), long>(void (*&&)(long),... |      1 |      5 | wall   | sec    |  0.636 |  0.636 |  0.636 |  0.636 |  0.000 |    0.0 |
| >>>           |_[long&& std::forward<long>(std::remove_reference<long>::type&)][/Users/jrmadsen/devel/c++/timemory-tu... |      1 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>           |_[void (*&&std::forward<void (*)(long)>(std::remove_reference<void (*)(long)>::type&))(long)][/Users/j... |      1 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>           |_[void std::__invoke_impl<void, void (*)(long), long>(std::__invoke_other, void (*&&)(long), long&&)][... |      1 |      6 | wall   | sec    |  0.636 |  0.636 |  0.636 |  0.636 |  0.000 |    0.0 |
| >>>             |_[void (*&&std::forward<void (*)(long)>(std::remove_reference<void (*)(long)>::type&))(long)][/Users... |      1 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[long&& std::forward<long>(std::remove_reference<long>::type&)][/Users/jrmadsen/devel/c++/timemory-... |      1 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[consume(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler... |      1 |      7 | wall   | sec    |  0.636 |  0.636 |  0.636 |  0.636 |  0.000 |   14.6 |
| >>>               |_[std::unique_lock<std::mutex>::unique_lock(std::mutex&)][/Users/jrmadsen/devel/c++/timemory-tutor... |      1 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   35.7 |
| >>>                 |_[std::mutex* std::__addressof<std::mutex>(std::mutex&)][/Users/jrmadsen/devel/c++/timemory-tuto... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[std::unique_lock<std::mutex>::lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-da... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   68.1 |
| >>>                   |_[std::mutex::lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/... |      1 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   37.9 |
| >>>                     |_[__gthread_mutex_lock(_opaque_pthread_mutex_t*)][/Users/jrmadsen/devel/c++/timemory-tutoria... |      1 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   94.4 |
| >>>                       |_[__gthread_active_p()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/librar... |      1 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::unique_lock<std::mutex>::unique_lock(std::mutex&, std::defer_lock_t)][/Users/jrmadsen/devel... |      1 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   75.0 |
| >>>                 |_[std::mutex* std::__addressof<std::mutex>(std::mutex&)][/Users/jrmadsen/devel/c++/timemory-tuto... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::chrono::duration<long long, std::ratio<1l, 1000l> >::duration<long, void>(long const&)][/Us... |  12151 |      8 | wall   | sec    |  0.004 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::common_type<std::chrono::duration<l... |  12151 |      8 | wall   | sec    |  0.320 |  0.000 |  0.000 |  0.000 |  0.000 |   22.0 |
| >>>                 |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long long, std::... |  12151 |      9 | wall   | sec    |  0.004 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[std::common_type<std::chrono::duration<long long, std::ratio<1l, 1000000000l> >, std::chrono::... |  12151 |      9 | wall   | sec    |  0.243 |  0.000 |  0.000 |  0.000 |  0.000 |   38.5 |
| >>>                   |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::count() const][/Users/jrmads... |  24302 |     10 | wall   | sec    |  0.007 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                   |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::duration<long long, std::rat... |  12151 |     10 | wall   | sec    |  0.138 |  0.000 |  0.000 |  0.000 |  0.000 |   36.7 |
| >>>                     |_[std::enable_if<std::chrono::__is_duration<std::chrono::duration<long long, std::ratio<1l, ... |  12151 |     11 | wall   | sec    |  0.084 |  0.000 |  0.000 |  0.000 |  0.000 |   32.4 |
| >>>                       |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> > std::chrono::__duration_c... |  12151 |     12 | wall   | sec    |  0.057 |  0.000 |  0.000 |  0.000 |  0.000 |   87.0 |
| >>>                         |_[std::chrono::duration<long long, std::ratio<1l, 1000l> >::count() const][/Users/jrmads... |  12151 |     13 | wall   | sec    |  0.004 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                         |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::duration<long long, vo... |  12151 |     13 | wall   | sec    |  0.004 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                     |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::count() const][/Users/jrma... |  12151 |     11 | wall   | sec    |  0.004 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                   |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::duration<long long, void>(lo... |  12151 |     10 | wall   | sec    |  0.004 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long long, std::... |  12151 |      9 | wall   | sec    |  0.004 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[bool std::chrono::operator< <std::chrono::_V2::steady_clock, std::chrono::duration<long long, st... |  12151 |      8 | wall   | sec    |  0.135 |  0.000 |  0.000 |  0.000 |  0.000 |   52.9 |
| >>>                 |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long long, std::... |  24302 |      9 | wall   | sec    |  0.007 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[bool std::chrono::operator< <long long, std::ratio<1l, 1000000000l>, long long, std::ratio<1l,... |  12151 |      9 | wall   | sec    |  0.056 |  0.000 |  0.000 |  0.000 |  0.000 |   87.4 |
| >>>                   |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::count() const][/Users/jrmads... |  24302 |     10 | wall   | sec    |  0.007 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::unique_lock<std::mutex>::try_lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-... |  12150 |      8 | wall   | sec    |  0.084 |  0.000 |  0.000 |  0.000 |  0.000 |   30.9 |
| >>>                 |_[std::mutex::try_lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-ap... |  12150 |      9 | wall   | sec    |  0.058 |  0.000 |  0.000 |  0.000 |  0.000 |   44.4 |
| >>>                   |_[__gthread_mutex_trylock(_opaque_pthread_mutex_t*)][/Users/jrmadsen/devel/c++/timemory-tutori... |  12150 |     10 | wall   | sec    |  0.032 |  0.000 |  0.000 |  0.000 |  0.000 |   85.0 |
| >>>                     |_[__gthread_active_p()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-... |  12150 |     11 | wall   | sec    |  0.005 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::unique_lock<std::mutex>::~unique_lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutor... |      2 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   38.9 |
| >>>                 |_[std::unique_lock<std::mutex>::unlock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-... |      1 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   36.4 |
| >>>                   |_[std::mutex::unlock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-ap... |      1 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   71.4 |
| >>>                     |_[__gthread_mutex_unlock(_opaque_pthread_mutex_t*)][/Users/jrmadsen/devel/c++/timemory-tutor... |      1 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                       |_[__gthread_active_p()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/librar... |      1 |     12 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_[__gnu_cxx::__normal_iterator<std::thread*, std::vector<std::thread, std::allocator<std::thread> > >::operator*... |     10 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)(long), long> > >::~_State_impl()][/Users/jr... |     10 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   92.2 |
| >>>   |_[std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)(long), long> > >::~_State_impl()][/Users/... |     10 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[__gnu_cxx::__normal_iterator<std::thread*, std::vector<std::thread, std::allocator<std::thread> > >::operator+... |     10 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)(long), long> > >::_M_run()][/Users/jrmadsen... |      8 |      1 | wall   | sec    |  4.854 |  0.607 |  0.565 |  0.655 |  0.031 |    0.0 |
| >>>   |_[std::thread::_Invoker<std::tuple<void (*)(long), long> >::operator()()][/Users/jrmadsen/devel/c++/timemory-t... |      8 |      2 | wall   | sec    |  4.854 |  0.607 |  0.565 |  0.655 |  0.031 |    0.0 |
| >>>     |_[void std::thread::_Invoker<std::tuple<void (*)(long), long> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, ... |      8 |      3 | wall   | sec    |  4.853 |  0.607 |  0.565 |  0.655 |  0.031 |    0.0 |
| >>>       |_[std::remove_reference<std::tuple<void (*)(long), long>&>::type&& std::move<std::tuple<void (*)(long), lo... |     16 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>       |_[std::tuple_element<1ul, std::tuple<void (*)(long), long> >::type&& std::get<1ul, void (*)(long), long>(s... |      8 |      4 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   43.2 |
| >>>         |_[std::tuple_element<1ul, std::tuple<void (*)(long), long> >::type& std::get<1ul, void (*)(long), long>(... |      8 |      5 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   30.5 |
| >>>           |_[long& std::__get_helper<1ul, long>(std::_Tuple_impl<1ul, long>&)][/Users/jrmadsen/devel/c++/timemory... |      8 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   61.0 |
| >>>             |_[std::_Tuple_impl<1ul, long>::_M_head(std::_Tuple_impl<1ul, long>&)][/Users/jrmadsen/devel/c++/time... |      8 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   96.0 |
| >>>               |_[std::_Head_base<1ul, long, false>::_M_head(std::_Head_base<1ul, long, false>&)][/Users/jrmadsen/... |      8 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[long&& std::forward<long&&>(std::remove_reference<long&&>::type&)][/Users/jrmadsen/devel/c++/timemory-... |      8 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>       |_[std::tuple_element<0ul, std::tuple<void (*)(long), long> >::type&& std::get<0ul, void (*)(long), long>(s... |      8 |      4 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   43.0 |
| >>>         |_[std::tuple_element<0ul, std::tuple<void (*)(long), long> >::type& std::get<0ul, void (*)(long), long>(... |      8 |      5 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   27.5 |
| >>>           |_[void (*&std::__get_helper<0ul, void (*)(long), long>(std::_Tuple_impl<0ul, void (*)(long), long>&))(... |      8 |      6 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   53.5 |
| >>>             |_[std::_Tuple_impl<0ul, void (*)(long), long>::_M_head(std::_Tuple_impl<0ul, void (*)(long), long>&)... |      8 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   97.1 |
| >>>               |_[std::_Head_base<0ul, void (*)(long), false>::_M_head(std::_Head_base<0ul, void (*)(long), false>... |      8 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[void (*&&std::forward<void (*&&)(long)>(std::remove_reference<void (*&&)(long)>::type&))(long)][/Users... |      8 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>       |_[std::__invoke_result<void (*)(long), long>::type std::__invoke<void (*)(long), long>(void (*&&)(long), l... |      8 |      4 | wall   | sec    |  4.850 |  0.606 |  0.564 |  0.654 |  0.031 |    0.0 |
| >>>         |_[long&& std::forward<long>(std::remove_reference<long>::type&)][/Users/jrmadsen/devel/c++/timemory-tuto... |      8 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[void (*&&std::forward<void (*)(long)>(std::remove_reference<void (*)(long)>::type&))(long)][/Users/jrm... |      8 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[void std::__invoke_impl<void, void (*)(long), long>(std::__invoke_other, void (*&&)(long), long&&)][/U... |      8 |      5 | wall   | sec    |  4.849 |  0.606 |  0.564 |  0.654 |  0.031 |    0.0 |
| >>>           |_[void (*&&std::forward<void (*)(long)>(std::remove_reference<void (*)(long)>::type&))(long)][/Users/j... |      8 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>           |_[long&& std::forward<long>(std::remove_reference<long>::type&)][/Users/jrmadsen/devel/c++/timemory-tu... |      8 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>           |_[consume(long)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_compiler_i... |      8 |      6 | wall   | sec    |  4.849 |  0.606 |  0.564 |  0.654 |  0.031 |   14.6 |
| >>>             |_[std::unique_lock<std::mutex>::unique_lock(std::mutex&)][/Users/jrmadsen/devel/c++/timemory-tutoria... |      8 |      7 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   32.9 |
| >>>               |_[std::mutex* std::__addressof<std::mutex>(std::mutex&)][/Users/jrmadsen/devel/c++/timemory-tutori... |      8 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::unique_lock<std::mutex>::lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-... |      8 |      8 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   45.9 |
| >>>                 |_[std::mutex::lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01... |      8 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   29.7 |
| >>>                   |_[__gthread_mutex_lock(_opaque_pthread_mutex_t*)][/Users/jrmadsen/devel/c++/timemory-tutorials... |      8 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   97.1 |
| >>>                     |_[__gthread_active_p()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-... |      8 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[std::unique_lock<std::mutex>::unique_lock(std::mutex&, std::defer_lock_t)][/Users/jrmadsen/devel/c... |      8 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   89.2 |
| >>>               |_[std::mutex* std::__addressof<std::mutex>(std::mutex&)][/Users/jrmadsen/devel/c++/timemory-tutori... |      8 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[std::chrono::duration<long long, std::ratio<1l, 1000l> >::duration<long, void>(long const&)][/User... |  92773 |      7 | wall   | sec    |  0.027 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::common_type<std::chrono::duration<lon... |  92773 |      7 | wall   | sec    |  2.443 |  0.000 |  0.000 |  0.002 |  0.000 |   22.3 |
| >>>               |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long long, std::ra... |  92773 |      8 | wall   | sec    |  0.026 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::common_type<std::chrono::duration<long long, std::ratio<1l, 1000000000l> >, std::chrono::du... |  92773 |      8 | wall   | sec    |  1.845 |  0.000 |  0.000 |  0.001 |  0.000 |   38.7 |
| >>>                 |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::count() const][/Users/jrmadsen... | 185546 |      9 | wall   | sec    |  0.053 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::duration<long long, std::ratio... |  92773 |      9 | wall   | sec    |  1.051 |  0.000 |  0.000 |  0.001 |  0.000 |   37.0 |
| >>>                   |_[std::enable_if<std::chrono::__is_duration<std::chrono::duration<long long, std::ratio<1l, 10... |  92773 |     10 | wall   | sec    |  0.636 |  0.000 |  0.000 |  0.001 |  0.000 |   32.7 |
| >>>                     |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> > std::chrono::__duration_cas... |  92773 |     11 | wall   | sec    |  0.428 |  0.000 |  0.000 |  0.000 |  0.000 |   87.4 |
| >>>                       |_[std::chrono::duration<long long, std::ratio<1l, 1000l> >::count() const][/Users/jrmadsen... |  92773 |     12 | wall   | sec    |  0.027 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                       |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::duration<long long, void... |  92773 |     12 | wall   | sec    |  0.027 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                   |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::count() const][/Users/jrmads... |  92773 |     10 | wall   | sec    |  0.026 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>                 |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::duration<long long, void>(long... |  92773 |      9 | wall   | sec    |  0.026 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long long, std::ra... |  92773 |      8 | wall   | sec    |  0.027 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[bool std::chrono::operator< <std::chrono::_V2::steady_clock, std::chrono::duration<long long, std:... |  92773 |      7 | wall   | sec    |  1.023 |  0.000 |  0.000 |  0.001 |  0.000 |   53.1 |
| >>>               |_[std::chrono::time_point<std::chrono::_V2::steady_clock, std::chrono::duration<long long, std::ra... | 185546 |      8 | wall   | sec    |  0.053 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[bool std::chrono::operator< <long long, std::ratio<1l, 1000000000l>, long long, std::ratio<1l, 1... |  92773 |      8 | wall   | sec    |  0.426 |  0.000 |  0.000 |  0.001 |  0.000 |   87.5 |
| >>>                 |_[std::chrono::duration<long long, std::ratio<1l, 1000000000l> >::count() const][/Users/jrmadsen... | 185546 |      9 | wall   | sec    |  0.053 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[std::unique_lock<std::mutex>::try_lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-da... |  92765 |      7 | wall   | sec    |  0.646 |  0.000 |  0.000 |  0.001 |  0.000 |   31.0 |
| >>>               |_[std::mutex::try_lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/... |  92765 |      8 | wall   | sec    |  0.446 |  0.000 |  0.000 |  0.001 |  0.000 |   44.9 |
| >>>                 |_[__gthread_mutex_trylock(_opaque_pthread_mutex_t*)][/Users/jrmadsen/devel/c++/timemory-tutorial... |  92765 |      9 | wall   | sec    |  0.246 |  0.000 |  0.000 |  0.000 |  0.000 |   85.3 |
| >>>                   |_[__gthread_active_p()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-ap... |  92765 |     10 | wall   | sec    |  0.036 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>             |_[std::unique_lock<std::mutex>::~unique_lock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutoria... |     16 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   27.1 |
| >>>               |_[std::unique_lock<std::mutex>::unlock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-da... |      8 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   51.7 |
| >>>                 |_[std::mutex::unlock()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/... |      8 |      9 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   77.0 |
| >>>                   |_[__gthread_mutex_unlock(_opaque_pthread_mutex_t*)][/Users/jrmadsen/devel/c++/timemory-tutoria... |      8 |     10 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   76.9 |
| >>>                     |_[__gthread_active_p()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-... |      8 |     11 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_[std::vector<std::thread, std::allocator<std::thread> >::~vector()][/Users/jrmadsen/devel/c++/timemory-tutorial... |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    7.5 |
| >>>   |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_M_get_Tp_allocator()][/Users/jrmadsen/devel/c... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_[void std::_Destroy<std::thread*, std::thread>(std::thread*, std::thread*, std::allocator<std::thread>&)][/Us... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    2.8 |
| >>>     |_[void std::_Destroy<std::thread*>(std::thread*, std::thread*)][/Users/jrmadsen/devel/c++/timemory-tutorials... |      1 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    6.9 |
| >>>       |_[void std::_Destroy_aux<false>::__destroy<std::thread*>(std::thread*, std::thread*)][/Users/jrmadsen/deve... |      1 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   39.0 |
| >>>         |_[std::thread* std::__addressof<std::thread>(std::thread&)][/Users/jrmadsen/devel/c++/timemory-tutorials... |     10 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         |_[void std::_Destroy<std::thread>(std::thread*)][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-d... |     10 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   31.4 |
| >>>           |_[std::thread::~thread()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/01_c... |     10 |      6 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   24.1 |
| >>>             |_[std::thread::joinable() const][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library... |     10 |      7 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   87.3 |
| >>>               |_[std::thread::id::id()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/library-api/0... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>               |_[std::operator==(std::thread::id, std::thread::id)][/Users/jrmadsen/devel/c++/timemory-tutorials/... |     10 |      8 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>   |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::~_Vector_base()][/Users/jrmadsen/devel/c++/tim... |      1 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   32.6 |
| >>>     |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_M_deallocate(std::thread*, unsigned long)][... |      1 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   44.4 |
| >>>       |_[std::allocator_traits<std::allocator<std::thread> >::deallocate(std::allocator<std::thread>&, std::threa... |      1 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   80.0 |
| >>>         |_[__gnu_cxx::new_allocator<std::thread>::deallocate(std::thread*, unsigned long)][/Users/jrmadsen/devel/... |      1 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>     |_[std::_Vector_base<std::thread, std::allocator<std::thread> >::_Vector_impl::~_Vector_impl()][/Users/jrmads... |      1 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   45.5 |
| >>>       |_[std::allocator<std::thread>::~allocator()][/Users/jrmadsen/devel/c++/timemory-tutorials/tutorial-day-1/l... |      1 |      4 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   83.3 |
| >>>         |_[__gnu_cxx::new_allocator<std::thread>::~new_allocator()][/Users/jrmadsen/devel/c++/timemory-tutorials/... |      1 |      5 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
```
