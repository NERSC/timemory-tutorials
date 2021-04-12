#!/usr/bin/env python

import sys
import timemory
from timemory.profiler import profile


def fib(n):
    return n if n < 2 else (fib(n-1) + fib(n-2))


@profile(["wall_clock"])
def run_profile(nfib):
    return fib(nfib)


if __name__ == "__main__":
    # read command line
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 23

    run_profile(n)

    # finalize
    timemory.finalize()
