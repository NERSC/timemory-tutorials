# Command Line Tools

An overview of the available command line tools in timemory.
A detailed readme is available inside each of the sub-directories.

If not done yet, please run the following command to build the apps available in the [apps](../../apps/README.md) directory.

```console
# from repository root directory
cd apps
./build.sh
source setup-env.sh
```

All the executables will be available in the `bin` directory of the repository.

## timemory-avail

The following command provides the information about everything available in the timemory package.

```console
timemory-avail -a
```

In order to search timemory package based on a specific keyword

```console
timemory-avail -H -d -r cache
```

The above command will search all hardware counters for the "cache" keyword and provide the list with their description.

## timem

`timem` tool provides performance and profiling information at the application level.

```console
timem -- lulesh
```

Use the `-o` option to generate a JSON file with the full command and the measurements:

```console
timem -o timem-report -- lulesh
```

If `timem` was built with PAPI support, you can use the `-e` option to collect hardware counters.

```console
timem -e PAPI_L2_DCM PAPI_L1_DCM PAPI_L2_ICM PAPI_L1_ICM -- lulesh
```

The above command will provide the information about L1 and L2 Data and Instruction Cache Misses.
Events passed to `-e` option can be obtained by running the `timemory-avail` command for the hardware counters as shown above.

## timemory-run

`timemory-run` allows dynamic instrumentation of a binary. This implies that the necessary components can be insterted into an application binary using this tool.
By default, the dynamic instrumentation will set the components to `wall_clock`. You can configure the default set of components with the `-d` option:

```console
timemory-run -d cpu_util peak_rss -o lulesh.inst -- lulesh
```

The above command adds `cpu_util` and `peak_rss` component instrumentation to the lulesh executable and rewrites it to `lulesh.inst` executable.

```console
./lulesh.inst
```

The above command would then print the cpu utilization and peak rss of the lulesh executable.
However, these are just the _default_ components. You can control the components at runtime via the `TIMEMORY_TRACE_COMPONENTS` environment variable.

This is an extremely useful tool which allows different instrumentations to be added to an executable and saved individually for later use.
The tool also allows usage of a custom instrumentation library along with other timemory components. Refer `02_timemory_run/03_binary_rewrite_custom_components`.

## timemory-plotter

The tool allows plotting of data from a json file.

```console
timemory-plotter -t basic_ai -f timemory-basic.region-output/cpu_roofline_ai.json
```

The above command will create a png image from the data available in the `timemory-basic.region-output/cpu_roofline_ai.json` and use the string passed to `-t` as the title.

## timemory-roofline

In order to generate a roofline plot for the lulesh application via the kokkos profiling hooks, configure your environment as follows:

```console
export KOKKOS_PROFILE_LIBRARY=libtimemory.so
export TIMEMORY_GLOBAL_COMPONENTS=cpu_roofline
export TIMEMORY_TIME_OUTPUT=OFF
```

The above commands set the kokkos profiling library to timemory, configures timemory to collect `cpu_roofline` component, and disables time-stamped subdirectories
(which commonly get enabled by the kokkos profiling hooks).

Roofline generation usually requires two passes to collect all the hardware counters so the `timemory-roofline` script just executes everything after `--` once,
toggles a few environment variables, and then executes everything after `--` again:

```console
timemory-roofline -t cpu_roofline -- lulesh
timemory-roofline -t cpu_roofline -- srun -n 2 lulesh
```

You should have a `roofline.png` image in your directory. If the script failed at the end with an error that it could not find the files,
you can run the script without the `--` and everything after it and passing the necessary `*_op.json` and `*_ai.json` files:

```console
timemory-roofline -t cpu_roofline -op timemory-lulesh-output/cpu_roofline_op.json -ai timemory-lulesh-output/cpu_roofline_ai.json
```
