# Library API Overview

Timemory offers a library API that can be integerated within your code base to get more fine grained control on instrumentation. Timemory library API offers wide range of functionalities with minmal and easy to use library calls. In this tutorial we will cover basics of adding timemory library calls in an example code and build it with timemory.

This tutorial has been divided into different parts, each part demonstrates a functionality/feature. For all the tutorials in this section we will use [example.cpp](example.cpp) file. Contents of this file may change for different sections depending upon the feature being demonstrated. With the exception of tutorial `01` for each tutorial you will find that [example.cpp](example.cpp) does not have timemory library calls added to it, these will be added by the attendees following the instructions in the `README`. The directory [solution](solution) in each tutorial contains a modified version of [example.cpp](example.cpp) that has the timemory calls added to it, you can refer to that as well.

## Getting Started

In order to include timemory library calls in your code we start off by including the required header:

```cpp
#include <timemory/library.h>
```

### Initializing Timemory Library

Initialization is optional. The main benefits of initialization are, by default, timemory will encode the name of the executable into the output folder, e.g. ./foo ... + initialization will generate output in timemory-foo-output/. Finalization is somewhat optional but highly recommended because when data is deleted at the termination of an application can be non-deterministic which can lead to segmentation faults. When MPI or UPC++ is being used, finalization should occur before these libraries are finalized and initialization can happen before or after but initialization afterwards is recommended. Multiple calls to initialization are permitted but multiple calls to finalization is not. Below code segment demonstrates the usage of initialization and finalization calls:

```cpp
#include <timemory/library.h>
int main(int argc, char** argv)
{
    timemory_init_library(argc, argv);
    // ...
    timemory_finalize_library();
}
```

### Building with Timemory Library

Simplest way of building with timemory is to use `cmake` to look up the required components. For Timemory library we find the required packages like:

```cmake
find_package(timemory REQUIRED COMPONENTS headers cxx)
```

the `headers` target provides the required headers, the `cxx` component finds the appropriate
C++ library based on the value of `BUILD_SHARED_LIBS` in CMake.
All the requested components are added to the `timemory::timemory` cmake target so
to link to these components, simply add `timemory::timemory` to the `target_link_libraries(...)`:

```cmake
target_link_libraries(library_example timemory::timemory)
```

For more information on building against timemory, see the
[documentation here](https://timemory.readthedocs.io/en/develop/getting_started/integrating.html#using-cmake).

## Environmental Controls

The vast majority of the environment variables can be viewed using the `timemory-avail` executable with the `-S/--settings` option. Additionally, the `<OUTPUT_PATH>/metadata.json` file will record all the environment variables during the simulation. In particular, some dynamically generated environment variables for components and variadic bundlers appear in the `metadata.json` file and not in the `timemory-avail` output.

For example, timemory instrumentation can be turned on or off by setting the below variabe to `true` or `false` respectively.

```console
export TIMEMORY_ENABLED=true
```

## Building the example code

The example code provided with this section does not perform any instrumentation but demonstrates building with Timemory and initializing and finalizing the timemory library. To build the sample code move to the directory where the `CMakeLists` file and the [example.cpp](example.cpp) file is located and the follow the below steps:

```console
cmake -B build .
cmake --build build --target all
```

After the has been built, you can do a test run using:

```console
cd build
./library_example 40
```

## Timemory Library API documentations

To have a list of all available options with Timemory Library API you can also refer to
the doxygen [documentation here](https://timemory.readthedocs.io/en/develop/api/library.html).

We will cover most of the use-cases in following sections.
