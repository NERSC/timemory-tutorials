# Applications

This folder contains some applications that are useful for demonstrating timemory.
Run the `./build.sh` script in this directory and the applications and libraries
will be placed in the `bin` and `lib` directories in the repository's root directory.

> NOTE: You can run CMake manually but be sure to run `git submodule update --init ${PWD}` first.

There are two main options: `USE_MPI` and `USE_CUDA`:

```console
./build.sh -DUSE_MPI=ON -DUSE_CUDA=ON
```

Enable/disable these options accordingly.

## Executables

| Executable     | Description                                |
| -------------- | ------------------------------------------ |
| `basic.region` | Contains basic library API calls           |
| `basic.trace`  | Contains no timemory instrumentation       |
| `lulesh`       | Benchmarking application built with Kokkos |

## Sample Build Output

```console
$ ./build.sh
Source directory: /Users/jrmadsen/devel/c++/timemory-tutorials/apps
Binary directory: /Users/jrmadsen/devel/c++/timemory-tutorials/apps/build.sh-apps
Output directory: /Users/jrmadsen/devel/c++/timemory-tutorials

##### Executing "cmake -B /Users/jrmadsen/devel/c++/timemory-tutorials/apps/build.sh-apps -DCMAKE_INSTALL_PREFIX=/Users/jrmadsen/devel/c++/timemory-tutorials /Users/jrmadsen/devel/c++/timemory-tutorials/apps"... #####

-- The C compiler identification is Clang 11.1.0
-- The CXX compiler identification is Clang 11.1.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /opt/conda/envs/timemory/bin/clang - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /opt/conda/envs/timemory/bin/clang++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found MPI_C: /opt/local/lib/mpich-mp/libmpi.dylib (found version "3.1")
-- Found MPI_CXX: /opt/local/lib/mpich-mp/libmpicxx.dylib (found version "3.1")
-- Found MPI: TRUE (found version "3.1")
-- [basic] timemory : "timemory" components: cxx-shared, headers
-- Found timemory: /opt/timemory/develop
-- Found Threads: TRUE
-- [custom_dynamic_instr] timemory : "timemory::custom-dynamic-instr" components: compile-options, cxx-shared, headers
-- Setting default Kokkos CXX standard to 17
-- Setting policy CMP0074 to use <Package>_ROOT variables
-- The project name is: Kokkos
-- Using -std=c++17 for C++17 standard as feature
-- Built-in Execution Spaces:
--     Device Parallel: NoTypeDefined
--     Host Parallel: Kokkos::OpenMP
--       Host Serial: SERIAL
--
-- Architectures:
-- Found TPLLIBDL: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib/libdl.tbd
-- Kokkos Devices: OPENMP;SERIAL, Kokkos Backends: OPENMP;SERIAL
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/jrmadsen/devel/c++/timemory-tutorials/apps/build.sh-apps

##### Executing "cmake --build /Users/jrmadsen/devel/c++/timemory-tutorials/apps/build.sh-apps --target all --parallel 8"... #####

Scanning dependencies of target basic.region
Scanning dependencies of target basic.trace
Scanning dependencies of target custom_dynamic_instr
Scanning dependencies of target kokkoscore
[  2%] Building CXX object basic/CMakeFiles/basic.trace.dir/basic-trace.cpp.o
[  5%] Building CXX object basic/CMakeFiles/basic.region.dir/basic-region.cpp.o
[  8%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_CPUDiscovery.cpp.o
[ 11%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_Core.cpp.o
[ 14%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_Error.cpp.o
[ 20%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_ExecPolicy.cpp.o
[ 20%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_HostBarrier.cpp.o
[ 22%] Building CXX object custom-dynamic-instr/CMakeFiles/custom_dynamic_instr.dir/custom_dynamic_instr.cpp.o
[ 25%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_HostSpace.cpp.o
[ 28%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_HostSpace_deepcopy.cpp.o
[ 31%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_HostThreadTeam.cpp.o
[ 34%] Linking CXX executable ../bin/basic.trace
[ 34%] Built target basic.trace
[ 37%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_MemoryPool.cpp.o
[ 40%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_MemorySpace.cpp.o
[ 42%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_NumericTraits.cpp.o
[ 45%] Linking CXX executable ../bin/basic.region
[ 48%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_Profiling.cpp.o
[ 51%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_Serial.cpp.o
[ 54%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_Serial_Task.cpp.o
[ 54%] Built target basic.region
[ 57%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_SharedAlloc.cpp.o
[ 60%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_Spinwait.cpp.o
[ 62%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_Stacktrace.cpp.o
[ 65%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/impl/Kokkos_hwloc.cpp.o
[ 68%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/OpenMP/Kokkos_OpenMP_Exec.cpp.o
[ 71%] Building CXX object kokkos/core/src/CMakeFiles/kokkoscore.dir/OpenMP/Kokkos_OpenMP_Task.cpp.o
[ 74%] Linking CXX shared library ../../../lib/libkokkoscore.dylib
[ 74%] Built target kokkoscore
Scanning dependencies of target kokkoscontainers
[ 77%] Building CXX object kokkos/containers/src/CMakeFiles/kokkoscontainers.dir/impl/Kokkos_UnorderedMap_impl.cpp.o
[ 80%] Linking CXX shared library ../../../lib/libkokkoscontainers.dylib
[ 80%] Built target kokkoscontainers
Scanning dependencies of target lulesh
[ 82%] Building CXX object lulesh/CMakeFiles/lulesh.dir/lulesh-init.cc.o
[ 88%] Building CXX object lulesh/CMakeFiles/lulesh.dir/lulesh-comm.cc.o
[ 88%] Building CXX object lulesh/CMakeFiles/lulesh.dir/lulesh-util.cc.o
[ 91%] Building CXX object lulesh/CMakeFiles/lulesh.dir/lulesh-viz.cc.o
[ 94%] Building CXX object lulesh/CMakeFiles/lulesh.dir/lulesh.cc.o
[ 97%] Linking CXX shared library ../lib/libcustom_dynamic_instr.dylib
[ 97%] Built target custom_dynamic_instr
[100%] Linking CXX executable ../bin/lulesh
[100%] Built target lulesh

##### Executing "cmake --build /Users/jrmadsen/devel/c++/timemory-tutorials/apps/build.sh-apps --target install --parallel 8"... #####

[  5%] Built target basic.region
[ 11%] Built target basic.trace
[ 17%] Built target custom_dynamic_instr
[ 77%] Built target kokkoscore
[ 82%] Built target kokkoscontainers
[100%] Built target lulesh
Install the project...
-- Install configuration: "RelWithDebInfo"
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/bin/basic.trace
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/bin/basic.region
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libcustom_dynamic_instr.dylib
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_HostSpace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Core.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_CopyViews.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Generic.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Traits.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_CPUDiscovery.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Profiling_C_Interface.h
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Default_Graph_Impl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Fetch_Sub.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Stacktrace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_SharedAlloc.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ChaseLev.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_View.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskBase.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_HostSharedPtr.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Serial_Task.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskPolicyData.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Fetch_Add.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Decrement.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskQueue_impl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_MultipleTaskQueue.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Compare_Exchange_Weak.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_BitOps.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskNode.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Tools.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/KokkosExp_IterateTileGPU.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskResult.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Tags.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_SimpleTaskScheduler.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/KokkosExp_ViewMapping.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_SingleTaskQueue.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_FunctorAdapter.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_MemorySpace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Assembly.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Volatile_Load.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ConcurrentBitset.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/KokkosExp_Host_IterateTile.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Increment.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Memory_Fence.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Default_Graph_fwd.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ViewArray.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskQueueMultiple.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Fetch_And.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ViewLayoutTiled.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_LIFO.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Profiling_Interface.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_GraphImpl_fwd.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Utilities.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_GraphImpl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Windows.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_OptionalRef.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_HostBarrier.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Compare_Exchange_Strong.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskQueueMemoryManager.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_LinkedListNode.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Spinwait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ViewMapping.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Exchange.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ViewUniformType.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Default_GraphNode_Impl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Store.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Default_GraphNodeKernel.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_FixedBufferMemoryPool.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_GraphImpl_Utilities.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_HostThreadTeam.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Load.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_SharedAlloc_timpl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_MinMax.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_AnalyzePolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_PhysicalLayout.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Timer.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Profiling.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Memory_Order.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Serial_WorkGraphPolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskQueueMultiple_impl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_HostSpace_deepcopy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Fetch_Or.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_FunctorAnalysis.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_GraphNodeCustomization.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Error.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_GraphNodeImpl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Combined_Reducer.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_EBO.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_MemoryPoolAllocator.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_VLAEmulation.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskQueue.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ViewCtor.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ViewTracker.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Atomic_Generic_Secondary.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ExecSpaceInitializer.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskQueueCommon.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_ClockTic.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_TaskTeamMember.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Profiling_DeviceInfo.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd/Kokkos_Fwd_SERIAL.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd/Kokkos_Fwd_THREADS.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd/Kokkos_Fwd_HPX.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd/Kokkos_Fwd_OPENMPTARGET.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd/Kokkos_Fwd_OPENMP.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd/Kokkos_Fwd_HBWSpace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd/Kokkos_Fwd_CUDA.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd/Kokkos_Fwd_SYCL.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/fwd/Kokkos_Fwd_HIP.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Threads.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Serial.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_AnonymousSpace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_HPX.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Macros.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_TaskPolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Graph_fwd.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Vectorization.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Array.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Atomic.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMPTarget
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMPTarget/Kokkos_OpenMPTarget_Instance.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMPTarget/Kokkos_OpenMPTarget_Exec.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMPTarget/Kokkos_OpenMPTarget_UniqueToken.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMPTarget/Kokkos_OpenMPTarget_Abort.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMPTarget/Kokkos_OpenMPTarget_Error.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMPTarget/Kokkos_OpenMPTarget_Parallel.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMPTarget/Kokkos_OpenMPTarget_Parallel_MDRange.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMPTarget/Kokkos_OpenMPTarget_Task.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_UniqueToken.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_ExecPolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_GraphKernelTrait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_Traits_fwd.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_OccupancyControlTrait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_PolicyTraitAdaptor.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_WorkItemPropertyTrait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_PolicyTraitMatcher.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_ExecutionSpaceTrait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_WorkTagTrait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_IterationPatternTrait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_ScheduleTrait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_LaunchBoundsTrait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/traits/Kokkos_IndexTypeTrait.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_MasterLock.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Half.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_View.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_HIP_Space.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Rank.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_BlockSize_Deduction.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Version_9_8_Compatibility.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_View.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Half.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Alloc.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Atomic_Intrinsics.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_WorkGraphPolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Parallel.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Locks.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Vectorization.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Atomic_Intrinsics_Restore_Builtins.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Team.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_UniqueToken.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_ReduceScan.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Graph_Impl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_GraphNodeKernel.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_MDRangePolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Instance.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_KernelLaunch.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_abort.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_GraphNode_Impl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Task.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Cuda/Kokkos_Cuda_Error.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_GraphNode.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/setup
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/setup/Kokkos_Setup_Cuda.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/setup/Kokkos_Setup_SYCL.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/setup/Kokkos_Setup_HIP.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_HBWSpace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Parallel.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Future.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HPX
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HPX/Kokkos_HPX_ChunkedRoundRobinExecutor.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HPX/Kokkos_HPX_WorkGraphPolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HPX/Kokkos_HPX_Task.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_ScratchSpace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Complex.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Crs.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_SYCL_Space.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Cuda.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/KokkosExp_MDRangePolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Tuners.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_MathematicalFunctions.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Concepts.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_OpenMP.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Threads
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Threads/Kokkos_ThreadsTeam.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Threads/Kokkos_Threads_WorkGraphPolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Threads/Kokkos_ThreadsExec.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Threads/Kokkos_Threads_Parallel.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_SYCL.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_MemoryPool.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Core_fwd.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_MDRangePolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Atomic.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_UniqueToken.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_WorkGraphPolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Locks.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_BlockSize_Deduction.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Shuffle_Reduce.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Parallel_Range.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Team.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_KernelLaunch.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Vectorization.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Parallel_Team.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Abort.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Instance.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_ReduceScan.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Parallel_MDRange.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/HIP/Kokkos_HIP_Error.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_Abort.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_Instance.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_Team.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_Parallel_Reduce.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_Parallel_Team.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_Parallel_Scan.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_MDRangePolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_DeepCopy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_Parallel_Range.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/SYCL/Kokkos_SYCL_UniqueToken.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Parallel_Reduce.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_TaskScheduler.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_hwloc.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_PointerOwnership.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Profiling_ProfileSection.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_OpenMPTarget.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_HIP.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Timer.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_OpenMPTargetSpace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Graph.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Layout.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_NumericTraits.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_MemoryTraits.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_TaskScheduler_fwd.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_CudaSpace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_LogicalSpaces.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Extents.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_WorkGraphPolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl/Kokkos_Declare_SERIAL.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl/Kokkos_Declare_CUDA.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl/Kokkos_Declare_SYCL.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl/Kokkos_Declare_HBWSpace.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl/Kokkos_Declare_OPENMPTARGET.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl/Kokkos_Declare_HIP.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl/Kokkos_Declare_OPENMP.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl/Kokkos_Declare_HPX.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/decl/Kokkos_Declare_THREADS.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMP
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMP/Kokkos_OpenMP_Parallel.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMP/Kokkos_OpenMP_Task.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMP/Kokkos_OpenMP_WorkGraphPolicy.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMP/Kokkos_OpenMP_Team.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/OpenMP/Kokkos_OpenMP_Exec.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_AcquireUniqueTokenImpl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Pair.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscore.3.3.99.dylib
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscore.3.3.dylib
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscore.dylib
-- Up-to-date: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscore.3.3.99.dylib
-- Up-to-date: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscore.3.3.dylib
-- Up-to-date: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscore.dylib
-- Up-to-date: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos
-- Up-to-date: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_StaticCrsGraph_factory.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_UnorderedMap_impl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Functional_impl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/impl/Kokkos_Bitset_impl.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_StaticCrsGraph.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Vector.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Bitset.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_DualView.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_UnorderedMap.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_ScatterView.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Functional.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_OffsetView.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_DynamicView.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_DynRankView.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_ErrorReporter.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscontainers.3.3.99.dylib
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscontainers.3.3.dylib
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscontainers.dylib
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscontainers.3.3.99.dylib
-- Up-to-date: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscontainers.3.3.dylib
-- Up-to-date: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/libkokkoscontainers.dylib
-- Up-to-date: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Sort.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/Kokkos_Random.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/cmake/Kokkos/KokkosConfig.cmake
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/cmake/Kokkos/KokkosConfigCommon.cmake
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/cmake/Kokkos/KokkosConfigVersion.cmake
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/cmake/Kokkos/KokkosTargets.cmake
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/lib/cmake/Kokkos/KokkosTargets-relwithdebinfo.cmake
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/KokkosCore_config.h
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/bin/nvcc_wrapper
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/bin/hpcbind
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/bin/kokkos_launch_compiler
-- Up-to-date: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/KokkosCore_config.h
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/KokkosCore_Config_FwdBackend.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/KokkosCore_Config_SetupBackend.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/KokkosCore_Config_DeclareBackend.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/include/kokkos/KokkosCore_Config_PostInclude.hpp
-- Installing: /Users/jrmadsen/devel/c++/timemory-tutorials/bin/lulesh
```
