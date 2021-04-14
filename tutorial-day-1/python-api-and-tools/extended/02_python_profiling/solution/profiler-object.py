#!/usr/bin/env python

import sys
import numpy as np
import timemory
from timemory.profiler import profile


def fib(n):
    return n if n < 2 else (fib(n - 1) + fib(n - 2))


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
    
    print("Running fibonacci...")
    obj = profile(["wall_clock"], flat=True)
    obj.start()
    ret += fib(n)
    ret += fib(n % 5 + 1)
    obj.stop()

    print("Running inefficient...")
    obj = profile(["wall_clock"], timeline=True)
    obj.start()
    ret += inefficient(n)
    obj.stop()

    return ret


if __name__ == "__main__":
    # read command line
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 23

    ans = run(n)
    print("Answer = {}".format(ans))

    # finalize
    timemory.finalize()
