# Controlling Components

Timemory allows the flexibility of switching components that are being collected. This makes it possible to collect different metrics for different regions of the code. For instance from the [example.cpp](example.cpp) file, if we want to replace the default list of components that is set at the beginning with `CPU utilization` and `peak resident set size` then we can make use of the below calls:

```cpp
// replaces the default component set with cpu_util and current_peak_rss
 timemory_push_components("cpu_util, current_peak_rss");

 //... some code ...

// return to the default components that were set in the beginning
 timemory_pop_components();
```

## Exercise#1

As an exercise modify the [example.cpp](example.cpp) in this directory so that for region `total_loops` and region `loop_1_region` only components `cpu_util` and `current_peak_rss` are collected. A solution for reference has been provided in the [solution](solution) directory. To build and run both the [example.cpp](example.cpp) and the solution use below instructions:

```console
cmake -B build .
cmake --build build --target all
cd build
./library_example 40
```

expected output should like this:

```console
./library_example 40
Answer = 1095815001
[current_peak_rss]|0> Outputting 'timemory-library-example-output/current_peak_rss.json'...
[current_peak_rss]|0> Outputting 'timemory-library-example-output/current_peak_rss.tree.json'...
[current_peak_rss]|0> Outputting 'timemory-library-example-output/current_peak_rss.txt'...

|--------------------------------------------------------------------------------------------------------------|
|                         ABSOLUTE VALUE OF HIGH-WATER MARK OF MEMORY ALLOCATION IN RAM                        |
|--------------------------------------------------------------------------------------------------------------|
|       LABEL         | COUNT  | DEPTH  |     METRIC     | UNITS  | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|---------------------|--------|--------|----------------|--------|--------|--------|--------|--------|--------|
| >>> total_loops     |      1 |      0 | start peak rss | MB     | 37.332 | 37.332 | 37.332 |  0.000 |    0.0 |
|                     |        |        |  stop peak rss | MB     | 37.332 | 37.332 | 37.332 |  0.000 |    0.0 |
| >>> |_loop_1_region |      1 |      1 | start peak rss | MB     | 37.332 | 37.332 | 37.332 |  0.000 |  100.0 |
|                     |        |        |  stop peak rss | MB     | 37.332 | 37.332 | 37.332 |  0.000 |  100.0 |
|--------------------------------------------------------------------------------------------------------------|

[cpu_util]|0> Outputting 'timemory-library-example-output/cpu_util.json'...
[cpu_util]|0> Outputting 'timemory-library-example-output/cpu_util.tree.json'...
[cpu_util]|0> Outputting 'timemory-library-example-output/cpu_util.txt'...

|------------------------------------------------------------------------------------------------------|
|                        PERCENTAGE OF CPU-CLOCK TIME DIVIDED BY WALL-CLOCK TIME                       |
|------------------------------------------------------------------------------------------------------|
|       LABEL         | COUNT | DEPTH | METRIC   | UNITS | SUM  | MEAN | MIN  | MAX  | STDDEV | % SELF |
|---------------------|-------|-------|----------|-------|------|------|------|------|--------|--------|
| >>> total_loops     |     1 |     0 | cpu_util | %     | 99.8 | 99.8 | 99.8 | 99.8 |    0.0 |    0.0 |
| >>> |_loop_1_region |     1 |     1 | cpu_util | %     | 99.8 | 99.8 | 99.8 | 99.8 |    0.0 |  100.0 |
|------------------------------------------------------------------------------------------------------|

[cpu]|0> Outputting 'timemory-library-example-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.txt'...

|---------------------------------------------------------------------------------------------------------------|
|                               TOTAL CPU TIME SPENT IN BOTH USER- AND KERNEL-MODE                              |
|---------------------------------------------------------------------------------------------------------------|
|       LABEL         | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|---------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main/total      |      1 |      0 | cpu    | sec    |  6.980 |  6.980 |  6.980 |  6.980 |  0.000 |   60.3 |
| >>> |_fib           |      1 |      1 | cpu    | sec    |  0.650 |  0.650 |  0.650 |  0.650 |  0.000 |  100.0 |
| >>> |_loop_2_region |      1 |      1 | cpu    | sec    |  2.120 |  2.120 |  2.120 |  2.120 |  0.000 |  100.0 |
|---------------------------------------------------------------------------------------------------------------|

[wall]|0> Outputting 'timemory-library-example-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.txt'...

|---------------------------------------------------------------------------------------------------------------|
|                                    REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                   |
|---------------------------------------------------------------------------------------------------------------|
|       LABEL         | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|---------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main/total      |      1 |      0 | wall   | sec    |  6.992 |  6.992 |  6.992 |  6.992 |  0.000 |   60.3 |
| >>> |_fib           |      1 |      1 | wall   | sec    |  0.651 |  0.651 |  0.651 |  0.651 |  0.000 |  100.0 |
| >>> |_loop_2_region |      1 |      1 | wall   | sec    |  2.122 |  2.122 |  2.122 |  2.122 |  0.000 |  100.0 |
|---------------------------------------------------------------------------------------------------------------|
```

You can observe that the regions `total_loops` and `loop_1_region` are only present in the tables for `CPU utilization` and `peak resident set size`.

## Exercise 2

Timemory also offers an option to add and remove components from the default component set. This can be done using the below API calls:

