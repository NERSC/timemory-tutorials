# Python Profiling

This example walks through the usage of profiler and line profiler sub-packages available in timemory Python package. The `profile` and `trace` can be used as decorator, context manager, or an object. These types takes a user-provided list of timemory components as an argument.

For various compatibility reasons, the class `timemory.profiler.Profiler` is aliased to `profiler` and `profile`
and the class `timemory.trace.Tracer` is aliased to `tracer` and `trace`. When used via the command-line,
a bare `@profile` or `@trace` can be use for either `timemory.profiler` or `timemory.trace`.

```python
from timemory.profiler import profile
from timemory.trace import trace
```

## Decorators

```python
@profile(["wall_clock"])
def foo():
    # ...
```

## Context-Manager

```python
def foo():
    with trace(["wall_clock"], key="<some-key"):
        # ...
```

## Object

```python
def foo():
    obj = profile(["wall_clock", "peak_rss"], flat=False, timeline=False)
    obj.start()

    # ...

    obj.stop()
```

## Built-ins

For the most flexibility from the command-line, add `@profile` to your code. The `@profile` built-in
is supported by the profiler, tracer, and line-profiler.

```python
@profile
def foo():
    # ...
```

## Starting Code

```python
import sys
import numpy as np


def fib(n):
    return n if n < 2 else (fib(n - 1) + fib(n - 2))


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
```

## Exercise

1. Run the following commands on `example.py` without modifying the python script:

```console
python -m timemory.trace         -d timemory-trace-output   -- ./example.py
python -m timemory.profiler      -d timemory-profile-output -- ./example.py
python -m timemory.line_profiler -v                         -- ./example.py
```

> You should note that the timemory profiler and tracer will instrument all function calls and all
> lines of code executed, respectively, while the line-profiler will produce nothing: it only works
> when `@profile` is added.

2. Add `@profile` to the fibonacci and inefficient functions.

```python
@profile
def fib(n):
    # ...

@profile
def inefficient(n):
    # ...
```

> Run the same commands as before and you will note that the line-profiler produces
> output now but the timemory profiler and tracer provide the same output as before.

3. Add `-b` to the timemory profiler and tracer commands to limit the scope to built-ins

```console
python -m timemory.trace         -b -d timemory-builtin-trace-output   -- ./example.py
python -m timemory.profiler      -b -d timemory-builtin-profile-output -- ./example.py
python -m timemory.line_profiler -v                                    -- ./example.py
```

> You will note now that the timemory profiler and tracer outputs record the `@profile` functions
> __*and*__ the functions which are called within these functions. Add `--only-files example.py`
> to limit the profiling to the exact same set of lines as the line-profiler.

4. Select a different component

```console
python -m timemory.trace         -c cpu_clock -b -d timemory-builtin-trace-output   -- ./example.py
python -m timemory.profiler      -c cpu_clock -b -d timemory-builtin-profile-output -- ./example.py
python -m timemory.line_profiler -c cpu_clock -v                                    -- ./example.py
```

5. Modify example.py directly to use the profiler or tracer via a decorator, context-manager, or object.

```python
@profile(["wall_clock"], flat=True)
def fib(n):
    # ...

@profile(["wall_clock"], timeline=True)
def inefficient(n):
    # ...
```

```python
@trace(["wall_clock"])
def fib(n):
    # ...

@trace(["wall_clock"])
def inefficient(n):
    # ...
```

```python
def run(nfib):
    ret = 0

    obj = profile(["wall_clock"], flat=True)
    obj.start()
    ret += fib(nfib)
    ret += fib(nfib % 5 + 1)
    obj.stop()

    obj = profile(["wall_clock"], timeline=True)
    obj.start()
    ret += inefficient(nfib)
    obj.stop()
    return ret

```

6. Configure the profiler and tracer settings

```python
from timemory.profiler import config as profiler_config

print(help(profiler_config))
profiler_config.only_filenames = [__file__]
```

