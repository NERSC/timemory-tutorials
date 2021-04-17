# NERSC Cori Modules

## Cori GPU

```console
module purge
module load cgpu
module load gcc
module load timemory
```

## Cori Haswell/KNL

```console
module swap PrgEnv-intel PrgEnv-gnu
module load timemory
```

## Expected Output

## Cori GPU Output

```console
$ module purge
$ module load cgpu
$ module load gcc
$ module load timemory
$ module list
Currently Loaded Modulefiles:
  1) esslurm                                  3) gcc/8.3.0                                5) openmpi/4.0.3                            7) papi/5.7.1.0-gpu                         9) python/3.8-anaconda-2020.11
  2) cgpu/1.0                                 4) cmake/3.18.2                             6) cuda/11.1.1                              8) likwid/5.0.0-gpu                        10) timemory/cgpu-develop-gcc-py38-openmpi
```

## Cori Haswell/KNL Output

```console
$ module list
Currently Loaded Modulefiles:
  1) modules/3.2.11.4                                  7) cray-libsci/19.06.1                              13) xpmem/2.2.20-7.0.1.1_4.20__g0475745.ari          19) PrgEnv-intel/6.0.5
  2) altd/2.0                                          8) udreg/2.3.2-7.0.1.1_3.47__g8175d3d.ari           14) job/2.2.4-7.0.1.1_3.47__g36b56f4.ari             20) craype-haswell
  3) darshan/3.2.1                                     9) ugni/6.0.14.0-7.0.1.1_7.49__ge78e5b0.ari         15) dvs/2.12_2.2.165-7.0.1.1_14.4__ge967908e         21) cray-mpich/7.7.10
  4) craype-network-aries                             10) pmi/5.0.14                                       16) alps/6.6.58-7.0.1.1_6.19__g437d88db.ari          22) craype-hugepages2M
  5) intel/19.0.3.199                                 11) dmapp/7.1.1-7.0.1.1_4.61__g38cf134.ari           17) rca/2.2.20-7.0.1.1_4.61__g8e3fb5b.ari
  6) craype/2.6.2                                     12) gni-headers/5.0.12.0-7.0.1.1_6.36__g3b1768f.ari  18) atp/2.1.3
$ module swap PrgEnv-intel PrgEnv-gnu
$ module load timemory
$ module list
Currently Loaded Modulefiles:
  1) modules/3.2.11.4                                  8) craype-haswell                                   15) gni-headers/5.0.12.0-7.0.1.1_6.36__g3b1768f.ari  22) PrgEnv-gnu/6.0.5
  2) altd/2.0                                          9) craype-hugepages2M                               16) xpmem/2.2.20-7.0.1.1_4.20__g0475745.ari          23) cmake/3.18.2
  3) darshan/3.2.1                                    10) cray-libsci/19.06.1                              17) job/2.2.4-7.0.1.1_3.47__g36b56f4.ari             24) likwid/5.0.0
  4) craype-network-aries                             11) udreg/2.3.2-7.0.1.1_3.47__g8175d3d.ari           18) dvs/2.12_2.2.165-7.0.1.1_14.4__ge967908e         25) python/3.8-anaconda-2020.11
  5) gcc/8.3.0                                        12) ugni/6.0.14.0-7.0.1.1_7.49__ge78e5b0.ari         19) alps/6.6.58-7.0.1.1_6.19__g437d88db.ari          26) timemory/haswell-develop-gcc-py38-mpich
  6) craype/2.6.2                                     13) pmi/5.0.14                                       20) rca/2.2.20-7.0.1.1_4.61__g8e3fb5b.ari
  7) cray-mpich/7.7.10                                14) dmapp/7.1.1-7.0.1.1_4.61__g38cf134.ari           21) atp/2.1.3
  $ timemory-avail --filter true
|--------------------------------|---------------|
|            COMPONENT           |   AVAILABLE   |
|--------------------------------|---------------|
| cpu_clock                      |     true      |
| cpu_roofline<double>           |     true      |
| cpu_roofline<float, double>    |     true      |
| cpu_roofline<float>            |     true      |
| cpu_util                       |     true      |
| current_peak_rss               |     true      |
| data_integer                   |     true      |
| data_unsigned                  |     true      |
| data_floating                  |     true      |
| kernel_mode_time               |     true      |
| likwid_marker                  |     true      |
| malloc_gotcha                  |     true      |
| memory_allocations             |     true      |
| monotonic_clock                |     true      |
| monotonic_raw_clock            |     true      |
| num_io_in                      |     true      |
| num_io_out                     |     true      |
| num_major_page_faults          |     true      |
| num_minor_page_faults          |     true      |
| ompt_handle<project::timemory> |     true      |
| page_rss                       |     true      |
| papi_array<8ul>                |     true      |
| papi_vector                    |     true      |
| peak_rss                       |     true      |
| priority_context_switch        |     true      |
| process_cpu_clock              |     true      |
| process_cpu_util               |     true      |
| read_bytes                     |     true      |
| read_char                      |     true      |
| system_clock                   |     true      |
| thread_cpu_clock               |     true      |
| thread_cpu_util                |     true      |
| trip_count                     |     true      |
| user_clock                     |     true      |
| user_mode_time                 |     true      |
| user_global_bundle             |     true      |
| user_kokkosp_bundle            |     true      |
| user_ompt_bundle               |     true      |
| user_profiler_bundle           |     true      |
| user_trace_bundle              |     true      |
| virtual_memory                 |     true      |
| voluntary_context_switch       |     true      |
| wall_clock                     |     true      |
| written_bytes                  |     true      |
| written_char                   |     true      |
|--------------------------------|---------------|
```
