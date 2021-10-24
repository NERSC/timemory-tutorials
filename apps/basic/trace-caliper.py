#!/usr/bin/env python

import time
import argparse
import numpy as np
import timemory
from timemory.trace import trace
from timemory.component import CaliperConfig


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


@trace(["caliper_marker"])
def run(n):
    print(f"Running fibonacci({n})...")
    ret = fibonacci(n) + fibonacci(n % 5 + 1)
    print(f"Running inefficient({n})...")
    return inefficient(n) / ret


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-n",
        "--nfib",
        help="Fibonacci value",
        type=int,
        default=19,
    )
    parser.add_argument(
        "-c",
        "--config",
        help="Caliper configuration",
        type=str,
        nargs="*",
        default=["runtime-report"],
    )

    args = parser.parse_args()

    cfg = CaliperConfig()
    cfg.configure(",".join(args.config))
    cfg.start()

    ts = time.perf_counter()
    ans = run(args.nfib)
    ts = time.perf_counter() - ts
    print("")
    print("Solution           :  {:12.6e}".format(ans))
    print("Elapsed time (sec) :  {:12.6f}".format(ts))

    cfg.stop()
    timemory.finalize()