```python
from timemory.trace import config as tracer_config

print(help(tracer_config))
tracer_config.only_filenames = [__file__]
```

### Decorator

```python
from timemory.profiler import profile

@profile(["wall_clock", "peak_rss"], flat=False, timeline=False)
def profile_decorator(nfib=3):
    return fibonacci(nfib)
```

### Context Manager

```python
def main():
    with profile(["wall_clock", "peak_rss"], flat=False, timeline=False):
        ans = fibonacci(nfib=3)
```

### Object

```python
def main():
    timemory.settings.flat_profile = False
    timemory.settings.timeline_profile = False
    prof = profile(["wall_clock", "peak_rss"])
    prof.start()
    profile_object(nfib=3)
    return
```

## Example Outputs

### Profiler via Decorator

```console
$ ./solution/profiler-decorator.py

Running fibonacci...
Running inefficient...
Answer = 2728981178161936.0
[wall]|0> Outputting 'timemory-profiler-decorator-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-profiler-decorator-output/wall.json'...
[wall]|0> Outputting 'timemory-profiler-decorator-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-profiler-decorator-output/wall.txt'...

|------------------------------------------------------------------------------------------------------------------------------------|
|                                              REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                              |
|------------------------------------------------------------------------------------------------------------------------------------|
|                  LABEL                   | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> fib/profiler-decorator.py:9          |  92744 |      0 | wall   | sec    | 23.341 |  0.000 |  0.000 |  1.520 |  0.007 |  100.0 |
| >>> inefficient/profiler-decorator.py:14 |      1 |      0 | wall   | sec    |  0.222 |  0.222 |  0.222 |  0.222 |  0.000 |  100.0 |
| >>> inefficient/profiler-decorator.py:14 |      1 |      0 | wall   | sec    |  0.193 |  0.193 |  0.193 |  0.193 |  0.000 |  100.0 |
| >>> inefficient/profiler-decorator.py:14 |      1 |      0 | wall   | sec    |  0.029 |  0.029 |  0.029 |  0.029 |  0.000 |  100.0 |
| >>> _sum/_methods.py:45                  |      1 |      0 | wall   | sec    |  0.029 |  0.029 |  0.029 |  0.029 |  0.000 |  100.0 |
| >>> _sum/_methods.py:45                  |      1 |      0 | wall   | sec    |  0.029 |  0.029 |  0.029 |  0.029 |  0.000 |  100.0 |
|------------------------------------------------------------------------------------------------------------------------------------|

```

### Tracer via Decorator

