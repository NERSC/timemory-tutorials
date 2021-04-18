# Basic Component Recording

In this subsection we will instrument the [example.cpp](example.cpp) code from the last the section. The idea is to collect a set of metrics for different regions in the code. Timemory allows to collect a vast set of performance metrics including any use defined set of components. In order to set the type of performance metrics that are to be collected Timemory uses components. Complete list of components currently available with Timemory can be found here: [timemory.readthedocs.io/en/develop/components.html#timing-components](https://timemory.readthedocs.io/en/develop/components.html#timing-components).

In this example we intend on collecting the `CPU time` and the `Wall clock time` for different regions of the code. To configure timemory to do that we add the below lines in our code:

```cpp
timemory_set_default("wall_clock, cpu_clock");
```

This sets the default components, in the next sections we will have a look at how to change the components for different regions of the code. To begin collecting the metrics associated with the components for a certain region use the below syntax:

```cpp
int main(int argc, char** argv)
{
    // initialize timemory library
    timemory_init_library(argc, argv);

    // set default components
    timemory_set_default("wall_clock, cpu_clock");

    // begin recording components for main
    uint64_t id0 = timemory_get_begin_record("main/total");

    //... code to be profiled ...

    // end recording for main
    timemory_end_record(id0);

    // finalize timemory library
    timemory_finalize_library();
    return EXIT_SUCCESS;
}

```

In the above code `timemory_get_begin_record("main/total")` begins collecting the metrics and `timemory_end_record()` stops the colleciton of these metrics. For easy analysis each region can be named with a string so that it can be later identified in the timemory output. [example.cpp](example.cpp) contains a more complete example of profiling different regions of the code for default components. To build and run the example follow the steps below:

```console
cmake -B build .
cmake --build build --target all
cd build
export TIMEMORY_ENABLED=true
./library_example 30
```

expected output:

```console
./library_example 30
Answer = 3524578
[cpu]|0> Outputting 'timemory-library-example-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.txt'...

|-----------------------------------------------------------------------------------------------------------|
|                             TOTAL CPU TIME SPENT IN BOTH USER- AND KERNEL-MODE                            |
|-----------------------------------------------------------------------------------------------------------|
|     LABEL       | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|-----------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main/total  |      1 |      0 | cpu    | sec    |  0.020 |  0.020 |  0.020 |  0.020 |  0.000 |   50.0 |
| >>> |_/nested:0 |      1 |      1 | cpu    | sec    |  0.010 |  0.010 |  0.010 |  0.010 |  0.000 |  100.0 |
| >>> |_/nested:1 |      1 |      1 | cpu    | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|-----------------------------------------------------------------------------------------------------------|

[wall]|0> Outputting 'timemory-library-example-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.txt'...

|-----------------------------------------------------------------------------------------------------------|
|                                  REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                 |
|-----------------------------------------------------------------------------------------------------------|
|     LABEL       | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|-----------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main/total  |      1 |      0 | wall   | sec    |  0.021 |  0.021 |  0.021 |  0.021 |  0.000 |   23.8 |
| >>> |_/nested:0 |      1 |      1 | wall   | sec    |  0.008 |  0.008 |  0.008 |  0.008 |  0.000 |  100.0 |
| >>> |_/nested:1 |      1 |      1 | wall   | sec    |  0.008 |  0.008 |  0.008 |  0.008 |  0.000 |  100.0 |
|-----------------------------------------------------------------------------------------------------------|
```
