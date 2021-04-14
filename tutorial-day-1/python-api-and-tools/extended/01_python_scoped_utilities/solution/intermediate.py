#!/usr/bin/env python

import sys
import json
import argparse
import timemory
from timemory.bundle import marker

simple_set = ["wall_clock", "peak_rss"]
extended_set = ["cpu_clock", "page_rss", "virtual_memory"]


@marker(simple_set)
def fib(n):
    return n if n < 2 else (fib(n - 1) + fib(n - 2))


def run(n):
    """ Decorator and context manager for high-level custom collection(s)"""
    with marker(simple_set + extended_set, f"run({n})"):
        fib(n)


if __name__ == "__main__":
    timemory.settings.scientific = True
    timemory.settings.flat_profile = True
    timemory.settings.timeline_profile = True
    timemory.settings.cout_output = False

    parser = argparse.ArgumentParser("example")
    parser.add_argument(
        "-n", "--fibonacci-value", default=15, type=int, help="Starting fibonacci value"
    )
    timemory.add_arguments(parser)

    args = parser.parse_args()

    run(args.fibonacci_value)

    # Get the results as dictionary
    data = timemory.get(hierarchy=False, components=["wall_clock"])

    # dump the output in pretty JSON
    with open("output.json", "w") as w:
        print("Outputting wall clock data to output.json")
        w.write("{}\n".format(json.dumps(data, indent=2, sort_keys=True)))

    print("\n{}".format(timemory.get_text(extended_set)))

    timemory.finalize()
