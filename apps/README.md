# Applications

## Getting Started

| Python Script            | Description                          |
|--------------------------|--------------------------------------|
| [example.py](example.py) | Base example with no instrumentation |

```console
export TIMEMORY_CONFIG_FILE=$PWD/timemory.cfg
```

## Basic Timemory Examples

| Python Script                                      | Description                                                            |
|----------------------------------------------------|------------------------------------------------------------------------|
| [basic/builtins.py](basic/builtins.py)             | Uses bare `@profile`                                                   |
| [basic/marker-basic.py](basic/marker-basic.py)     | Uses `@marker([...])` for single-entries in call-graph                 |
| [basic/profiler-basic.py](basic/profiler-basic.py) | Uses `@profile([...])` for profiling a function and child functions    |
| [basic/trace-basic.py](basic/trace-basic.py)       | Uses `@trace([...])` for line profiling a function and child functions |

## Extended Timemory Examples

| Python Script                                                                | Description                                                                                  |
|------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------|
| [extended/example-metadata.py](extended/example-metadata.py)                 | Demos how to add metadata to output                                                          |
| [extended/function-wrappers.py](extended/function-wrappers.py)               | Demos how to wrap popular 3rd-party C library calls (e.g. MPI)                               |
| [extended/marker-argparse-00.py](extended/marker-argparse-00.py)             | Demos adding argparse subparser support for timemory settings                                |
| [extended/marker-argparse-01.py](extended/marker-argparse-01.py)             | Demos adding argparse support for a specific settings                                        |
| [extended/marker-caliper.py](extended/marker-caliper.py)                     | Demos Caliper support in python via timemory                                                 |
| [extended/marker-data.py](extended/marker-data.py)                           | Demos directly accessing timemory call-graph data for components in python                   |
| [extended/marker-extended.py](extended/marker-extended.py)                   | Demos configuring marker components at runtime via command-line options                      |
| [extended/profiler-caliper.py](extended/profiler-caliper.py)                 | Demos Caliper support in python via timemory                                                 |
| [extended/profiler-context-manager.py](extended/profiler-context-manager.py) | Demos using `profile` context-managers instead of decorators + `flat` and `timeline` options |
| [extended/profiler-extended.py](extended/profiler-extended.py)               | Demos using `flat` and `timeline` options for `@profile` decorators                          |
| [extended/profiler-object.py](extended/profiler-object.py)                   | Demos using `profile` object instead of decorators or context-managers                       |
| [extended/trace-caliper.py](extended/trace-caliper.py)                       | Demos Caliper support in python via timemory                                                 |
| [extended/trace-context-manager.py](extended/trace-context-manager.py)       | Demos using `trace` context-managers instead of decorators                                   |

## Exercises

Here we'll outline and demo the Python profiling capabilities of timemory.

Note that tracing in timemory is also possible. We have several tracing
exercises at the bottom of this page.

### Profiling Full Script

Now let's try profiling via the command line:

```console
python -m timemory.profiler -d timemory-profile-output -- ./example.py
```

where `-d` tells timemory where to put the output files and [example.py](example.py) is the script we'd like to profile.

Note: `python -m timemory.profiler` can be replaced by `timemory-python-profiler` -- which is a console script installed in the bin folder of your python install.

```console
timemory-python-profiler -d timemory-profile-output -- ./example.py
```

### Tracing Full Script

Now let's try line profiling via the command line:

```console
python -m timemory.trace -d timemory-trace-output -- ./example.py
```

where `-d` tells timemory where to put the output files and [example.py](example.py) is the script we'd like to trace.

Note: `python -m timemory.trace` can be replaced by `timemory-python-trace` -- which is a console script installed in the bin folder of your python install.

```console
timemory-python-trace -d timemory-trace-output -- ./example.py
```

### Profiling Select Functions

In [basic/builtins.py](basic/builtins.py) you will see a bare `@profile` added to select functions.
Run `timemory-python-profiler` with the `-b` option:

Now let's try profiling via the command line:

```console
timemory-python-profiler -b -d timemory-profile-builtin-output -- ./basic/builtins.py
```

where `-b` tells timemory to only profile the functions which are decorated with `@profile`,
`-d` tells timemory where to put the output files, and [basic/builtins.py](basic/builtins.py) is the script we'd like to profile.

### Tracing Select Functions

In [basic/builtins.py](basic/builtins.py) you will see a bare `@profile` added to select functions (note: `@trace` is also valid).
Run `timemory-python-profiler` with the `-b` option:

Now let's try tracing via the command line:

```console
timemory-python-trace -b -d timemory-trace-builtin-output -- ./basic/builtins.py
```

where `-b` tells timemory to only trace the functions which are decorated with `@profile` (or `@trace`),
`-d` tells timemory where to put the output files, and [basic/builtins.py](basic/builtins.py) is the script we'd like to trace.

### Markers using decorators within script

You can take a look at [basic/marker-basic.py](basic/marker-basic.py) which already has
`@marker(...)` decorators added to the `fibonacci`, `inefficient`, and `run`
functions:

```console
python basic/marker-basic.py
```

Try removing the `@marker(...)` from the `fibonacci` function and note how the results change.
Hint: `fibonacci` should no longer be present in the results.

### Profiling using decorators within script

You can take a look at [basic/profiler-basic.py](basic/profiler-basic.py) which already has
`@profile(...)` decorators added to the `fibonacci`, `inefficient`, and `run`
functions:

```console
python basic/profiler-basic.py
```

Try removing the `@profile(...)` from the `fibonacci` function and note how the results change.
Hint: `fibonacci` should still be present in the results. Why?

### Tracing using decorators within script

You can take a look at [basic/trace-basic.py](basic/trace-basic.py) which already has
`@trace(...)` decorators added to the `fibonacci`, `inefficient`, and `run`
functions:

```console
python basic/trace-basic.py
```

Try removing the `@trace(...)` from the `fibonacci` function and note how the results change.
Hint: `fibonacci` should still be present in the results. Why?
