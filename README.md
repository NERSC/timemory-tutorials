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

The docker containers are Ubuntu 18.04 images with the GCC 7.5 compiler. 
Both containers have timemory installed with all the necessary dependencies for the exercises.
Some exercises may try to collect hardware counters via PAPI. If you are running docker
on Linux, the file `/proc/sys/kernel/perf_event_paranoid` must contain a value of 1 or less
in order for HW counter collection to work. Changing this value requires sudo access.
If you do not have sudo access or are running the docker container on macOS or Windows, 
you will not be able to collect hardware counters.

#### CPU only Image

```console
git clone -b ecp-2021 https://github.com/NERSC/timemory-tutorials.git
docker pull nersc/timemory:ecp-2021-cpu
docker run -it --rm -v $PWD:/home/timemory-tutorials nersc/timemory:ecp-2021-cpu
```

#### GPU Image

> Requires `nvidia-docker`

```console
git clone -b ecp-2021 https://github.com/NERSC/timemory-tutorials.git
docker pull nersc/timemory:ecp-2021-gpu
nvidia-docker run -it --rm -v $PWD:/home/timemory-tutorials nersc/timemory:ecp-2021-gpu
```

### NERSC Training Account

You can register for a NERSC training account at [iris.nersc.gov/train](https://iris.nersc.gov/train).
The training code should have been provided to you in an email. Once you have registered, signed the
User Agreement Form, you will be given a training account of the form `trainxxx` and the account
will be activated immediately. You can login into the compute systems with the provided password without MFA.

The timemory ECP tutorial has reserved 4 Haswell nodes and 8 GPU nodes for the tutorial. The reservations are

| Day | System | Reservation Name |
| --- | ------ | ---------------- |
| 1   | Haswell | timemory-ecp2021_hsw1 |  
|     | GPU | timemory-ecp2021_gpu1 |  
| 2   | Haswell | timemory-ecp2021_hsw2 |  
|     | GPU | timemory-ecp2021_gpu2 |  

These reservations will be active 30 minutes before the start of the tutorial (8:30 AM PDT) until 30 minutes after (12:30 PM PDT).

The nodes are shared resources so please be sure to use the exact commands provided below:

#### Cori Haswell

Two hours of 4 physical CPU cores (8 hyperthreads).

```console
git clone -b ecp-2021 https://github.com/NERSC/timemory-tutorials.git
cd timemory-tutorials
module swap PrgEnv-intel PrgEnv-gnu
module load timemory
salloc --reservation=<name> -A ntrain -q shared -N 1 -n 1 -c 8 -t 02:00:00
```

Proceed to [Testing Installation](#testing-installation).

#### Cori GPU

```console
git clone -b ecp-2021 https://github.com/NERSC/timemory-tutorials.git
cd timemory-tutorials
module purge
module load cgpu gcc
module load timemory
salloc --reservation=<name> -A ntrain -N 1 -C gpu -G 1 -n 1 -c 20 -t 02:00:00
```

Proceed to [Testing Installation](#testing-installation).

### Local Installation Instructions

If your native system is Linux, consider using spack to install dyninst and/or PAPI:

```console
mkdir scripts/tools
cd scripts/tools
git clone https://github.com/spack/spack.git
source ./spack/share/spack/setup-env.sh
spack compiler find
spack external find
spack install boost~shared+pic dyninst@10.2.1%gcc
spack load boost dyninst
```

#### Linux

```console
PYTHON_VERSION=3.8
git clone https://github.com/NERSC/timemory.git timemory-source
python${PYTHON_VERSION} -m pip install --user -r timemory-source/.requirements/runtime.txt
python${PYTHON_VERSION} -m pip install --user -r timemory-source/.requirements/mpi_runtime.txt
cmake -B timemory-build \
  -D CMAKE_INSTALL_PREFIX=timemory-install \
  -D CMAKE_BUILD_TYPE=RelWithDebInfo \
  -D TIMEMORY_USE_DYNINST=ON \
  -D TIMEMORY_USE_GOTCHA=ON \
  -D TIMEMORY_USE_MPI=ON \
  -D TIMEMORY_USE_PYTHON=ON \
  -D TIMEMORY_BUILD_TOOLS=ON \
  -D PYTHON_EXECUTABLE=$(which python${PYTHON_VERSION})
cmake --build timemory-build --target all --parallel 8
cmake --build timemory-build --target install --parallel 8
export CMAKE_PREFIX_PATH=$PWD/timemory-install:${CMAKE_PREFIX_PATH}
export PATH=$PWD/timemory-install/bin:${PATH}
export LD_LIBRARY_PATH=$PWD/timemory-install/lib64:$PWD/timemory-install/lib:${LD_LIBRARY_PATH}
export PYTHONPATH=$PWD/timemory-install/lib/python${PYTHON_VERSION}/site-packages:${PYTHONPATH}
```

#### macOS

In general, the only timemory feature from the tutorial that is not available on macOS is the dynamic instrumentation (`timemory-run`).

```console
PYTHON_VERSION=3.8
git clone https://github.com/NERSC/timemory.git timemory-source
python${PYTHON_VERSION} -m pip install --user -r timemory-source/.requirements/runtime.txt
python${PYTHON_VERSION} -m pip install --user -r timemory-source/.requirements/mpi_runtime.txt
cmake -B timemory-build \
  -D CMAKE_INSTALL_PREFIX=timemory-install \
  -D CMAKE_BUILD_TYPE=RelWithDebInfo \
  -D TIMEMORY_USE_MPI=ON \
  -D TIMEMORY_USE_PYTHON=ON \
  -D TIMEMORY_BUILD_TOOLS=ON \
  -D PYTHON_EXECUTABLE=$(which python${PYTHON_VERSION})
cmake --build timemory-build --target all --parallel 8
cmake --build timemory-build --target install --parallel 8
export CMAKE_PREFIX_PATH=$PWD/timemory-install:${CMAKE_PREFIX_PATH}
export PATH=$PWD/timemory-install/bin:${PATH}
export DYLD_LIBRARY_PATH=$PWD/timemory-install/lib:${DYLD_LIBRARY_PATH}
export PYTHONPATH=$PWD/timemory-install/lib/python${PYTHON_VERSION}/site-packages:${PYTHONPATH}
```

> If you do not have python 3.8, replace `PYTHON_VERSION` with the appropriate python 3.x version.

#### Windows

Not generally recommended because `timem`, compiler instrumentation, and dynamic instrumentation are not available.
However, you can follow along with some of the other command-line tools and execute all the Python exercises.

### Testing Installation

The [apps](apps) folder contains various C and C++ applications for use in the tutorials.
See the [apps/README.md](apps/README.md) for more information and more detailed build instructions.

Regardless of whichever option you chose, perform these steps to verify you have an working installation.
From the same directory as this file, cd into [apps](apps) and run [build.sh](apps/build.sh):

```
cd apps
./build.sh
```

or run these commands manually:

```console
git submodule update --init .
cmake -B build-apps -DCMAKE_INSTALL_PREFIX=.. .
cmake --build build-apps --target all --parallel 4
cmake --build build-apps --target install --parallel 4
```

> If using the GPU docker container, execute `spack load cuda` prior to running `./build.sh`.

Once this script has completed, source the `setup-env.sh` script in the directory to add the `bin` and `lib` folders to your environment paths:

```console
# from apps folder
source setup-env.sh
```

Then you should be able to execute the `basic.trace` and `lulesh` executables:

```console
$ basic.trace
Entering Generation region...
Entering Thread region...
fibonacci(4) = 3
fibonacci(2) = 1
fibonacci(10) = 55
fibonacci(6) = 8
fibonacci(1) = 1
fibonacci(9) = 34
fibonacci(5) = 5
fibonacci(3) = 2
fibonacci(7) = 13
fibonacci(8) = 21
Entering MPI region...
fibonacci(1) = 1
fibonacci(2) = 1
fibonacci(3) = 2
fibonacci(4) = 3
fibonacci(5) = 5
fibonacci(6) = 8
fibonacci(7) = 13
fibonacci(8) = 21
fibonacci(9) = 34
fibonacci(10) = 55
Entering Scatter-Gather region...
Master rank: 0, Number of elements per process: 201
Avg of all elements is 0.50268507, Avg computed across original data is 0.50268507
Master rank: 0, Number of elements per process: 201
Avg of all elements is 0.50268507, Avg computed across original data is 0.50268507
Master rank: 0, Number of elements per process: 202
Avg of all elements is 0.50482798, Avg computed across original data is 0.50482798
Master rank: 0, Number of elements per process: 203
Avg of all elements is 0.50361019, Avg computed across original data is 0.50361019
Master rank: 0, Number of elements per process: 205
Avg of all elements is 0.50388300, Avg computed across original data is 0.50388300
Master rank: 0, Number of elements per process: 208
Avg of all elements is 0.50435603, Avg computed across original data is 0.50435603
Master rank: 0, Number of elements per process: 213
Avg of all elements is 0.49831548, Avg computed across original data is 0.49831548
Master rank: 0, Number of elements per process: 221
Avg of all elements is 0.50195003, Avg computed across original data is 0.50195003
Master rank: 0, Number of elements per process: 234
Avg of all elements is 0.49980655, Avg computed across original data is 0.49980655
Master rank: 0, Number of elements per process: 255
Avg of all elements is 0.50612098, Avg computed across original data is 0.50612098
Finished...
$ lulesh
Kokkos::OpenMP::initialize WARNING: OMP_PROC_BIND environment variable not set
  In general, for best performance with OpenMP 4.0 or better set OMP_PROC_BIND=spread and OMP_PLACES=threads
  For best performance with OpenMP 3.1 set OMP_PROC_BIND=true
  For unit testing set OMP_PROC_BIND=false

Kokkos::Cuda::initialize WARNING: Cuda is allocating into UVMSpace by default
                                  without setting CUDA_LAUNCH_BLOCKING=1.
                                  The code must call Cuda().fence() after each kernel
                                  or will likely crash when accessing data on the host.

Kokkos::Cuda::initialize WARNING: Cuda is allocating into UVMSpace by default
                                  without setting CUDA_MANAGED_FORCE_DEVICE_ALLOC=1 or
                                  setting CUDA_VISIBLE_DEVICES.
                                  This could on multi GPU systems lead to severe performance"
                                  penalties.
Running problem size 30^3 per domain until completion
Num processors: 1
Num threads: 12
Total number of elements: 27000

To run other sizes, use -s <integer>.
To run a fixed number of iterations, use -i <integer>.
To run a more or less balanced region set, use -b <integer>.
To change the relative costs of regions, use -c <integer>.
To print out progress, use -p
To write an output file for VisIt, use -v
See help (-h) for more options

Run completed:  
   Problem size        =  30 
   MPI tasks           =  1 
   Iteration count     =  932 
   Final Origin Energy = 2.025075e+05 
   Testing Plane 0 of Energy Array on rank 0:
        MaxAbsDiff   = 3.819878e-11
        TotalAbsDiff = 7.896667e-10
        MaxRelDiff   = 1.288839e-12


Elapsed time         =       6.86 (s)
Grind time (us/z/c)  = 0.27253402 (per dom)  (0.27253402 overall)
FOM                  =  3669.2667 (z/s)

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