```console
$ ./solution/tracer.py

Running fibonacci...
Running inefficient...
Answer = 2728981178161936.0
[wall]|0> Outputting 'timemory-tracer-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-tracer-output/wall.json'...
[wall]|0> Outputting 'timemory-tracer-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-tracer-output/wall.txt'...

|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                                                    REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                                                                    |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                        LABEL                                                         | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|----------------------------------------------------------------------------------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> @trace(["wall_clock"])                                                                        [fib/tracer.py:09] |        |      0 |        |        |        |        |        |        |        |        |
| >>> def fib(n):                                                                                   [fib/tracer.py:10] |        |      0 |        |        |        |        |        |        |        |        |
| >>>     return n if n < 2 else (fib(n - 1) + fib(n - 2))                                          [fib/tracer.py:11] |  92735 |      0 | wall   | sec    |  0.345 |  0.000 |  0.345 |  0.345 |  0.000 |  100.0 |
| >>> @trace(["wall_clock"])                                                                [inefficient/tracer.py:14] |        |      0 |        |        |        |        |        |        |        |        |
| >>> def inefficient(n):                                                                   [inefficient/tracer.py:15] |        |      0 |        |        |        |        |        |        |        |        |
| >>>     a = 0                                                                             [inefficient/tracer.py:16] |      1 |      0 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>     for i in range(n):                                                                [inefficient/tracer.py:17] |     24 |      0 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         a += i                                                                        [inefficient/tracer.py:18] |     23 |      0 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>>         for j in range(n):                                                            [inefficient/tracer.py:19] |    552 |      0 | wall   | sec    |  0.002 |  0.000 |  0.002 |  0.002 |  0.000 |  100.0 |
| >>>             a += j                                                                    [inefficient/tracer.py:20] |    529 |      0 | wall   | sec    |  0.002 |  0.000 |  0.002 |  0.002 |  0.000 |  100.0 |
| >>>     arr = np.arange(a * n * n * n, dtype=np.double)                                   [inefficient/tracer.py:21] |      1 |      0 | wall   | sec    |  0.202 |  0.202 |  0.202 |  0.202 |  0.000 |  100.0 |
| >>>     return arr.sum()                                                                  [inefficient/tracer.py:22] |      1 |      0 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> def _sum(a, axis=None, dtype=None, out=None, keepdims=False,                              [_sum/_methods.py:045] |        |      0 |        |        |        |        |        |        |        |        |
| >>>          initial=_NoValue, where=True):                                                   [_sum/_methods.py:046] |        |      0 |        |        |        |        |        |        |        |        |
| >>>     return umr_sum(a, axis, dtype, out, keepdims, initial, where)                         [_sum/_methods.py:047] |      1 |      0 | wall   | sec    |  0.029 |  0.029 |  0.029 |  0.029 |  0.000 |  100.0 |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|

```

### Command-line Trace (unmodified)

```console
$ python -m timemory.trace -c cpu_clock -d timemory-trace-output   -- ./solution/unmodified.py

[timemory]> tracing: ['solution/unmodified.py']
Running fibonacci...
Running inefficient...
Answer = 509443184046766.0
[cpu]|0> Outputting 'timemory-trace-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-trace-output/cpu.json'...
[cpu]|0> Outputting 'timemory-trace-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-trace-output/cpu.txt'...

|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                                               TOTAL CPU TIME SPENT IN BOTH USER- AND KERNEL-MODE                                                                               |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                        LABEL                                                         | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|----------------------------------------------------------------------------------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> import sys                                                                           [<module>/unmodified.py:03] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> import numpy as np                                                                   [<module>/unmodified.py:04] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> def fib(n):                                                                          [<module>/unmodified.py:07] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     return n if n < 2 else (fib(n - 1) + fib(n - 2))                                 [<module>/unmodified.py:08] |  21892 |      0 | cpu    | sec    |  0.080 |  0.000 |  0.080 |  0.080 |  0.000 |  100.0 |
| >>> def inefficient(n):                                                                  [<module>/unmodified.py:11] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     a = 0                                                                            [<module>/unmodified.py:12] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     for i in range(n):                                                               [<module>/unmodified.py:13] |     21 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>         a += i                                                                       [<module>/unmodified.py:14] |     20 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>         for j in range(n):                                                           [<module>/unmodified.py:15] |    420 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>             a += j                                                                   [<module>/unmodified.py:16] |    400 |      0 | cpu    | sec    |  0.010 |  0.000 |  0.010 |  0.010 |  0.000 |  100.0 |
| >>>     arr = np.arange(a * n * n * n, dtype=np.double)                                  [<module>/unmodified.py:17] |      1 |      0 | cpu    | sec    |  0.080 |  0.080 |  0.080 |  0.080 |  0.000 |  100.0 |
| >>>     return arr.sum()                                                                 [<module>/unmodified.py:18] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> def run(nfib):                                                                       [<module>/unmodified.py:21] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     ret = 0                                                                          [<module>/unmodified.py:22] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     print("Running fibonacci...")                                                    [<module>/unmodified.py:23] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     ret += fib(nfib)                                                                 [<module>/unmodified.py:24] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     ret += fib(nfib % 5 + 1)                                                         [<module>/unmodified.py:25] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     print("Running inefficient...")                                                  [<module>/unmodified.py:26] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     ret += inefficient(nfib)                                                         [<module>/unmodified.py:27] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     return ret                                                                       [<module>/unmodified.py:28] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> if __name__ == "__main__":                                                           [<module>/unmodified.py:31] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     nfib = int(sys.argv[1]) if len(sys.argv) > 1 else 20                             [<module>/unmodified.py:32] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     ans = run(nfib)                                                                  [<module>/unmodified.py:33] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     print("Answer = {}".format(ans))                                                 [<module>/unmodified.py:34] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> def _sum(a, axis=None, dtype=None, out=None, keepdims=False,                              [_sum/_methods.py:045] |        |      0 |        |        |        |        |        |        |        |        |
| >>>          initial=_NoValue, where=True):                                                   [_sum/_methods.py:046] |        |      0 |        |        |        |        |        |        |        |        |
| >>>     return umr_sum(a, axis, dtype, out, keepdims, initial, where)                         [_sum/_methods.py:047] |      1 |      0 | cpu    | sec    |  0.020 |  0.020 |  0.020 |  0.020 |  0.000 |  100.0 |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
```

