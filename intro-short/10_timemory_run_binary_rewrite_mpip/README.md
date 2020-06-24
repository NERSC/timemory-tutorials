# timemory-run binary rewrite

This example walks through the usage of `timemory-run` tool for dynamically instrumenting an *MPI application* binary and then re-writing the instrumented binary back to the disk. The MPI instrumentation mode is specified by `--mpip` option and the instrumented binary is written to the path specified by the `-o <inst_bin>` option.

## About timemory-run
The `timemory-run` tool provides a fine grained control over instrumentation insertion by allowing users to include/exclude functions, files, modules or libraries from instrumentation, choose instrumentation modes, and enable loop level instrumentation. The `timemory-run` tool also allows instrumentation of MPI and/or OpenMP applications as well. **NOTE:** The instrumentation settings such as time or memory measurement units, floating point precision and so on are controlled by setting appropriate environment variables either in system or by passing them as `--env VARIABLE=VALUE` to `timemory-run`.

## Usage: 
**NOTE:** Make sure the libtimemory.so is in the `LD_LIBRARY_PATH` environment variable before running `timemory-run`.
```
$ timemory-run <OPTIONS> --mpip -o <INSTRUMENTED_BINARY> -- <BINARY>
```

## Example
```
$ timemory-run --mpip --env TIMEMORY_MEMORY_UNITS=B -o mpi_hello_world.inst -- mpi_hello_world

 [command]: /home/mhaseeb/examples/mpi_hello_world

instrumentation target: /home/mhaseeb/examples/mpi_hello_world
loading library: 'libtimemory.so'...
loading library: 'libtimemory-mpip.so'...
timemory-run: Unable to find function exit
timemory-run: Unable to find function MPI_Init
timemory-run: Unable to find function MPI_Finalize
Instrumenting with 'timemory_push_trace' and 'timemory_pop_trace'...
Instrumenting 'mpip' via 'timemory_register_mpip' and 'timemory_deregister_mpip'...
+ Adding mpip instrumentation...
Parsing module: mpi_hello_world
Dumping 'available_module_functions.txt'...
Dumping 'instrumented_module_functions.txt'...

The instrumented executable image is stored in '/home/mhaseeb/repos/timemory/_skbuild/linux-x86_64-3.7/cmake-build/./mpi_hello_world'
[timemory-run]> Getting linked libraries for /home/mhaseeb/examples/mpi_hello_world...
[timemory-run]> Consider instrumenting the relevant libraries...

        /home/mhaseeb/repos/spack/opt/spack/linux-ubuntu18.04-broadwell/gcc-8.4.0/mpich-3.3.2-prhs6vjvpc3x5xjcuwqnfyhpbq44ixdh/lib/libmpi.so.12
        /usr/lib/x86_64-linux-gnu/libstdc++.so.6
        /lib/x86_64-linux-gnu/libgcc_s.so.1
        /lib/x86_64-linux-gnu/libc.so.6
        /home/mhaseeb/repos/spack/opt/spack/linux-ubuntu18.04-broadwell/gcc-8.4.0/libpciaccess-0.13.5-elaepsa53s4qvzsoo3fj7eqebgq57xzj/lib/libpciaccess.so.0
        /opt/rocm/opencl/lib/x86_64/libOpenCL.so.1
        /home/mhaseeb/repos/spack/opt/spack/linux-ubuntu18.04-broadwell/gcc-8.4.0/libxml2-2.9.10-i64pazfcyfc4tfopcdf4t34th4nne7mo/lib/libxml2.so.2
        /lib/x86_64-linux-gnu/libm.so.6
        /lib/x86_64-linux-gnu/libpthread.so.0
        /lib/x86_64-linux-gnu/librt.so.1
        /lib64/ld-linux-x86-64.so.2
        /lib/x86_64-linux-gnu/libdl.so.2
        /home/mhaseeb/repos/spack/opt/spack/linux-ubuntu18.04-broadwell/gcc-8.4.0/zlib-1.2.11-2utecthmk6d7kbti7vcfhs7nbmnpaa22/lib/libz.so.1
        /home/mhaseeb/repos/spack/opt/spack/linux-ubuntu18.04-broadwell/gcc-8.4.0/xz-5.2.5-b6hly6auxatwwqz252smffah3ukw5hrv/lib/liblzma.so.5
        /home/mhaseeb/repos/spack/opt/spack/linux-ubuntu18.04-broadwell/gcc-8.4.0/libiconv-1.16-h775wrn22yslluqrwttv3vdtwioyymoj/lib/libiconv.so.2
```

