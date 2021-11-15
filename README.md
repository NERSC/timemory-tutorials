# Timemory SC2021 HPCTools Tutorial

## Overview

This repository is intended to provide walk-through tutorials for using timemory ([Github](https://github.com/NERSC/timemory)) ([Documentation](https://timemory.readthedocs.io)) for software monitoring (performance analysis, debugging, logging) with Python.
The pre-built tools and libraries primarily focus on performance analysis.

## Example Applications

Visit the [apps/README](apps/README.md) for information on the examples.

## Containers

### CPU Only

Configuration:

- Python 3.8
- Caliper
- GOTCHA
- gperftools
- LIKWID
- PAPI
- Perfetto
- Dyninst (runtime instrumentation and binary rewrite)
- Hatchet
- line-profiler

#### Docker

```console
docker pull jrmadsen/timemory:hpctools-sc2021-cpu
docker run -it --rm --cap-add=SYS_ADMIN -v $PWD:/home/timemory-tutorials jrmadsen/timemory:hpctools-sc2021-cpu /bin/bash -l
```

#### Shifter (Cori-Haswell, Cori-KNL)

```console
salloc -q interactive -C haswell -A ntrain -N 1 -c 8 -t 01:00:00
srun shifter --image=jrmadsen/timemory:hpctools-sc2021-cpu -- /bin/bash
```

### CUDA Support

Configuration:

- Python 3.8
- CUDA 11.4
- Caliper
- GOTCHA
- gperftools
- LIKWID
- PAPI
- Perfetto
- Dyninst (runtime instrumentation and binary rewrite)
- Hatchet
- line-profiler

#### Docker

```console
docker pull jrmadsen/timemory:hpctools-sc2021-gpu
docker run -it --rm --cap-add=SYS_ADMIN -v $PWD:/home/timemory-tutorials jrmadsen/timemory:hpctools-sc2021-gpu /bin/bash -l
```

#### Shifter (Cori-GPU)

```console
salloc -A ntrain -N 1 -C gpu -G 1 -n 1 -c 20 -t 01:00:00
srun shifter --image=jrmadsen/timemory:hpctools-sc2021-gpu -- /bin/bash
```