```cpp
// add new components to the default list of components
    timemory_add_components("current_peak_rss, cpu_util");

// ... some code here ...

// remove components from default set of components
    timemory_remove_components("current_peak_rss, cpu_util");
```

Using the above two calls, as an exercise modify the [example.cpp](example.cpp) code so that for region `total_loops` and region `loop_1_region` in addition to the components set in the beginning two additional components of `current_peak_rs` and `cpu_util` are also collected. Build and run using the same instructions as above. Your expected output should look like this:

```console
./library_example 40
Answer = 1095815001
[current_peak_rss]|0> Outputting 'timemory-library-example-output/current_peak_rss.json'...
[current_peak_rss]|0> Outputting 'timemory-library-example-output/current_peak_rss.tree.json'...
[current_peak_rss]|0> Outputting 'timemory-library-example-output/current_peak_rss.txt'...

|--------------------------------------------------------------------------------------------------------------|
|                         ABSOLUTE VALUE OF HIGH-WATER MARK OF MEMORY ALLOCATION IN RAM                        |
|--------------------------------------------------------------------------------------------------------------|
|       LABEL         | COUNT  | DEPTH  |     METRIC     | UNITS  | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|---------------------|--------|--------|----------------|--------|--------|--------|--------|--------|--------|
| >>> total_loops     |      1 |      0 | start peak rss | MB     | 37.332 | 37.332 | 37.332 |  0.000 |    0.0 |
|                     |        |        |  stop peak rss | MB     | 37.332 | 37.332 | 37.332 |  0.000 |    0.0 |
| >>> |_loop_1_region |      1 |      1 | start peak rss | MB     | 37.332 | 37.332 | 37.332 |  0.000 |  100.0 |
|                     |        |        |  stop peak rss | MB     | 37.332 | 37.332 | 37.332 |  0.000 |  100.0 |
|--------------------------------------------------------------------------------------------------------------|

[cpu_util]|0> Outputting 'timemory-library-example-output/cpu_util.json'...
[cpu_util]|0> Outputting 'timemory-library-example-output/cpu_util.tree.json'...
[cpu_util]|0> Outputting 'timemory-library-example-output/cpu_util.txt'...

|------------------------------------------------------------------------------------------------------|
|                        PERCENTAGE OF CPU-CLOCK TIME DIVIDED BY WALL-CLOCK TIME                       |
|------------------------------------------------------------------------------------------------------|
|       LABEL         | COUNT | DEPTH | METRIC   | UNITS | SUM  | MEAN | MIN  | MAX  | STDDEV | % SELF |
|---------------------|-------|-------|----------|-------|------|------|------|------|--------|--------|
| >>> total_loops     |     1 |     0 | cpu_util | %     | 99.9 | 99.9 | 99.9 | 99.9 |    0.0 |    0.0 |
| >>> |_loop_1_region |     1 |     1 | cpu_util | %     | 99.9 | 99.9 | 99.9 | 99.9 |    0.0 |  100.0 |
|------------------------------------------------------------------------------------------------------|

[cpu]|0> Outputting 'timemory-library-example-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.txt'...

|-----------------------------------------------------------------------------------------------------------------|
|                                TOTAL CPU TIME SPENT IN BOTH USER- AND KERNEL-MODE                               |
|-----------------------------------------------------------------------------------------------------------------|
|        LABEL          | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|-----------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main/total        |      1 |      0 | cpu    | sec    |  7.060 |  7.060 |  7.060 |  7.060 |  0.000 |    0.0 |
| >>> |_fib             |      1 |      1 | cpu    | sec    |  0.670 |  0.670 |  0.670 |  0.670 |  0.000 |  100.0 |
| >>> |_total_loops     |      1 |      1 | cpu    | sec    |  6.390 |  6.390 |  6.390 |  6.390 |  0.000 |    0.0 |
| >>>   |_loop_1_region |      1 |      2 | cpu    | sec    |  4.250 |  4.250 |  4.250 |  4.250 |  0.000 |  100.0 |
| >>>   |_loop_2_region |      1 |      2 | cpu    | sec    |  2.140 |  2.140 |  2.140 |  2.140 |  0.000 |  100.0 |
|-----------------------------------------------------------------------------------------------------------------|

[wall]|0> Outputting 'timemory-library-example-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.txt'...

|-----------------------------------------------------------------------------------------------------------------|
|                                     REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                    |
|-----------------------------------------------------------------------------------------------------------------|
|        LABEL          | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|-----------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main/total        |      1 |      0 | wall   | sec    |  7.070 |  7.070 |  7.070 |  7.070 |  0.000 |    0.0 |
| >>> |_fib             |      1 |      1 | wall   | sec    |  0.672 |  0.672 |  0.672 |  0.672 |  0.000 |  100.0 |
| >>> |_total_loops     |      1 |      1 | wall   | sec    |  6.397 |  6.397 |  6.397 |  6.397 |  0.000 |    0.0 |
| >>>   |_loop_1_region |      1 |      2 | wall   | sec    |  4.253 |  4.253 |  4.253 |  4.253 |  0.000 |  100.0 |
| >>>   |_loop_2_region |      1 |      2 | wall   | sec    |  2.144 |  2.144 |  2.144 |  2.144 |  0.000 |  100.0 |
|-----------------------------------------------------------------------------------------------------------------|
```

You can observe that in addition to the default components being collected for the two regions of `total_loops` and `loop_1_region`, two additional components have also been collected. Solution for this exercise can be found in `solution_2` directory.
