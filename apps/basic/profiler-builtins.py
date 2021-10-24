#!/usr/bin/env python

import sys
import time
import argparse
import numpy as np


@profile
def fibonacci(n):
    return n if n < 2 else (fibonacci(n - 1) + fibonacci(n - 2))


@profile
def inefficient(n):
    a = 0
    for i in range(n):
        a += i
        for j in range(n):
            a += j
    arr = np.arange(a * n * n * n, dtype=np.double)
    return arr.sum()


def run(nfib):
    ret = 0
    ret += fibonacci(nfib)
    ret += fibonacci(nfib % 5 + 1)
    ret += inefficient(nfib)
    return ret


if __name__ == "__main__":
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
