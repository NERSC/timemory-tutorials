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

#include "timemory/library.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>

long
fib(long n)
{
    return (n < 2) ? n : (fib(n - 1) + fib(n - 2));
}

int
main(int argc, char** argv)
{
    long nfib = (argc > 1) ? atol(argv[1]) : 43;
    int  nitr = (argc > 2) ? atoi(argv[2]) : 2;
// initialize timemory library
    timemory_init_library(argc, argv);
// set default components
    timemory_set_default("wall_clock, cpu_clock");
// begin recording components for main region    
    timemory_push_region("main/total"); 
// begin recording for fib
    timemory_push_region("fib");
    long     ans = fib(nfib);
// end recording for fib
    timemory_pop_region("fib");
    
    timemory_add_components("cpu_util");
  timemory_remove_components("cpu_util");  
//begin collection for loops total
   timemory_push_region("total_loops");
// begin collection for first loop
    timemory_push_region("loop_1_region");
    for(int i = 0; i < nitr*2; ++i)
    {
        ans += fib(nfib + 1);
    }
// end collection for first loop
    timemory_pop_region("loop_1_region");


// begin collection for second loop
    timemory_push_region("loop_2_region");
    for(int i = 0; i < nitr; ++i)
    {
       	ans += fib(nfib + 1);
    }
// end collection for second loop
    timemory_pop_region("loop_2_region");

// end collection for loops total
    timemory_pop_region("total_loops");

// end recording for main region
    timemory_pop_region("main/total");

    std::cout<<"Answer = "<< ans<<"\n";

// finalize timemory library
    timemory_finalize_library();
    return EXIT_SUCCESS;
}