### Command-line Profile (unmodified)

```console
$ python -m timemory.profiler -c cpu_clock -d timemory-profile-output -- ./solution/unmodified.py

[timemory]> profiling: ['solution/unmodified.py']
Running fibonacci...
Running inefficient...
Answer = 509443184046766.0
[cpu]|0> Outputting 'timemory-profile-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-profile-output/cpu.json'...
[cpu]|0> Outputting 'timemory-profile-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-profile-output/cpu.txt'...

|-----------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                     TOTAL CPU TIME SPENT IN BOTH USER- AND KERNEL-MODE                                                    |
|-----------------------------------------------------------------------------------------------------------------------------------------------------------|
|                             LABEL                               | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|-----------------------------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> run/unmodified.py:21                                        |      1 |      0 | cpu    | sec    |  0.390 |  0.390 |  0.390 |  0.390 |  0.000 |    2.6 |
| >>> |_run/unmodified.py:21                                      |      2 |      1 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_fib/unmodified.py:7                                       |      2 |      1 | cpu    | sec    |  0.280 |  0.140 |  0.000 |  0.280 |  0.198 |    0.0 |
| >>>   |_fib/unmodified.py:7                                     |      2 |      2 | cpu    | sec    |  0.280 |  0.140 |  0.080 |  0.200 |  0.085 |    0.0 |
| >>>     |_fib/unmodified.py:7                                   |      4 |      3 | cpu    | sec    |  0.280 |  0.070 |  0.030 |  0.150 |  0.054 |    0.0 |
| >>>       |_fib/unmodified.py:7                                 |      8 |      4 | cpu    | sec    |  0.280 |  0.035 |  0.010 |  0.120 |  0.035 |    0.0 |
| >>>         |_fib/unmodified.py:7                               |     16 |      5 | cpu    | sec    |  0.280 |  0.018 |  0.000 |  0.110 |  0.025 |    0.0 |
| >>>           |_fib/unmodified.py:7                             |     32 |      6 | cpu    | sec    |  0.280 |  0.009 |  0.000 |  0.070 |  0.014 |    0.0 |
| >>>             |_fib/unmodified.py:7                           |     64 |      7 | cpu    | sec    |  0.280 |  0.004 |  0.000 |  0.050 |  0.008 |    0.0 |
| >>>               |_fib/unmodified.py:7                         |    128 |      8 | cpu    | sec    |  0.280 |  0.002 |  0.000 |  0.040 |  0.005 |    0.0 |
| >>>                 |_fib/unmodified.py:7                       |    256 |      9 | cpu    | sec    |  0.280 |  0.001 |  0.000 |  0.020 |  0.003 |    0.0 |
| >>>                   |_fib/unmodified.py:7                     |    512 |     10 | cpu    | sec    |  0.280 |  0.001 |  0.000 |  0.010 |  0.002 |    0.0 |
| >>>                     |_fib/unmodified.py:7                   |   1024 |     11 | cpu    | sec    |  0.280 |  0.000 |  0.000 |  0.010 |  0.002 |    0.0 |
| >>>                       |_fib/unmodified.py:7                 |   2026 |     12 | cpu    | sec    |  0.280 |  0.000 |  0.000 |  0.010 |  0.001 |   21.4 |
| >>>                         |_fib/unmodified.py:7               |   3632 |     13 | cpu    | sec    |  0.220 |  0.000 |  0.000 |  0.010 |  0.001 |   40.9 |
| >>>                           |_fib/unmodified.py:7             |   5020 |     14 | cpu    | sec    |  0.130 |  0.000 |  0.000 |  0.010 |  0.001 |   46.2 |
| >>>                             |_fib/unmodified.py:7           |   4760 |     15 | cpu    | sec    |  0.070 |  0.000 |  0.000 |  0.010 |  0.000 |   28.6 |
| >>>                               |_fib/unmodified.py:7         |   2942 |     16 | cpu    | sec    |  0.050 |  0.000 |  0.000 |  0.010 |  0.000 |   80.0 |
| >>>                                 |_fib/unmodified.py:7       |   1152 |     17 | cpu    | sec    |  0.010 |  0.000 |  0.000 |  0.010 |  0.000 |  100.0 |
| >>>                                   |_fib/unmodified.py:7     |    274 |     18 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                     |_fib/unmodified.py:7   |     36 |     19 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                       |_fib/unmodified.py:7 |      2 |     20 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_inefficient/unmodified.py:11                              |      1 |      1 | cpu    | sec    |  0.100 |  0.100 |  0.100 |  0.100 |  0.000 |    0.0 |
| >>>   |_inefficient/unmodified.py:11                            |      2 |      2 | cpu    | sec    |  0.100 |  0.050 |  0.010 |  0.090 |  0.057 |   90.0 |
| >>>     |__sum/_methods.py:45                                   |      1 |      3 | cpu    | sec    |  0.010 |  0.010 |  0.010 |  0.010 |  0.000 |    0.0 |
| >>>       |__sum/_methods.py:45                                 |      1 |      4 | cpu    | sec    |  0.010 |  0.010 |  0.010 |  0.010 |  0.000 |  100.0 |
|-----------------------------------------------------------------------------------------------------------------------------------------------------------|
```

