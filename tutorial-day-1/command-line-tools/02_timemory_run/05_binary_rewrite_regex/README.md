# timemory-run binary rewrite regex

This example walks through the usage of regular expressions for including/excluding functions, modules, files and so on from the `timemory-run`'s dynamic instrumentation. The functions/methods are included or excluded from instrumentation by providing them as `-I (or) -R (or) --function-include <REGEX1> <REGEX2>..` and `-E (or) --function-exclude <REGEX3> <REGEX4> ...` respectively. Similarly, the libraries, modules, and can be included or excluded from dynamic instrumentation by providing them as `-MI <LIB1> ...` and `-ME <LIB2> ...` respectively.

## About timemory-run
See [About timemory-run in 06_timemory_run_launch_process](../06_timemory_run_launch_process/README.md#about-timemory-run).

## Usage: 

**NOTE:** Make sure the libtimemory-stubs.so is in the `LD_LIBRARY_PATH` environment variable before running `timemory-run`.

```console
$ timemory-run -I <regex1> <regex2> -E <regex3> -MI <regex5> -ME <regex6> <OPTIONS> -o <INSTRUMENTED_BINARY> -- <BINARY>
```

## Example

This example dynamically instruments an example code. All functions are instrumented including the `allreduce` function.

```console
$ timemory-run -o example.inst -- example && ./example.inst

|----------------------------------------------------------------------------------------------------------------------|
|                                       REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                       |
|----------------------------------------------------------------------------------------------------------------------|
|           LABEL            | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|----------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> MPI_Init(int*, char**) |      1 |      0 | wall   | sec    |  0.002 |  0.002 |  0.002 |  0.002 |  0.000 |   60.1 |
| >>> |_fibonacci            |     31 |      1 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_allreduce            |      1 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
| >>> |_scatter_gather       |     10 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   73.2 |
| >>>   |_create_rand_nums   |     10 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
|----------------------------------------------------------------------------------------------------------------------|
```

Now we will exclude the `allreduce` function from dynamic instrumentation of our example code.

```console
$ timemory-run -E 'allreduce*' -o example.inst -- example && ./example.inst

|----------------------------------------------------------------------------------------------------------------------|
|                                       REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                       |
|----------------------------------------------------------------------------------------------------------------------|
|           LABEL            | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|----------------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> MPI_Init(int*, char**) |      1 |      0 | wall   | sec    |  0.003 |  0.003 |  0.003 |  0.003 |  0.000 |   52.4 |
| >>> |_fibonacci            |     31 |      1 | wall   | sec    |  0.001 |  0.000 |  0.000 |  0.001 |  0.000 |  100.0 |
| >>> |_scatter_gather       |     10 |      1 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |   69.1 |
| >>>   |_create_rand_nums   |     10 |      2 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
|----------------------------------------------------------------------------------------------------------------------|
```