#!/usr/bin/env python

import sys
import numpy as np


@profile
def fib(n):
    return n if n < 2 else (fib(n - 1) + fib(n - 2))


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
    print("Running fibonacci...")
    ret += fib(nfib)
    ret += fib(nfib % 5 + 1)
    print("Running inefficient...")
    ret += inefficient(nfib)
    return ret


if __name__ == "__main__":
    nfib = int(sys.argv[1]) if len(sys.argv) > 1 else 20
    ans = run(nfib)
    print("Answer = {}".format(ans))
