# matvec micro benchmark
The benchmark implements a simple matrix multiplication example using OpenMP4.5 framework and Kokkos framework. The intention is to show how multiple loops can be distributed across various hierarchies of the GPU architecture.

## Building and Running using cmake
Each directory has a basic CMakeLists.txt which can be used to build the application.

### OpenMPTarget
For the OpenMPTarget version the cmake is only has options for the clang compiler.
Switch OPENMP_TARGET variable on in order to turn on the offload flags.

### Kokkos
For the Kokkos version, pass the Kokkos build path to Kokkos_ROOT as specified in the Kokkos documentation.

## Building and Running using make
Each directory also has a Makefile to which the compiler and version information can be passed via COMP variable :
make COMP=clang OPENMP_TARGET=y
