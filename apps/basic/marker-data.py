#!/usr/bin/env python

import os
import json
import time
import argparse
import numpy as np
import timemory
from timemory.bundle import marker
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
    with profile(["wall_clock"]):
        ret = fibonacci(n)
    print(f"Running inefficient({n})...")
    with marker(["wall_clock"], f"inefficient({n})"):
        ret = inefficient(n) / ret
    return ret


if __name__ == "__main__":
    timemory.settings.precision = 6
    timemory.settings.timing_units = "usec"
    timemory.settings.auto_output = False
    timemory.settings.time_output = True
    timemory.settings.flat_profile = False
    timemory.settings.timeline_profile = True

    # below are meaningless in a timeline profile
    os.environ["TIMEMORY_PRINT_COUNT"] = "OFF"
    os.environ["TIMEMORY_PRINT_STATS"] = "OFF"
    os.environ["TIMEMORY_PRINT_MEAN"] = "OFF"

    parser = argparse.ArgumentParser("example")
    parser.add_argument(
        "-n",
        "--nfib",
        help="Fibonacci value",
        type=int,
        default=19,
    )
    parser.add_argument(
        "-o",
        "--output",
        help="Output file name w/o extension",
        type=str,
        default="output",
    )
    timemory.add_arguments(parser)

    args = parser.parse_args()

    with marker(["wall_clock"], os.path.basename(__file__)):
        ts = time.perf_counter()
        ans = run(args.nfib)
        ts = time.perf_counter() - ts
        print("")
        print("Solution           :  {:12.6e}".format(ans))
        print("Elapsed time (sec) :  {:12.6f}".format(ts))

    # Get the results as dictionary
    data = timemory.get(hierarchy=False, components=["wall_clock"])

    # dump the output in pretty JSON
    fname = timemory.settings.compose_output_filename(args.output, ".json")
    print(f"\nWriting data to '{fname}'...\n")
    with open(fname, "w") as w:
        w.write("{}\n".format(json.dumps(data, indent=2, sort_keys=True)))

    data = timemory.storage.WallClockStorage.get()
    print("[flat storage] {} :: {}".format(data[0].prefix(), data[0].data()))

    data = timemory.storage.WallClockStorage.get_tree()
    data = data[0].children()  # first node is dummy node
    print(
        "[tree storage] {} :: {}".format(
            data[0].value().prefix(), data[0].value().inclusive().data()
        )
    )

    timemory.finalize()
