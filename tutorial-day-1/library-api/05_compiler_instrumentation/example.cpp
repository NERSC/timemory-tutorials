// MIT License
//
// Copyright (c) 2020, The Regents of the University of California,
// through Lawrence Berkeley National Laboratory (subject to receipt of any
// required approvals from the U.S. Dept. of Energy).  All rights reserved.
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
    // associate but defer
    lock_t try_lk{ mutex, std::defer_lock };
    // try until time point
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
