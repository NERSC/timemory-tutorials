# Testing Installation

The [apps](apps) folder contains various C and C++ applications for use in the tutorials.
See the [apps/README.md](apps/README.md) for more information and more detailed build instructions.

Regardless of whichever option you chose in the [resources](resources.md) section,
perform these steps to verify you have an working installation.
From the same directory as this file, cd into [apps](apps) and run [build.sh](apps/build.sh):

```console
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
