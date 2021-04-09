# timemory-tutorials

## ECP 2021 Tutorial

### Overview

> Day 1: April 19th, 2021

> Day 2: April 26th, 2021

This repository is intended to provide walk-through tutorials for using timemory ([Github](https://github.com/NERSC/timemory)) ([Documentation](https://timemory.readthedocs.io)) for software monitoring (performance analysis, debugging, logging).
The pre-built tools and libraries primarily focus on performance analysis.

The tutorial is split into two sections/days.
The materials for Day 1 of the tutorial are in the `tutorial-day-1` folder.
Day 1 focuses on the pre-built command-line tools, libraries, and python bindings.
The materials for Day 2 of the tutorial are in the `tutorial-day-2` folder.
Day 2 focuses on the toolkit features of timemory and will teach you how to leverage timemory
to build your own extensions, libraries, and tools.

| Topic                                    | Estimated Duration |                                                                      |
| ---------------------------------------- | :----------------: | :------------------------------------------------------------------: |
| Introduction                             |     30 minutes     |           [README](tutorial-day-1/introduction/README.md)            |
| Command Line Tools (D)                   |     10 minutes     |        [README](tutorial-day-1/command-line-tools/README.md)         |
| Command Line Tools (E)                   |     30 minutes     |        [README](tutorial-day-1/command-line-tools/README.md)         |
| Library API (P)                          |     10 minutes     |            [README](tutorial-day-1/library-api/README.md)            |
| Library API (E)                          |     30 minutes     |            [README](tutorial-day-1/library-api/README.md)            |
| Python API and tools (P)                 |     10 minutes     |       [README](tutorial-day-1/python-api-and-tools/README.md)        |
| Python API and tools (D, E)              |     30 minutes     |       [README](tutorial-day-1/python-api-and-tools/README.md)        |
| Visualizing and Analyzing Results (P, D) |     15 minutes     | [README](tutorial-day-1/visualizing-and-analyzing-results/README.md) |
| __Total__                                |    165 minutes     |                                                                      |

* (P) &#8594; Presentation
* (E) &#8594; Exercises
* (D) &#8594; Demo

### Resources

#### Docker Container

```console
docker pull nersc/timemory:ecp-2021
docker run -it --rm ...
```

#### NERSC Training Account

Instructions for creating training account

#### Local Installation Instructions

```console
./install.sh
```

### Applications

The application folder contains various C, C++, and Python for use in the tutorials. It is recommended to use Spack to install timemory and various dependencies. Many folders contain a `spack.sh` script that can be tweaked as needed.

#### Building Applications

A build script is provided in `apps/build.sh`. Any additional CMake commands can be passed to the scripts (e.g. `-DUSE_MPI=ON`) and the executables and libraries will be installed to the `bin` and `lib` folder.

```console
cd apps
./build.sh -DUSE_MPI=OFF
```

#### Setup

From the root directory of the repository:

```console
source ./scripts/setup-env.sh
```

### Modules

This repository provides modulefiles for known HPC systems. Execute `source scripts/setup-env.sh` or run `module use modulefiles`.

#### Example

```console
source scripts/setup-env.sh
module use timemory-tutorials/cori-gpu
```

## Short Introduction


- Command-line Tools
  - `timemory-avail`
    - This tools provides information about various components, runtime configuration options, and hardware counters
  - `timem`
    - This tool is a drop-in replacement for the UNIX `time` tool but provides more metrics
    - This tool can also be used to run PAPI from the command-line
  - `timemory-run`
    - This is the dynamic instrumentation tool which will insert timemory measurements into existing binaries
- Python Interface
  - Decorators and context-managers for single-use (single function or region)
  - Decorators and context-managers for full profiling (every Python interpreter function call within function or region)
  - Stand-alone Components (e.g. Python classes for each component)
