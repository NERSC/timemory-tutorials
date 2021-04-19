# Command line tools

An overview of the available command line tools in timemory.
A detailed readme is available inside each of the sub-directories.

If not done yet, please run the following command to build the apps available in the "apps" directory.
$ ./build.sh
All the executables will be available in the `build.sh-apps/bin` directory.

## timemory-avail

The following command provides the information about everything available in the timemory package along with its description. 
$ timemory-avail -a -d

In order to search timemory package based on a specific keyword
$ timemory-avail -H -d -r cache
The above command will search all hardware counters for the "cache" keyword and provide the list with their description.
 
## timem

`timem` tool provides performance and profiling information for an application.

$ timem -- ./build.sh-apps/bin/lulesh
The above command will only profile the default information. 

$ timem -e PAPI_L2_DCM PAPI_L1_DCM PAPI_L2_ICM PAPI_L1_ICM -- ./build.sh-apps/bin/lulesh
The above command will provide the information about L1 and L2 Data and Instruction Cache Misses.
Events passed to `-e` option can be obtained by running the `timemory-avail` command for the hardware counters as shown above.

## timemory-run

`timemory-run` allows dynamic instrumentation of a binary. This implies that the necessary components can be insterted into an application binary using this tool. 

$ timemory-run -d cpu_util peak_rss -o lulesh.inst -- ./build.sh-apps/bin/lulesh
The above command adds `cpu_util` and `peak_rss` component instrumentation to the lulesh executable and rewrites it to `lulesh.inst` executable.

$ ./lulesh.inst
The above command would then print the cpu utilization and peak rss of the lulesh executable.

This is an extremely useful tool which allows different instrumentations to be added to an executable and saved individually for later use. 
The tool also allows usage of a custom instrumentation library along with other timemory components. Refer `02_timemory_run/03_binary_rewrite_custom_components`

## timemory-plotter

The tool allows plotting of data from a json file.

$ python -m timemory.plotting -t basic_ai -f timemory-basic.region-output/cpu_roofline_ai.json

The above command will create a png image from the data available in the `timemory-basic.region-output/cpu_roofline_ai.json` and use the string passed to `-t` as the title.

### timemory-roofline

In order to generate a roofline plot for the "basic" application, follow the following steps:  

$ export TIMEMORY_GLOBAL_COMPONENTS=cpu_roofline
The above command sets the timemory to collect `cpu_roofline` component.

$ timemory-run -d cpu_roofline -o basic.inst -- ./basic.trace 
The above command will add `cpu_roofline` intrumentation to the `basic.trace` executable.

$ timemory-roofline -t cpu_roofline -- ./basic.inst
The above command will use the `timemory-roofline` script to generate a roofline plot from the `basic.inst` binary instrumentation.