### Command-line Trace with `@profile`

```console
$ python -m timemory.trace -c cpu_clock -b -d timemory-builtin-trace-output -- ./solution/with-builtins.py

[timemory]> tracing: ['solution/with-builtins.py']
Answer = 509443184046766.0
[cpu]|0> Outputting 'timemory-builtin-trace-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-builtin-trace-output/cpu.json'...
[cpu]|0> Outputting 'timemory-builtin-trace-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-builtin-trace-output/cpu.txt'...

|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                                               TOTAL CPU TIME SPENT IN BOTH USER- AND KERNEL-MODE                                                                               |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                        LABEL                                                         | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|----------------------------------------------------------------------------------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> @profile                                                                               [fib/with-builtins.py:07] |        |      0 |        |        |        |        |        |        |        |        |
| >>> def fib(n):                                                                            [fib/with-builtins.py:08] |        |      0 |        |        |        |        |        |        |        |        |
| >>>     return n if n < 2 else (fib(n - 1) + fib(n - 2))                                   [fib/with-builtins.py:09] |  21892 |      0 | cpu    | sec    |  0.090 |  0.000 |  0.090 |  0.090 |  0.000 |  100.0 |
| >>> @profile                                                                               [fib/with-builtins.py:12] |        |      0 |        |        |        |        |        |        |        |        |
| >>> def inefficient(n):                                                                    [fib/with-builtins.py:13] |        |      0 |        |        |        |        |        |        |        |        |
| >>>     a = 0                                                                              [fib/with-builtins.py:14] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     for i in range(n):                                                                 [fib/with-builtins.py:15] |     21 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>         a += i                                                                         [fib/with-builtins.py:16] |     20 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>         for j in range(n):                                                             [fib/with-builtins.py:17] |    420 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>             a += j                                                                     [fib/with-builtins.py:18] |    400 |      0 | cpu    | sec    |  0.010 |  0.000 |  0.010 |  0.010 |  0.000 |  100.0 |
| >>>     arr = np.arange(a * n * n * n, dtype=np.double)                                    [fib/with-builtins.py:19] |      1 |      0 | cpu    | sec    |  0.080 |  0.080 |  0.080 |  0.080 |  0.000 |  100.0 |
| >>>     return arr.sum()                                                                   [fib/with-builtins.py:20] |      1 |      0 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> def _sum(a, axis=None, dtype=None, out=None, keepdims=False,                              [_sum/_methods.py:045] |        |      0 |        |        |        |        |        |        |        |        |
| >>>          initial=_NoValue, where=True):                                                   [_sum/_methods.py:046] |        |      0 |        |        |        |        |        |        |        |        |
| >>>     return umr_sum(a, axis, dtype, out, keepdims, initial, where)                         [_sum/_methods.py:047] |      1 |      0 | cpu    | sec    |  0.010 |  0.010 |  0.010 |  0.010 |  0.000 |  100.0 |
|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
```

