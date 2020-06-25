# python_profiling

This example walks through the usage of profiler sub-package available in timemory Python package. The `profiler` can be used as decorator, context manager or an object. The `profiler` takes a user-provided list of timemory components as argument which are used for profiling. Further, the *flat* and *timeline* profiling modes can be controlled by providing `flat` and `timeline` arguments to the profiler or by setting `TIMEMORY_FLAT_PROFILE` and `TIMEMORY_TIMELINE_PROFILE` environment variables or by setting/unsetting `timemory.settings.flat_profile` and `timemory.settings.timeline_profile` respectively.

## Usage
The following code snippets demonstrates the use of `profiler` as decorator, context manager and object. In all cases, we will profile the `fibonacci(nfib=18)` function using "wall_clock" and "peak_rss" compoenents.

**Decorator**
```
from timemory.profiler import profile

@profile(["wall_clock", "peak_rss"], flat=False, timeline=False)
def profile_decorator(nfib=3):
    return fibonacci(nfib)
```
**Context Manager**
```
def main():
    with profile(["wall_clock", "peak_rss"], flat=False, timeline=False):
        ans = fibonacci(nfib=3)
```
**Object**
```
def main():
    timemory.settings.flat_profile = False
    timemory.settings.timeline_profile = False
    prof = profile(["wall_clock", "peak_rss"])
    prof.start()
    profile_object(nfib=3)
    return
```
## Example
Profile fibonacci(n=3) function as decorator, context manager and as object using "wall_clock" and "peak_rss" components.
```
$ python profile_fib.py
#--------------------- tim::manager initialized [rank=0][id=0][pid=2442] ---------------------#

USE = True, COMPONENTS = ['wall_clock', 'peak_rss', '']
USE = True, COMPONENTS = ['wall_clock', 'peak_rss', '']
Answer = 2
USE = True, COMPONENTS = ['wall_clock', 'peak_rss', '']
Answer = 2
[wall]|0> Outputting 'timemory-ex-python-profiler-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-ex-python-profiler-output/wall.json'...
[wall]|0> Outputting 'timemory-ex-python-profiler-output/wall.txt'...
Opening 'timemory-ex-python-profiler-output/wall.jpeg' for output...
Closed 'timemory-ex-python-profiler-output/wall.jpeg'...

|--------------------------------------------------------------------------------------------------------------------------------------|
|                                               REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                               |
|--------------------------------------------------------------------------------------------------------------------------------------|
|                   LABEL                    | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|--------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> profile_decorate@ex_python_profiler:12 |      1 |      0 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   26.1 |
| >>> |_fib@ex_python_profiler:8             |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   35.3 |
| >>>   |_fib@ex_python_profiler:8           |      2 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   78.9 |
| >>>     |_fib@ex_python_profiler:8         |      2 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> fib@ex_python_profiler:8               |      1 |      0 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   48.3 |
| >>> |_fib@ex_python_profiler:8             |      2 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   83.7 |
| >>>   |_fib@ex_python_profiler:8           |      2 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> profile_object@ex_python_profiler:16   |      1 |      0 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   20.9 |
| >>> |_fib@ex_python_profiler:8             |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   36.8 |
| >>>   |_fib@ex_python_profiler:8           |      2 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   81.0 |
|--------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>>     |_fib@ex_python_profiler:8         |      2 |      3 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
|--------------------------------------------------------------------------------------------------------------------------------------|

[peak_rss]|0> Outputting 'timemory-ex-python-profiler-output/peak_rss.json'...
[peak_rss]|0> Outputting 'timemory-ex-python-profiler-output/peak_rss.txt'...
Opening 'timemory-ex-python-profiler-output/peak_rss.jpeg' for output...
Closed 'timemory-ex-python-profiler-output/peak_rss.jpeg'...

|----------------------------------------------------------------------------------------------------------------------------------------|
|           MEASURES CHANGES IN THE HIGH-WATER MARK FOR THE AMOUNT OF MEMORY ALLOCATED IN RAM. MAY FLUCTUATE IF SWAP IS ENABLED          |
|----------------------------------------------------------------------------------------------------------------------------------------|
|                   LABEL                    | COUNT  | DEPTH  | METRIC   | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|--------------------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>> profile_decorate@ex_python_profiler:12 |      1 |      0 | peak_rss | MB     |  0.448 |  0.448 |  0.448 |  0.448 |  0.000 |  100.0 |
| >>> |_fib@ex_python_profiler:8             |      1 |      1 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_fib@ex_python_profiler:8           |      2 |      2 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     |_fib@ex_python_profiler:8         |      2 |      3 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> fib@ex_python_profiler:8               |      1 |      0 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_fib@ex_python_profiler:8             |      2 |      1 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_fib@ex_python_profiler:8           |      2 |      2 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> profile_object@ex_python_profiler:16   |      1 |      0 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_fib@ex_python_profiler:8             |      1 |      1 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_fib@ex_python_profiler:8           |      2 |      2 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|--------------------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>>     |_fib@ex_python_profiler:8         |      2 |      3 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|----------------------------------------------------------------------------------------------------------------------------------------|


[metadata::manager::finalize]> Outputting 'timemory-ex-python-profiler-output/metadata.json'...


#---------------------- tim::manager destroyed [rank=0][id=0][pid=2442] ----------------------#
```