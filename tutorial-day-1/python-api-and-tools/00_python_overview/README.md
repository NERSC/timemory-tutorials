# python overview

This example walks through a generic overview of capabilities, functionalities and settings available in timemory's Python package. Specifically, we will discuss the different sub-packages, bundles, modules, plotting, roofline, settings and environment variable interactions that are key to getting started with timemory's Python interface. The settings controlling timemory's behavior can be set either directly via `timemory.settings` or using environment variables. The component subpackage contains the individual components that can be instantiated and then used as per user requirements. The bundles and profiler sub-package contains scoped utilities and profiler interface that can be used either as decorators or context managers. The generic data plotting (such as plotting instrumentation graphs) and roofline plotting are available in subsequent sub-packages.

## Examples

The following code snippets demonstrate some of the key basic features of timemory package

### Settings

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

### Environment Variables

```python
import os

os.environ["TIMEMORY_FLAT_PROFILE"] = "ON"
os.environ["TIMEMORY_TIMELINE_PROFILE"] = "ON"
timemory.settings.parse()
```

### Decorators

```python
from timemory.bundle import marker

@marker(["cpu_clock", "peak_rss"])
def fibonacci(n=20):
    return n if n < 2 else (fibonacci(n-1) + fibonacci(n-2))
```

### Context Managers

```python
from timemory.profiler import profile

def runner(n=20):
    with profile(["wall_clock", "cpu_util"]):
        fibonacci(n)
```

### Individual Components

```python
from timemory.component import WallClock

wc = WallClock("wall")
wc.start()
answer = fibonacci(25)
wc.stop()
data = wc.get()
print(data)
```

### Roofline
This module contains the routines required for roofline plotting.
