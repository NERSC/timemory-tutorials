#!/usr/bin/env python

import os
import time
import argparse
import numpy as np
import timemory
from timemory.bundle import marker
from timemory.component import CaliperConfig


components = []


def get_tools(extra=[]):
    return ["caliper_marker"] + extra + timemory.settings.global_components.split()


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
    timemory.init([__file__])

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
    parser.add_argument(
        "-C",
        "--components",
        help="timemory component types",
        default=["user_global_bundle"],
        choices=timemory.component.get_available_types(),
        nargs="*",
        type=str,
    )
    timemory.settings.add_arguments(
        parser, subparser=False, selection=["global_components"]
    )

    args = parser.parse_args()

    cfg = CaliperConfig()
    cfg.configure(",".join(args.config))
    cfg.start()

    with marker(get_tools(args.components), os.path.basename(__file__)):
        ts = time.perf_counter()
        ans = run(args.nfib)
        ts = time.perf_counter() - ts
        print("")
        print("Solution           :  {:12.6e}".format(ans))
        print("Elapsed time (sec) :  {:12.6f}".format(ts))

    cfg.stop()
    timemory.finalize()
