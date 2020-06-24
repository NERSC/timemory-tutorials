# timemory-tutorials
Tutorials for Timemory

## Applications

The application folder contains various C, C++, and Python for use in the tutorials. It is recommended to use Spack to install timemory and various dependencies. Many folders contain a `spack.sh` script that can be tweaked as needed.

## Short Introduction

The `intro-short` tutorial is focused on the command-line tools and the Python API.

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
  