## Test the instrumented MPI binary
```
$ mpirun -n 4 ./mpi_hello_world
> [timemory_trace_init@'../../../source/trace.cpp':594] rank = 0, pid = 31534, thread = 0, args = wall_clock...
> [timemory_trace_init@'../../../source/trace.cpp':594] rank = 0, pid = 31535, thread = 0, args = wall_clock...
> [timemory_trace_init@'../../../source/trace.cpp':594] rank = 0, pid = 31537, thread = 0, args = wall_clock...
> [timemory_trace_init@'../../../source/trace.cpp':594] rank = 0, pid = 31536, thread = 0, args = wall_clock...
#--------------------- tim::manager initialized [rank=0][id=0][pid=31535] ---------------------#

#--------------------- tim::manager initialized [rank=0][id=0][pid=31537] ---------------------#

#--------------------- tim::manager initialized [rank=0][id=0][pid=31534] ---------------------#

#--------------------- tim::manager initialized [rank=0][id=0][pid=31536] ---------------------#

Hello world from processor maeve, rank 0 out of 4 processors
Hello world from processor maeve, rank 1 out of 4 processors
Hello world from processor maeve, rank 2 out of 4 processors
Hello world from processor maeve, rank 3 out of 4 processors
[mpi_comm_data]|0> Outputting 'timemory-mpi-hello-world-output/mpi_comm_data.json'...
[mpi_comm_data]|0> Outputting 'timemory-mpi-hello-world-output/mpi_comm_data.txt'...
Opening 'timemory-mpi-hello-world-output/mpi_comm_data.jpeg' for output...
Closed 'timemory-mpi-hello-world-output/mpi_comm_data.jpeg'...

|---------------------------------------------------------------------------------------------------------------------------------|
|                                                   TRACKS MPI COMMUNICATION DATA                                                 |
|---------------------------------------------------------------------------------------------------------------------------------|
|            LABEL              | COUNT  | DEPTH  |    METRIC     | UNITS  |  SUM    | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|-------------------------------|--------|--------|---------------|--------|---------|--------|--------|--------|--------|--------|
| >>> MPI_Irecv                 |     16 |      0 | mpi_comm_data | B      | 128.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
| >>> |_MPI_Irecv_dst_0         |      4 |      1 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
| >>>   |_MPI_Irecv_dst_0_tag_0 |      4 |      2 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |  100.0 |
| >>> |_MPI_Irecv_dst_1         |      4 |      1 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
| >>>   |_MPI_Irecv_dst_1_tag_0 |      4 |      2 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |  100.0 |
| >>> |_MPI_Irecv_dst_2         |      4 |      1 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
| >>>   |_MPI_Irecv_dst_2_tag_0 |      4 |      2 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |  100.0 |
| >>> |_MPI_Irecv_dst_3         |      4 |      1 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
| >>>   |_MPI_Irecv_dst_3_tag_0 |      4 |      2 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |  100.0 |
| >>> MPI_Isend                 |     16 |      0 | mpi_comm_data | B      | 128.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
|-------------------------------|--------|--------|---------------|--------|---------|--------|--------|--------|--------|--------|
| >>> |_MPI_Isend_dst_0         |      4 |      1 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
| >>>   |_MPI_Isend_dst_0_tag_0 |      4 |      2 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |  100.0 |
| >>> |_MPI_Isend_dst_1         |      4 |      1 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
| >>>   |_MPI_Isend_dst_1_tag_0 |      4 |      2 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |  100.0 |
| >>> |_MPI_Isend_dst_2         |      4 |      1 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
| >>>   |_MPI_Isend_dst_2_tag_0 |      4 |      2 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |  100.0 |
| >>> |_MPI_Isend_dst_3         |      4 |      1 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |    0.0 |
| >>>   |_MPI_Isend_dst_3_tag_0 |      4 |      2 | mpi_comm_data | B      |  32.000 |  8.000 |  8.000 |  8.000 |  0.000 |  100.0 |
|---------------------------------------------------------------------------------------------------------------------------------|



#---------------------- tim::manager destroyed [rank=2][id=0][pid=31536] ----------------------#


#---------------------- tim::manager destroyed [rank=3][id=0][pid=31537] ----------------------#


#---------------------- tim::manager destroyed [rank=1][id=0][pid=31535] ----------------------#
```