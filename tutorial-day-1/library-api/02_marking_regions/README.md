# Region Markers

Timemory allows for easy labelling of regions of code with markers using `timemory_push_region()` and `timemory_pop_region()` calls. For instance:

```cpp
int main(int argc, char** argv)
{
// initialize timemory library
    timemory_init_library(argc, argv);

// set default components
    timemory_set_default("wall_clock, cpu_clock");

// begin recording for region main
    timemory_push_region("main");

    //... code in the region ...

// end recording for main region
    timemory_pop_region("main");

// finalize timemory library
    timemory_finalize_library();
    return EXIT_SUCCESS;
}
```

In the above example you can see that we have replaced the `timemory_get_begin_record` and `timemory_end_record()` with `timemory_push_region()` and `timemory_pop_region()` calls which allows for easy labelling of the regions.

## Exercise

As an exercise you will need to modify the [example.cpp](example.cpp) code available in this directory such that it utilizes region markers. A solution is available in the [solution](solution) folder.

### Building and running

To build the code in [example.cpp](example.cpp) use:

```console
cmake -B build .
cmake --build build --target all
```

To run use:

```console
cd build
export TIMEMORY_ENABLED=true
./library_example 40
```

Expected output:

```console
./library_example 40
Answer = 1095815001
[cpu]|0> Outputting 'timemory-library-example-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-library-example-output/cpu.txt'...

|-------------------------------------------------------------------------------------------------------------|
|                              TOTAL CPU TIME SPENT IN BOTH USER- AND KERNEL-MODE                             |
|-------------------------------------------------------------------------------------------------------------|
|      LABEL        | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|-------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main/total    |      1 |      0 | cpu    | sec    |  6.680 |  6.680 |  6.680 |  6.680 |  0.000 |    0.0 |
| >>> |_fib         |      1 |      1 | cpu    | sec    |  0.640 |  0.640 |  0.640 |  0.640 |  0.000 |  100.0 |
| >>> |_total_loops |      1 |      1 | cpu    | sec    |  6.040 |  6.040 |  6.040 |  6.040 |  0.000 |    0.0 |
| >>>   |_loop_1    |      1 |      2 | cpu    | sec    |  4.060 |  4.060 |  4.060 |  4.060 |  0.000 |  100.0 |
| >>>   |_loop_2    |      1 |      2 | cpu    | sec    |  1.980 |  1.980 |  1.980 |  1.980 |  0.000 |  100.0 |
|-------------------------------------------------------------------------------------------------------------|

[wall]|0> Outputting 'timemory-library-example-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.txt'...

|-------------------------------------------------------------------------------------------------------------|
|                                   REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                  |
|-------------------------------------------------------------------------------------------------------------|
|      LABEL        | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|-------------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main/total    |      1 |      0 | wall   | sec    |  6.684 |  6.684 |  6.684 |  6.684 |  0.000 |    0.0 |
| >>> |_fib         |      1 |      1 | wall   | sec    |  0.640 |  0.640 |  0.640 |  0.640 |  0.000 |  100.0 |
| >>> |_total_loops |      1 |      1 | wall   | sec    |  6.044 |  6.044 |  6.044 |  6.044 |  0.000 |    0.0 |
| >>>   |_loop_1    |      1 |      2 | wall   | sec    |  4.061 |  4.061 |  4.061 |  4.061 |  0.000 |  100.0 |
| >>>   |_loop_2    |      1 |      2 | wall   | sec    |  1.983 |  1.983 |  1.983 |  1.983 |  0.000 |  100.0 |
|-------------------------------------------------------------------------------------------------------------|
```

After you have completed the changes for the exercise your output should still resemble to the one above (from unmodified example.cpp).

Documentation are available [here](https://timemory.readthedocs.io/en/develop/api/library.html?highlight=timemory_push_regions#_CPPv420timemory_push_regionPKc).
