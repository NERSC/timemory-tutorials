# Overview

Here we'll outline and demo the Python profiling capabilities of timemory. 

Note that tracing in timemory is also possible. We have several tracing
exercises at the bottom of this page. If you would like to see what it looks
like to use timemory tracing in a Python program please see [here](solutions).

## Profiling

First we'll talk about profiling. Let's first take a look at our example.py.
Note that right now there is no instrumentation -- it looks like a normal python
script. 

### Profiling via the command line

Now let's try profiling via the command line:

```
python -m timemory.profiler -d timemory-profile-output -- ./example.py
```

where `-d` tells timemory where to put the output files and [example.py](example.py) is the script we'd like to profile.

Let's take a look at our output:

```
stephey@cori06:~> python -m timemory.profiler -d timemory-profile-output -- ./example.py
[timemory]> profiling: ['/global/u1/s/stephey/example.py']
Running fibonacci...
Running inefficient...
Answer = 509443184046766.0
[wall]|0> Outputting 'timemory-profile-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-profile-output/wall.json'...
[wall]|0> Outputting 'timemory-profile-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-profile-output/wall.txt'...

|--------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                        REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                                        |
|--------------------------------------------------------------------------------------------------------------------------------------------------------|
|                            LABEL                             | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|--------------------------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> run/example.py:19                                        |      1 |      0 | wall   | sec    |  0.353 |  0.353 |  0.353 |  0.353 |  0.000 |    3.3 |
| >>> |_run/example.py:19                                      |      2 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_fib/example.py:5                                       |      2 |      1 | wall   | sec    |  0.187 |  0.093 |  0.187 |  0.187 |  0.132 |    0.0 |
| >>>   |_fib/example.py:5                                     |      2 |      2 | wall   | sec    |  0.187 |  0.093 |  0.116 |  0.116 |  0.033 |    0.0 |
| >>>     |_fib/example.py:5                                   |      4 |      3 | wall   | sec    |  0.187 |  0.047 |  0.073 |  0.073 |  0.019 |    0.0 |
| >>>       |_fib/example.py:5                                 |      8 |      4 | wall   | sec    |  0.187 |  0.023 |  0.046 |  0.046 |  0.011 |    0.1 |
| >>>         |_fib/example.py:5                               |     16 |      5 | wall   | sec    |  0.187 |  0.012 |  0.029 |  0.029 |  0.006 |    0.1 |
| >>>           |_fib/example.py:5                             |     32 |      6 | wall   | sec    |  0.186 |  0.006 |  0.018 |  0.018 |  0.004 |    0.2 |
| >>>             |_fib/example.py:5                           |     64 |      7 | wall   | sec    |  0.186 |  0.003 |  0.012 |  0.012 |  0.002 |    0.5 |
| >>>               |_fib/example.py:5                         |    128 |      8 | wall   | sec    |  0.185 |  0.001 |  0.008 |  0.008 |  0.001 |    0.9 |
| >>>                 |_fib/example.py:5                       |    256 |      9 | wall   | sec    |  0.183 |  0.001 |  0.005 |  0.005 |  0.001 |    1.9 |
| >>>                   |_fib/example.py:5                     |    512 |     10 | wall   | sec    |  0.180 |  0.000 |  0.003 |  0.003 |  0.000 |    3.8 |
| >>>                     |_fib/example.py:5                   |   1024 |     11 | wall   | sec    |  0.173 |  0.000 |  0.002 |  0.002 |  0.000 |    8.1 |
| >>>                       |_fib/example.py:5                 |   2026 |     12 | wall   | sec    |  0.159 |  0.000 |  0.001 |  0.001 |  0.000 |   15.7 |
| >>>                         |_fib/example.py:5               |   3632 |     13 | wall   | sec    |  0.134 |  0.000 |  0.001 |  0.001 |  0.000 |   28.0 |
| >>>                           |_fib/example.py:5             |   5020 |     14 | wall   | sec    |  0.097 |  0.000 |  0.001 |  0.001 |  0.000 |   42.7 |
| >>>                             |_fib/example.py:5           |   4760 |     15 | wall   | sec    |  0.055 |  0.000 |  0.001 |  0.001 |  0.000 |   57.1 |
| >>>                               |_fib/example.py:5         |   2942 |     16 | wall   | sec    |  0.024 |  0.000 |  0.000 |  0.000 |  0.000 |   68.8 |
| >>>                                 |_fib/example.py:5       |   1152 |     17 | wall   | sec    |  0.007 |  0.000 |  0.000 |  0.000 |  0.000 |   76.9 |
| >>>                                   |_fib/example.py:5     |    274 |     18 | wall   | sec    |  0.002 |  0.000 |  0.000 |  0.000 |  0.000 |   73.3 |
| >>>                                     |_fib/example.py:5   |     36 |     19 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   96.0 |
| >>>                                       |_fib/example.py:5 |      2 |     20 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_inefficient/example.py:9                               |      1 |      1 | wall   | sec    |  0.155 |  0.155 |  0.155 |  0.155 |  0.000 |    0.1 |
| >>>   |_inefficient/example.py:9                             |      2 |      2 | wall   | sec    |  0.155 |  0.077 |  0.133 |  0.133 |  0.078 |   85.8 |
| >>>     |__sum/_methods.py:36                                |      1 |      3 | wall   | sec    |  0.022 |  0.022 |  0.022 |  0.022 |  0.000 |    0.1 |
| >>>       |__sum/_methods.py:36                              |      1 |      4 | wall   | sec    |  0.022 |  0.022 |  0.022 |  0.022 |  0.000 |  100.0 |
|--------------------------------------------------------------------------------------------------------------------------------------------------------|
```

