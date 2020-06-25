#!/global/project/projectdirs/m1759/timemory-tutorial/spack/opt/spack/cray-cnl7-skylake_avx512/gcc-7.3.0/python-3.7.7-2al4c7otz3ozjcengqvynx2jzxzcrisf/bin/python

import sys
import json
import timemory
from timemory.util import marker

def fib(n):
    return n if n < 2 else (fib(n-1) + fib(n-2))

@marker(["wall_clock"])
def run(n):
    ''' Decorator and context manager for high-level pre-defined collection '''
    fib(n)

if __name__ == "__main__":
    n = int(sys.argv[1] if len(sys.argv) > 1 else 10)
    run(n)
    timemory.finalize()
