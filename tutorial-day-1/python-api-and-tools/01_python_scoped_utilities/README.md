# Python Scoped Utilities

This example walks through the scoped utilities available in timemory Python package. These utilities include `auto_timer`, `marker`, `rss_usage` and `timer`, and provide instrumentation capabilities either as decorators or context managers. The `marker` utility allows instrumentation using a custom user-provided list of timemory components whereas the `auto_timer` and `rss_usage` utilities use timemory components relevant to timing, and rss measurements respectively. The following code snippets demonstrates the use of `timemory.util.marker` utility as decorator and context manager respectively. In both cases, the marker uses "wall_clock" and "peak_rss" components components for instrumentation.

## Decorator

```python
from timemory.util import marker

@marker(components=("wall_clock", timemory.component.peak_rss))
def fib_decorator(n=35):
    return fibonacci(n)
```

## Context Manager

```python
def main():
    with marker(get_config(["wall_clock", "peak_rss"]), key="marker_ctx_manager"):
        fibonacci(35)
```

## Starting code

* TODO: write explanation

```python
def fib(n):
    return n if n < 2 else (fib(n-1) + fib(n-2))

def run(n):
    ''' Decorator and context manager for high-level pre-defined collection '''
    fib(n)

if __name__ == "__main__":
    n = int(sys.argv[1] if len(sys.argv) > 1 else 10)
    run(n)
```

## Exercise

* Import timemory

```python
import timemory
from timemory.util import marker
```

* Add timemory finalization at the end of main

```python
timemory.finalize()
```

* Use decorator either fib or run function with a marker
* The first argument for a marker should be a list of components

```python
@marker(["wall_clock"])
def run(n):
    # ...
```

* Execute code

```console
./basic.py 15
```

## Example Output

Instrument fibonacci(n=35) function using `marker` both as decorator and context manager using "wall_clock" and "peak_rss" components.

```console
$ python3.7 fibonacci_example.py
#--------------------- tim::manager initialized [rank=0][id=0][pid=13417] ---------------------#

[wall]|0> Outputting 'timemory-fibonacci-example-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-fibonacci-example-output/wall.json'...
[wall]|0> Outputting 'timemory-fibonacci-example-output/wall.txt'...
Opening 'timemory-fibonacci-example-output/wall.jpeg' for output...
Closed 'timemory-fibonacci-example-output/wall.jpeg'...

|------------------------------------------------------------------------------------------------------------------|
|                                     REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                     |
|------------------------------------------------------------------------------------------------------------------|
|         LABEL          | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> fib_decorator      |      1 |      0 | wall   | sec    |  3.040 |  3.040 |  3.040 |  3.040 |  0.000 |  100.0 |
| >>> marker_ctx_manager |      1 |      0 | wall   | sec    |  3.034 |  3.034 |  3.034 |  3.034 |  0.000 |  100.0 |
|------------------------------------------------------------------------------------------------------------------|

[peak_rss]|0> Outputting 'timemory-fibonacci-example-output/peak_rss.json'...
[peak_rss]|0> Outputting 'timemory-fibonacci-example-output/peak_rss.txt'...
Opening 'timemory-fibonacci-example-output/peak_rss.jpeg' for output...
Closed 'timemory-fibonacci-example-output/peak_rss.jpeg'...

|--------------------------------------------------------------------------------------------------------------------|
| MEASURES CHANGES IN THE HIGH-WATER MARK FOR THE AMOUNT OF MEMORY ALLOCATED IN RAM. MAY FLUCTUATE IF SWAP IS ENABLED|
|--------------------------------------------------------------------------------------------------------------------|
|         LABEL          | COUNT  | DEPTH  | METRIC   | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>> fib_decorator      |      1 |      0 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> marker_ctx_manager |      1 |      0 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|--------------------------------------------------------------------------------------------------------------------|


[metadata::manager::finalize]> Outputting 'timemory-fibonacci-example-output/metadata.json'...
```
