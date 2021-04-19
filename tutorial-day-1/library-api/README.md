# Library API
Timemory offers a library API that can be integerated within your code base to get more fine grained control on instrumentation. Timemory library API offers wide range of functionalities with minmal and easy to use library calls. In this tutorial we will cover basics of adding timemory library calls in example code and build it with timemory library component. The tutorial has been divided into six sections, a brief overview of each section is given below.

## 0. Overview
This section will provide a brief overview of everything, including timemory library headers, building with timemory and running and executing the examples. Each section contains an `example.cpp` file that contains the example code on which you will work and the directory `solution` contains the expected final code.

## 1. Basic Component Recording
Timemory allows to collect a vast set of performance metrics including any use defined set of components. In order to set the type of performance metrics that are to be collected Timemory uses components. Complete list of components currently available with Timemory can be found [here]( timemory.readthedocs.io/en/develop/components.html#timing-components). This section will cover basic calls that can be used to instrument your code. For example in below code snippet you can see that timemory calls are being used to time a region of code using `wall clock` and the `cpu clock` component. More details can be found in that section.
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
## 2. Marking Regions
Timemory allows multiple syntax for gathering multiple performance analysis metrics for large code bases keeping track of different code regins can become difficult. For that timemory provides code region markers where you can name regions of code using timemory API calls for easily tracking performance metrics from regions of codes. This section will cover API calls which enable you to do that and includes an exercise to modify the provided code. The below code snippet shows that `timemory_push_region` and `timemory_pop_region` calls are being used to gather components for `main` region.

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

## 3. Controlling Components
Timemory allows the flexibility of switching components that are being collected. This makes it possible to collect different metrics for different regions of the code. For example in below code snippet `timemory_push_components` overrides the default components that were set initially and then returns to the default components following a call to `timemory_pop_components`.

```cpp
// replaces the default component set with cpu_util and current_peak_rss
timemory_push_components("cpu_util, current_peak_rss");

//... some code ...

// return to the default components that were set in the beginning
timemory_pop_components();
```

## 4. Custom Components
Timemory offers the flexibility of replacing the entire stack of components being recorded or adding in your own custom components. This is achieved by replacing the timemory_create_function and timemory_delete function with function pointers that you provide. Each function in the library API which leads to components being collected (e.g. timemory_get_begin_record(), timemory_push_region(), etc.) uses these function to start/stop the components. This tutorial will provide an overview on that.

## 5. Timemory Compiler Instrumentation
Timemory offers an option of compiler instrumentation without modifying your application code. This example demonstrates the usage of timemory compiler instrumentation
([documentation](https://timemory.readthedocs.io/en/develop/tools/timemory-compiler-instrument/README.html)).
