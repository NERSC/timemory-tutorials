# Timemory ECP 2021 Tutorial

## Overview

| Date             | Topics                                                                   |
| ---------------- | ------------------------------------------------------------------------ |
| April 19th, 2021 | Command-line tools, Library API, Python API, Python tools, Visualization |
| April 26th, 2021 | Python toolkit, C++ template API                                         |

This repository is intended to provide walk-through tutorials for using timemory ([Github](https://github.com/NERSC/timemory)) ([Documentation](https://timemory.readthedocs.io)) for software monitoring (performance analysis, debugging, logging).
The pre-built tools and libraries primarily focus on performance analysis.

The tutorial is split into two sections/days.
The materials for Day 1 of the tutorial are in the `tutorial-day-1` folder.
Day 1 focuses on the pre-built command-line tools, libraries, and python bindings.
The materials for Day 2 of the tutorial are in the `tutorial-day-2` folder.
Day 2 focuses on the toolkit features of timemory and will teach you how to leverage timemory
to build your own extensions, libraries, and tools.

| Topic                                    | Estimated Duration |
| ---------------------------------------- | :----------------: |
| Introduction                             |     30 minutes     |
| Command Line Tools (D)                   |     10 minutes     |
| Command Line Tools (E)                   |     30 minutes     |
| Library API (P)                          |     10 minutes     |
| Library API (E)                          |     30 minutes     |
| Python API and tools (P)                 |     10 minutes     |
| Python API and tools (D, E)              |     30 minutes     |
| Visualizing and Analyzing Results (P, D) |     15 minutes     |
| __Total__                                |    165 minutes     | 

* (P) &#8594; Presentation
* (E) &#8594; Exercises
* (D) &#8594; Demo

## Resources

### Docker Container

```console
docker pull nersc/timemory:ecp-2021
docker run -it --rm ...
```

### NERSC Training Account

Instructions for creating training account

### Local Installation Instructions

```console
./install.sh
```

## Applications

The [apps](apps) folder contains various C and C++ applications for use in the tutorials.
See instructions in [apps/README.md](apps/README.md) in order to build these applications.

Once these applications are installed, source the script [scripts/setup-env.sh](scripts/setup-env.sh)
to add the installed folders to the `PATH` and `LD_LIBRARY_PATH` environment variables:

```console
source scripts/setup-env.sh
```

## NERSC Cori Modules

See instructions in [modules.md](modules.md).
