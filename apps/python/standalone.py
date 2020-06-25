#!@PYTHON_EXECUTABLE@

import sys
import json
import timemory
from timemory.component import WallClock


def fib(n):
    return n if n < 2 else (fib(n-1) + fib(n-2))


if __name__ == "__main__":
    # read command line
    nfib = int(sys.argv[1]) if len(sys.argv) > 1 else 20
    nitr = int(sys.argv[2]) if len(sys.argv) > 2 else 10

    wc = WallClock("")
    ntot = 0
    for i in range(nitr):
        wc.start()
        ntot += run(n)
        wc.stop()

    print("fibonacci({}) x {} iterations = {}".format(nfib, nitr, ntot))
    print("Total runtime: {:12.6} {}".format(wc.get(), wc.display_units()))