### Command-line Profile with `@profile`

```console
$ python -m timemory.profiler -c cpu_clock -b -d timemory-builtin-profile-output -- ./solution/with-builtins.py

[timemory]> profiling: ['solution/with-builtins.py']
Answer = 509443184046766.0
[cpu]|0> Outputting 'timemory-builtin-profile-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-builtin-profile-output/cpu.json'...
[cpu]|0> Outputting 'timemory-builtin-profile-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-builtin-profile-output/cpu.txt'...

|------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                     TOTAL CPU TIME SPENT IN BOTH USER- AND KERNEL-MODE                                                     |
|------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                              LABEL                               | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|------------------------------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> fib/with-builtins.py:7                                       |      2 |      0 | cpu    | sec    |  0.490 |  0.245 |  0.000 |  0.490 |  0.346 |    0.0 |
| >>> |_fib/with-builtins.py:7                                     |      2 |      1 | cpu    | sec    |  0.490 |  0.245 |  0.170 |  0.320 |  0.106 |    0.0 |
| >>>   |_fib/with-builtins.py:7                                   |      4 |      2 | cpu    | sec    |  0.490 |  0.122 |  0.070 |  0.230 |  0.073 |    0.0 |
| >>>     |_fib/with-builtins.py:7                                 |      8 |      3 | cpu    | sec    |  0.490 |  0.061 |  0.020 |  0.160 |  0.043 |    0.0 |
| >>>       |_fib/with-builtins.py:7                               |     16 |      4 | cpu    | sec    |  0.490 |  0.031 |  0.010 |  0.120 |  0.026 |    0.0 |
| >>>         |_fib/with-builtins.py:7                             |     32 |      5 | cpu    | sec    |  0.490 |  0.015 |  0.000 |  0.100 |  0.017 |    0.0 |
| >>>           |_fib/with-builtins.py:7                           |     64 |      6 | cpu    | sec    |  0.490 |  0.008 |  0.000 |  0.070 |  0.010 |    2.0 |
| >>>             |_fib/with-builtins.py:7                         |    128 |      7 | cpu    | sec    |  0.480 |  0.004 |  0.000 |  0.050 |  0.007 |    0.0 |
| >>>               |_fib/with-builtins.py:7                       |    256 |      8 | cpu    | sec    |  0.480 |  0.002 |  0.000 |  0.030 |  0.004 |    4.2 |
| >>>                 |_fib/with-builtins.py:7                     |    512 |      9 | cpu    | sec    |  0.460 |  0.001 |  0.000 |  0.020 |  0.003 |    4.3 |
| >>>                   |_fib/with-builtins.py:7                   |   1024 |     10 | cpu    | sec    |  0.440 |  0.000 |  0.000 |  0.010 |  0.002 |   13.6 |
| >>>                     |_fib/with-builtins.py:7                 |   2026 |     11 | cpu    | sec    |  0.380 |  0.000 |  0.000 |  0.010 |  0.001 |    7.9 |
| >>>                       |_fib/with-builtins.py:7               |   3632 |     12 | cpu    | sec    |  0.350 |  0.000 |  0.000 |  0.010 |  0.001 |   31.4 |
| >>>                         |_fib/with-builtins.py:7             |   5020 |     13 | cpu    | sec    |  0.240 |  0.000 |  0.000 |  0.010 |  0.001 |   45.8 |
| >>>                           |_fib/with-builtins.py:7           |   4760 |     14 | cpu    | sec    |  0.130 |  0.000 |  0.000 |  0.010 |  0.001 |   76.9 |
| >>>                             |_fib/with-builtins.py:7         |   2942 |     15 | cpu    | sec    |  0.030 |  0.000 |  0.000 |  0.010 |  0.000 |    0.0 |
| >>>                               |_fib/with-builtins.py:7       |   1152 |     16 | cpu    | sec    |  0.030 |  0.000 |  0.000 |  0.010 |  0.001 |  100.0 |
| >>>                                 |_fib/with-builtins.py:7     |    274 |     17 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                   |_fib/with-builtins.py:7   |     36 |     18 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                     |_fib/with-builtins.py:7 |      2 |     19 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> inefficient/with-builtins.py:12                              |      1 |      0 | cpu    | sec    |  0.090 |  0.090 |  0.090 |  0.090 |  0.000 |    0.0 |
| >>> |_inefficient/with-builtins.py:12                            |      2 |      1 | cpu    | sec    |  0.090 |  0.045 |  0.020 |  0.070 |  0.035 |   77.8 |
| >>>   |__sum/_methods.py:45                                      |      1 |      2 | cpu    | sec    |  0.020 |  0.020 |  0.020 |  0.020 |  0.000 |    0.0 |
| >>>     |__sum/_methods.py:45                                    |      1 |      3 | cpu    | sec    |  0.020 |  0.020 |  0.020 |  0.020 |  0.000 |  100.0 |
|------------------------------------------------------------------------------------------------------------------------------------------------------------|
```