This will generate profiling data stored in several files in the `timemory-profile-output` directory and also produce some output to stdout.

### Profiling using decorators inside our scripts

Now let's try profiling by using decorators in our script instead.

You can take a look at [example-profile-decorators.py](example-profile-decorators.py) which already has
`@profile(["wall_clock"])` decorators added to the `fib` and `inefficient`
functions. Note that we also had to import `from timemory.profiler import
profile` at the top of our script. Now let's profile using our decorators.

```
python example-profile-decorators.py
```

The output should look like:

```
stephey@cori06:/global/cscratch1/sd/stephey/timemory-tutorials/tutorial-day-1/python-api-and-tools> python example-profile-decorators.py 
Running fibonacci...
Running inefficient...
Answer = 509443184046766.0
[wall]|0> Outputting 'timemory-example-profile-decorators-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-example-profile-decorators-output/wall.json'...
[wall]|0> Outputting 'timemory-example-profile-decorators-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-example-profile-decorators-output/wall.txt'...

|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                                 REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                                                |
|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                    LABEL                                      | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|-------------------------------------------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> fib/example-profile-decorators.py:5                                       |      2 |      0 | wall   | sec    |  0.440 |  0.220 |  0.440 |  0.440 |  0.311 |    0.0 |
| >>> |_fib/example-profile-decorators.py:5                                     |      2 |      1 | wall   | sec    |  0.440 |  0.220 |  0.276 |  0.276 |  0.079 |    0.0 |
| >>>   |_fib/example-profile-decorators.py:5                                   |      4 |      2 | wall   | sec    |  0.440 |  0.110 |  0.167 |  0.167 |  0.042 |    0.0 |
| >>>     |_fib/example-profile-decorators.py:5                                 |      8 |      3 | wall   | sec    |  0.440 |  0.055 |  0.104 |  0.104 |  0.024 |    0.1 |
| >>>       |_fib/example-profile-decorators.py:5                               |     16 |      4 | wall   | sec    |  0.439 |  0.027 |  0.064 |  0.064 |  0.014 |    0.1 |
| >>>         |_fib/example-profile-decorators.py:5                             |     32 |      5 | wall   | sec    |  0.439 |  0.014 |  0.039 |  0.039 |  0.008 |    0.3 |
| >>>           |_fib/example-profile-decorators.py:5                           |     64 |      6 | wall   | sec    |  0.438 |  0.007 |  0.025 |  0.025 |  0.004 |    0.5 |
| >>>             |_fib/example-profile-decorators.py:5                         |    128 |      7 | wall   | sec    |  0.435 |  0.003 |  0.015 |  0.015 |  0.002 |    1.0 |
| >>>               |_fib/example-profile-decorators.py:5                       |    256 |      8 | wall   | sec    |  0.431 |  0.002 |  0.010 |  0.010 |  0.001 |    2.1 |
| >>>                 |_fib/example-profile-decorators.py:5                     |    512 |      9 | wall   | sec    |  0.422 |  0.001 |  0.006 |  0.006 |  0.001 |    4.4 |
| >>>                   |_fib/example-profile-decorators.py:5                   |   1024 |     10 | wall   | sec    |  0.403 |  0.000 |  0.004 |  0.004 |  0.000 |    8.9 |
| >>>                     |_fib/example-profile-decorators.py:5                 |   2026 |     11 | wall   | sec    |  0.367 |  0.000 |  0.003 |  0.003 |  0.000 |   18.0 |
| >>>                       |_fib/example-profile-decorators.py:5               |   3632 |     12 | wall   | sec    |  0.301 |  0.000 |  0.001 |  0.001 |  0.000 |   31.4 |
| >>>                         |_fib/example-profile-decorators.py:5             |   5020 |     13 | wall   | sec    |  0.207 |  0.000 |  0.001 |  0.001 |  0.000 |   46.3 |
| >>>                           |_fib/example-profile-decorators.py:5           |   4760 |     14 | wall   | sec    |  0.111 |  0.000 |  0.001 |  0.001 |  0.000 |   59.9 |
| >>>                             |_fib/example-profile-decorators.py:5         |   2942 |     15 | wall   | sec    |  0.044 |  0.000 |  0.000 |  0.000 |  0.000 |   73.7 |
| >>>                               |_fib/example-profile-decorators.py:5       |   1152 |     16 | wall   | sec    |  0.012 |  0.000 |  0.000 |  0.000 |  0.000 |   82.4 |
| >>>                                 |_fib/example-profile-decorators.py:5     |    274 |     17 | wall   | sec    |  0.002 |  0.000 |  0.000 |  0.000 |  0.000 |   89.8 |
| >>>                                   |_fib/example-profile-decorators.py:5   |     36 |     18 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   95.0 |
| >>>                                     |_fib/example-profile-decorators.py:5 |      2 |     19 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> inefficient/example-profile-decorators.py:9                               |      1 |      0 | wall   | sec    |  0.153 |  0.153 |  0.153 |  0.153 |  0.000 |    0.1 |
| >>> |_inefficient/example-profile-decorators.py:9                             |      2 |      1 | wall   | sec    |  0.153 |  0.077 |  0.130 |  0.130 |  0.076 |   85.0 |
| >>>   |__sum/_methods.py:36                                                   |      1 |      2 | wall   | sec    |  0.023 |  0.023 |  0.023 |  0.023 |  0.000 |    0.1 |
| >>>     |__sum/_methods.py:36                                                 |      1 |      3 | wall   | sec    |  0.023 |  0.023 |  0.023 |  0.023 |  0.000 |  100.0 |
|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
```

