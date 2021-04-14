# python overview

This example walks through a generic overview of capabilities, functionalities and settings available in timemory's Python package. Specifically, we will discuss the different sub-packages, bundles, modules, plotting, roofline, settings and environment variable interactions that are key to getting started with timemory's Python interface. The settings controlling timemory's behavior can be set either directly via `timemory.settings` or using environment variables. The component subpackage contains the individual components that can be instantiated and then used as per user requirements. The bundles and profiler sub-package contains scoped utilities and profiler interface that can be used either as decorators or context managers. The generic data plotting (such as plotting instrumentation graphs) and roofline plotting are available in subsequent sub-packages.

## Settings

```python
import json
import os
import timemory

timemory.settings.verbose = 1
timemory.settings.debug = False
timemory.settings.json_output = True
timemory.settings.mpi_thread  = False
timemory.settings.dart_output = True
timemory.settings.dart_count = 1
timemory.settings.banner = False
```

## Environment Variables

```python
import os

os.environ["TIMEMORY_FLAT_PROFILE"] = "ON"
os.environ["TIMEMORY_TIMELINE_PROFILE"] = "ON"
timemory.settings.parse()
```

## Decorators

```python
from timemory.bundle import marker

@marker(["cpu_clock", "peak_rss"])
def fibonacci(n=20):
    return n if n < 2 else (fibonacci(n-1) + fibonacci(n-2))
```

## Context Managers

```python
from timemory.profiler import profile

def runner(n=20):
    with profile(["wall_clock", "cpu_util"]):
        fibonacci(n)
```

## Individual Components

```python
from timemory.component import WallClock

wc = WallClock("wall")
wc.start()
answer = fibonacci(25)
wc.stop()
data = wc.get()
print(data)
```

## Argument Parser Support

Add in a subparser (`timemory-config`) for `argparse.ArgumentParser` which enables
command-line argument support for a subset of timemory settings.

### Python

```python
import argparse

parser = argparse.ArgumentParser("example")
parser.add_argument(...)
timemory.add_arguments(parser)

args = parser.parse_args()
```

### Usage

```console
./script.py <user-options> timemory-config <timemory-options>
./script.py -n 10 timemory-config --enabled False
```

### Python Function Profiling

Function profiler is available through decorators, context-managers, or command-line.

```python
from timemory.profiler import profile

@profile(["wall_clock"])
def foo():
    # ...

def bar():
    with profile(["wall_clock"]):
        import time
        time.sleep(2)
```

When using the command-line tools, the components are selected via command-line options so a
bare `@profile` is all that is required/recommended.

```python
@profile
def foo():
    # ...

@profile
def bar():
    import time
    time.sleep(2)
```

In order to differentiate timemory arguments from script arguments in command-line tools, whenever
a script has arguments, provide the script and the script arguments after a double-hyphen (`--`)
on the command line.

> Note: `python -m timemory.profiler` and `timemory-python-profiler` can be used interchangeably below.

```console
python -m timemory.profiler <script>
python -m timemory.profiler -- <script> <script-arguments>
python -m timemory.profiler <options> -- <script> <script-arguments>
timemory-python-profiler <options> -- <script> <script-arguments>
```

### Python Line Profiling

Timemory provides two line-profilers. The first, located in `timemory.trace`,
is very similar to the function profiler above. The second is the popular python `line_profiler`
package extended to use timemory internally for data collection. Due to `line_profiler` being
designed for scalar values, the `timemory.line_profiler` only supports the subset of timemory
components whose `get()` member function returns a scalar value, e.g. `cpu_clock` and `peak_rss`
are supported because these components return floating-point values whereas `current_peak_rss`
is not supported because it returns a pair of scalar values (the `peak_rss` measurement at the
`start()` and at `stop()`).

```python
from timemory.trace import trace

@trace(["wall_clock"])
def foo():
    # ...

def bar():
    with trace(["wall_clock"]):
        import time
        time.sleep(2)
```

When using the command-line tools, the components are selected via command-line options so a
bare `@trace` or bare `@profile` is all that is required/recommended. `timemory.trace` supports
both the `@trace` and `@profile` built-ins. `timemory.line_profiler` only supports `@profile`.

```python
@profile
def foo():
    # ...

@profile
def bar():
    import time
    time.sleep(2)
```

> Note: `python -m timemory.trace` and `timemory-python-trace` can be used interchangeably below.

```console
python -m timemory.trace <script>
python -m timemory.trace -- <script> <script-arguments>
python -m timemory.trace <options> -- <script> <script-arguments>
```

> Note: `python -m timemory.line_profiler` and `timemory-python-line-profiler` can be used interchangeably below.

```console
python -m timemory.line_profiler <script>
python -m timemory.line_profiler -- <script> <script-arguments>
python -m timemory.line_profiler <options> -- <script> <script-arguments>
timemory-python-line-profiler <options> -- <script> <script-arguments>
```

## Component Storage

When the decorators or context-managers are used to collect data or individual components
are used while calling `push()` and `pop()` on each instance, timemory will maintain
aggregates of all the measurements according to various settings. You can access this
data programmatically without I/O.

```python
from timemory.storage import WallClockStorage

# data for current rank
data = WallClockStorage.get()
# combined data on rank zero but all ranks must call it
dmp_data = WallClockStorage.dmp_get()
```