### Command-line Line Profiler with `@profile`

```console
$ python -m timemory.line_profiler -c cpu_clock -v -- ./solution/with-builtins.py

Answer = 509443184046766.0
Wrote profile results to with-builtins.py.prof
Metric unit: 1 sec

Total metric: 0.04 sec
File: ./solution/with-builtins.py
Function: fib at line 7

Line #      Hits       Metric  Per Hit % Metric  Line Contents
==============================================================
     7                                           @profile
     8                                           def fib(n):
     9    218912          0.0      0.0    100.0      return n if n < 2 else (fib(n - 1) + fib(n - 2))

Total metric: 0.09 sec
File: ./solution/with-builtins.py
Function: inefficient at line 12

Line #      Hits       Metric  Per Hit % Metric  Line Contents
==============================================================
    12                                           @profile
    13                                           def inefficient(n):
    14         1          0.0      0.0      0.0      a = 0
    15        21          0.0      0.0      0.0      for i in range(n):
    16        20          0.0      0.0      0.0          a += i
    17       420          0.0      0.0      0.0          for j in range(n):
    18       400          0.0      0.0      0.0              a += j
    19         1          0.1      0.1    100.0      arr = np.arange(a * n * n * n, dtype=np.double)
    20         5          0.0      0.0      0.0      return arr.sum()
```
