#!/usr/bin/env python

import sys
import json
import timemory
from timemory.bundle import marker

simple_set = ["wall_clock", "peak_rss"]
extended_set = ["cpu_clock", "cpu_util",
                "page_rss", "virtual_memory"]


def fib(n):
    return n if n < 2 else (fib(n-1) + fib(n-2))


@marker(simple_set)
def run(n):
    ''' Decorator and context manager for high-level custom collection(s)'''
    fib(n)
    with marker(simple_set + extended_set, key="auto_tuple_ctx_manager"):
        fib(n)


if __name__ == "__main__":
    # read command line
    n = int(sys.argv[1] if len(sys.argv) > 1 else 10)

    timemory.settings.flat_profile = True
    timemory.settings.timeline_profile = True

    run(n)

    timemory.finalize()
    # Get the results as dictionary
    #data = timemory.get()

    # dump the output in pretty JSON
    #print("\n{}".format(json.dumps(data, indent=4, sort_keys=True)))

    # suppress output
    #timemory.settings.auto_output = False
