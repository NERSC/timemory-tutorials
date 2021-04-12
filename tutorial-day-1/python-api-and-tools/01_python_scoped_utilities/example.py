#!/usr/bin/env python

import sys
import json


def fib(n):
    return n if n < 2 else (fib(n - 1) + fib(n - 2))


def run(n):
    """ Decorator and context manager for high-level pre-defined collection """
    fib(n)


if __name__ == "__main__":
    n = int(sys.argv[1] if len(sys.argv) > 1 else 10)
    run(n)