### Profiling using context managers

Now let's try profiling using context managers rather than decorators. You can
take a look at [example-profile-context.py](example-profile-context.py). We still have the same import at
the top of our script: `from timemory.profiler import profile`. We have added
context mangers to our `fib` and `inefficient`, for example: `with
profile(["wall_clock"])` for the `fib` function.

Now let's run our program:

```
python example-profile-context.py
```

The output should look like:

```
stephey@cori06:/global/cscratch1/sd/stephey/timemory-tutorials/tutorial-day-1/python-api-and-tools> python example-profile-context.py 
Running fibonacci...
Running inefficient...
Answer = 509443184046766.0
[wall]|0> Outputting 'timemory-example-profile-context-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-example-profile-context-output/wall.json'...
[wall]|0> Outputting 'timemory-example-profile-context-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-example-profile-context-output/wall.txt'...

|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                                              REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                                              |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                  LABEL                                   | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|--------------------------------------------------------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> fib/example-profile-context.py:6                                     |  15126 |      0 | wall   | sec    |  1.723 |  0.000 |  0.001 |  0.001 |  0.000 |   71.8 |
| >>> |_fib/example-profile-context.py:6                                   |   2584 |      1 | wall   | sec    |  0.486 |  0.000 |  0.001 |  0.001 |  0.000 |   38.3 |
| >>>   |_fib/example-profile-context.py:6                                 |   1597 |      2 | wall   | sec    |  0.300 |  0.000 |  0.001 |  0.001 |  0.000 |   37.9 |
| >>>     |_fib/example-profile-context.py:6                               |    987 |      3 | wall   | sec    |  0.186 |  0.000 |  0.001 |  0.001 |  0.000 |   37.7 |
| >>>       |_fib/example-profile-context.py:6                             |    610 |      4 | wall   | sec    |  0.116 |  0.000 |  0.001 |  0.001 |  0.000 |   37.5 |
| >>>         |_fib/example-profile-context.py:6                           |    377 |      5 | wall   | sec    |  0.073 |  0.000 |  0.001 |  0.001 |  0.000 |   37.2 |
| >>>           |_fib/example-profile-context.py:6                         |    233 |      6 | wall   | sec    |  0.046 |  0.000 |  0.001 |  0.001 |  0.000 |   37.3 |
| >>>             |_fib/example-profile-context.py:6                       |    144 |      7 | wall   | sec    |  0.029 |  0.000 |  0.001 |  0.001 |  0.000 |   37.0 |
| >>>               |_fib/example-profile-context.py:6                     |     89 |      8 | wall   | sec    |  0.018 |  0.000 |  0.001 |  0.001 |  0.000 |   37.5 |
| >>>                 |_fib/example-profile-context.py:6                   |     55 |      9 | wall   | sec    |  0.011 |  0.000 |  0.001 |  0.001 |  0.000 |   35.8 |
| >>>                   |_fib/example-profile-context.py:6                 |     34 |     10 | wall   | sec    |  0.007 |  0.000 |  0.001 |  0.001 |  0.000 |   38.5 |
| >>>                     |_fib/example-profile-context.py:6               |     21 |     11 | wall   | sec    |  0.004 |  0.000 |  0.001 |  0.001 |  0.000 |   38.2 |
| >>>                       |_fib/example-profile-context.py:6             |     13 |     12 | wall   | sec    |  0.003 |  0.000 |  0.000 |  0.000 |  0.000 |   38.9 |
| >>>                         |_fib/example-profile-context.py:6           |      8 |     13 | wall   | sec    |  0.002 |  0.000 |  0.000 |  0.000 |  0.000 |   40.7 |
| >>>                           |_fib/example-profile-context.py:6         |      5 |     14 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |   51.3 |
| >>>                             |_fib/example-profile-context.py:6       |      3 |     15 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   45.6 |
| >>>                               |_fib/example-profile-context.py:6     |      2 |     16 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   51.6 |
| >>>                                 |_fib/example-profile-context.py:6   |      1 |     17 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   53.9 |
| >>>                                   |_fib/example-profile-context.py:6 |      1 |     18 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> inefficient/example-profile-context.py:11                            |      2 |      0 | wall   | sec    |  0.202 |  0.101 |  0.178 |  0.178 |  0.109 |   88.1 |
| >>> |__sum/_methods.py:36                                                |      1 |      1 | wall   | sec    |  0.024 |  0.024 |  0.024 |  0.024 |  0.000 |    0.2 |
| >>>   |__sum/_methods.py:36                                              |      1 |      2 | wall   | sec    |  0.024 |  0.024 |  0.024 |  0.024 |  0.000 |  100.0 |
|--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
```

## Exercises: do the same thing but with tracing

Exercise 1: Command line tracing

Hint: How does the command line syntax change? 

Exercise 2: Tracing with a decorator

Hint: How does the import statement change? Does the decorator change?

Exercise 3: Tracing with a context manager

Hint: How does the import statement change? Does the context manager change?


You can find the solutions in the [solutions](solutions) folder, but we encourage you to try them first!

