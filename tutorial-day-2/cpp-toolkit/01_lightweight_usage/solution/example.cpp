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

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>

#include <timemory/timemory.hpp>

int
main(int argc, char** argv)
{
    tim::settings::timing_units() = "nsec";
    tim::timemory_init(argc, argv);

    int64_t dsize   = 250;      // data size
    int64_t nrepeat = 50;       // number of times to repeat calculation
    int64_t nitr    = 10;       // iterations over data
    int64_t nseed   = 5464515;  // random seed

    if(argc > 1)
        dsize = std::stoul(argv[1]);
    if(argc > 2)
        nrepeat = std::stoul(argv[2]);
    if(argc > 3)
        nitr = std::stoul(argv[3]);
    if(argc > 4)
        nseed = std::stoul(argv[4]);

    set_seed(nseed);

    auto sum = execute_matmul(dsize, nrepeat, nitr);
    std::cout << "MM sum = " << sum << std::endl;

    tim::timemory_finalize();
    return 0;
}
