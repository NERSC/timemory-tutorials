#!/usr/bin/env python

import os
import time
import argparse
import numpy as np
import timemory
from timemory.bundle import marker


def get_tools():
    return [timemory.settings.components]


@marker(get_tools)
def fibonacci(n):
    return n if n < 2 else (fibonacci(n - 1) + fibonacci(n - 2))


@marker(get_tools, add_args=True)
def inefficient(n):
    a = 0
    for i in range(n):
        a += i
        for j in range(n):
            a += j
    arr = np.arange(a * n * n * n, dtype=np.double)
    return arr.sum()


@marker(get_tools, add_args=True)
def run(n):
    print(f"Running fibonacci({n})...")
    ret = fibonacci(n) + fibonacci(n % 5 + 1)
    print(f"Running inefficient({n})...")
    return inefficient(n) / ret


if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        usage="<script> -n [VALUE] timemory-config --components [COMPONENTS...]"
    )
    parser.add_argument(
        "-n",
        "--nfib",
        help="Fibonacci value",
        type=int,
        default=19,
    )
    timemory.settings.add_arguments(parser)

    args = parser.parse_args()

    with marker(get_tools, os.path.basename(__file__)):
        ts = time.perf_counter()
        ans = run(args.nfib)
        ts = time.perf_counter() - ts
        print("")
        print("Solution           :  {:12.6e}".format(ans))
        print("Elapsed time (sec) :  {:12.6f}".format(ts))

    timemory.finalize()
