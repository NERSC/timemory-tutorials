# timemory-run binary rewrite

This example walks through the usage of `timemory-run` tool for dynamically instrumenting an application binary and then re-writing the instrumented binary back to the disk. The instrumentation routines are inserted from the libtimemory.so library by default. The updated (instrumented) binary is written to the path specified by the `-o <inst_bin>` option to `timemory-run`. 

## About timemory-run
See [About timemory-run in 06_timemory_run_launch_process](../06_timemory_run_launch_process/README.md#about-timemory-run).

## Usage: 

**NOTE:** Make sure the libtimemory.so is in the `LD_LIBRARY_PATH` environment variable before running `timemory-run`.

```console
$ timemory-run <OPTIONS> -o <INSTRUMENTED_BINARY> -- <BINARY>
```

## Example

```console
$ timemory-run -d wall_clock cpu_util peak_rss -o lscpu.inst -- /usr/bin/lscpu

 [command]: /usr/bin/lscpu

instrumentation target: /usr/bin/lscpu
loading library: 'libtimemory.so'...
timemory-run: Unable to find function exit
timemory-run: Unable to find function MPI_Init
timemory-run: Unable to find function MPI_Finalize
Instrumenting with 'timemory_push_trace' and 'timemory_pop_trace'...
Parsing module: lscpu
Dumping 'available_module_functions.txt'...
Dumping 'instrumented_module_functions.txt'...

The instrumented executable image is stored in '/home/mhaseeb/repos/haseeb/timemory/build/lscpu.inst'
[timemory-run]> Getting linked libraries for /usr/bin/lscpu...
[timemory-run]> Consider instrumenting the relevant libraries...

        /lib/x86_64-linux-gnu/libsmartcols.so.1
        /lib/x86_64-linux-gnu/libc.so.6
        /lib64/ld-linux-x86-64.so.2
```

### Test the Instrumented binary

```console
$ ./lscpu.inst
#------------------------- tim::manager initialized [id=0][pid=13746] -------------------------#

[pid=13746][tid=0][timemory_trace_init@'../source/trace.cpp':594]> rank = 0, pid = 13746, thread = 0, args = wall_clock,cpu_util,peak_rss...
Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              12
On-line CPU(s) list: 0-11
Thread(s) per core:  2
Core(s) per socket:  6
Socket(s):           1
NUMA node(s):        1
Vendor ID:           GenuineIntel
CPU family:          6
Model:               79
Model name:          Intel(R) Core(TM) i7-6800K CPU @ 3.40GHz
Stepping:            1
CPU MHz:             2142.482
CPU max MHz:         4000.0000
CPU min MHz:         1200.0000
BogoMIPS:            6799.28
Virtualization:      VT-x
L1d cache:           32K
L1i cache:           32K
L2 cache:            256K
L3 cache:            15360K
NUMA node0 CPU(s):   0-11
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid dca sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb cat_l3 cdp_l3 invpcid_single pti intel_ppin ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm cqm rdt_a rdseed adx smap intel_pt xsaveopt cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local dtherm ida arat pln pts md_clear flush_l1d
[peak_rss]|0> Outputting 'timemory-lscpu.inst-output/peak_rss.json'...
[peak_rss]|0> Outputting 'timemory-lscpu.inst-output/peak_rss.txt'...
Opening 'timemory-lscpu.inst-output/peak_rss.jpeg' for output...
Closed 'timemory-lscpu.inst-output/peak_rss.jpeg'...

|------------------------------------------------------------------------------------------------------|
|MEASURES CHANGES IN THE HIGH-WATER MARK FOR THE AMOUNT OF MEMORY ALLOCATED IN RAM. MAY FLUCTUATE IF SWAP IS ENABLED|
|------------------------------------------------------------------------------------------------------|
|  LABEL   | COUNT  | DEPTH  | METRIC   | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|----------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>> main |      1 |      0 | peak_rss | B      |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|------------------------------------------------------------------------------------------------------|

[cpu_util]|0> Outputting 'timemory-lscpu.inst-output/cpu_util.json'...
[cpu_util]|0> Outputting 'timemory-lscpu.inst-output/cpu_util.txt'...
Opening 'timemory-lscpu.inst-output/cpu_util.jpeg' for output...
Closed 'timemory-lscpu.inst-output/cpu_util.jpeg'...

|-------------------------------------------------------------------------------------------|
|                   PERCENTAGE OF CPU-CLOCK TIME DIVIDED BY WALL-CLOCK TIME                 |
|-------------------------------------------------------------------------------------------|
|  LABEL   | COUNT | DEPTH | METRIC   | UNITS | SUM  | MEAN | MIN  | MAX  | STDDEV | % SELF |
|----------|-------|-------|----------|-------|------|------|------|------|--------|--------|
| >>> main |     1 |     0 | cpu_util | %     | 43.1 | 43.1 | 43.1 | 43.1 |    0.0 |  100.0 |
|-------------------------------------------------------------------------------------------|

[wall]|0> Outputting 'timemory-lscpu.inst-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-lscpu.inst-output/wall.json'...
[wall]|0> Outputting 'timemory-lscpu.inst-output/wall.txt'...
Opening 'timemory-lscpu.inst-output/wall.jpeg' for output...
Closed 'timemory-lscpu.inst-output/wall.jpeg'...

|----------------------------------------------------------------------------------------------------|
|                              REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                              |
|----------------------------------------------------------------------------------------------------|
|  LABEL   | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|----------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main |      1 |      0 | wall   | sec    |  0.023 |  0.023 |  0.023 |  0.023 |  0.000 |  100.0 |
|----------------------------------------------------------------------------------------------------|


[metadata::manager::finalize]> Outputting 'timemory-lscpu.inst-output/metadata.json'...
```