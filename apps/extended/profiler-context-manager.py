#!/usr/bin/env python

import sys
import time
import argparse
import numpy as np
import timemory
from timemory.profiler import profile


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
    print(f"Running fibonacci({n})...")
    with profile(["wall_clock"], flat=True, timeline=False):
        ret = fibonacci(n) + fibonacci(n % 5 + 1)
    print(f"Running inefficient({n})...", flat=False, timeline=True)
    with profile(["wall_clock"]):
        ret = inefficient(n) / ret
    return ret


if __name__ == "__main__":
    timemory.init([__file__])

    parser = argparse.ArgumentParser(usage="<script> -n [VALUE]")
    parser.add_argument(
        "-n",
        "--nfib",
        help="Fibonacci value",
        type=int,
        default=19,
    )

    args = parser.parse_args()

    ts = time.perf_counter()
    ans = run(args.nfib)
    ts = time.perf_counter() - ts
    print("")
    print("Solution           :  {:12.6e}".format(ans))
    print("Elapsed time (sec) :  {:12.6f}".format(ts))

    timemory.finalize()
