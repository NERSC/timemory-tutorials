# timemory-tutorials
Tutorials for Timemory

## Applications

The application folder contains various C, C++, and Python for use in the tutorials. It is recommended to use Spack to install timemory and various dependencies. Many folders contain a `spack.sh` script that can be tweaked as needed.

## Short Introduction

The `intro-short` tutorial is focused on the command-line tools and the Python API.

- Command-line Tools
  - `timemory-avail`
  - `timem`
  - `timemory-run`
- Python Interface
  - Decorators and context-managers for single-use (single function or region)
  - Decorators and context-managers for full profiling (every Python interpreter function call within function or region)
  - Stand-alone Components (e.g. Python classes for each component)
  
