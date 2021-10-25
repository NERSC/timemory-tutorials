#!/usr/bin/env python

import sys
import time
import argparse
import numpy as np
import timemory


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
    ret = fibonacci(n) + fibonacci(n % 5 + 1)
    print(f"Running inefficient({n})...")
    return inefficient(n) / ret


if __name__ == "__main__":
    timemory.init([__file__])

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-n",
        "--nfib",
        help="Fibonacci value",
        type=int,
        default=[11, 13, 15, 17, 19],
        nargs="+",
    )

    args = parser.parse_args(sys.argv[1:])

    # add an array
    timemory.manager.add_metadata("nfib", args.nfib)
    performance = []
    for i, n in enumerate(args.nfib):
        ts = time.perf_counter()
        ans = run(n)
        ts = time.perf_counter() - ts

        print("Solution           :  {:12.6e}".format(ans))
        print("Elapsed time (sec) :  {:12.6f}".format(ts))
        print("")

        timemory.manager.add_metadata(f"solution@{i}", ans)
        performance.append(ts / n)

    timemory.manager.add_metadata("performance", performance)

    timemory.finalize()
