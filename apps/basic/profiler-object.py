#!/usr/bin/env python

import sys
import time
import argparse
import numpy as np
import timemory
from timemory.profiler import profile
from timemory.profiler import config as profiler_config


def fibonacci(n):
    return n if n < 2 else (fibonacci(n - 1) + fibonacci(n - 2))


def inefficient(n):
    a = 0
    for i in range(n):
        a += i
        for j in range(n):
            a += j
    arr = np.arange(a * n * n * n, dtype=np.double)
    return arr.sum()


def run(n):
    ret = 0

    print(f"Running fibonacci({n})...")
    obj = profile(["wall_clock"])
    obj.start()
    ret = fibonacci(n)
    obj.stop()

    print(f"Running inefficient({n})...")
    obj = profile(["wall_clock"], flat=True, timeline=True)
    obj.start()
    ret = inefficient(n) / (ret + fibonacci(n % 5 + 1))
    obj.stop()

    return ret


if __name__ == "__main__":
    timemory.init([__file__] + sys.argv[1:])
    timemory.settings.precision = 6
    profiler_config.include_line = True
    profiler_config.full_filepath = True

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-n",
        "--nfib",
        help="Fibonacci value",
        type=int,
        default=19,
    )

    args = parser.parse_args(sys.argv[1:])

    ts = time.perf_counter()
    ans = run(args.nfib)
    ts = time.perf_counter() - ts
    print("")
    print("Solution           :  {:12.6e}".format(ans))
    print("Elapsed time (sec) :  {:12.6f}".format(ts))

    # finalize
    timemory.finalize()
