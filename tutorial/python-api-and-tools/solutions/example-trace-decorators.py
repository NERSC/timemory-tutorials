import sys
import numpy as np
from timemory.trace import trace

@trace(["wall_clock"])
def fib(n):
    return n if n < 2 else (fib(n - 1) + fib(n - 2))

@trace(["wall_clock"])
def inefficient(n):
    a = 0
    for i in range(n):
        a += i
        for j in range(n):
            a += j
    arr = np.arange(a * n * n * n, dtype=np.double)
    return arr.sum()


def run(n):
    ret = 0
    print("Running fibonacci...")
    ret += fib(n)
    ret += fib(n % 5 + 1)
    print("Running inefficient...")
    ret += inefficient(n)
    return ret


if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 20
    ans = run(n)
    print("Answer = {}".format(ans))
