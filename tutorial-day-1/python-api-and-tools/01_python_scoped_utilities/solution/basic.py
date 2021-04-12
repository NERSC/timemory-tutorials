#!/usr/bin/env python

import sys
import json
import timemory
from timemory.util import marker


def fib(n):
    return n if n < 2 else (fib(n - 1) + fib(n - 2))


@marker(["wall_clock"])
def run(n):
    """ Decorator and context manager for high-level pre-defined collection """
    fib(n)


if __name__ == "__main__":
    n = int(sys.argv[1] if len(sys.argv) > 1 else 10)
    timemory.settings.scientific = True
    run(n)
    timemory.finalize()
