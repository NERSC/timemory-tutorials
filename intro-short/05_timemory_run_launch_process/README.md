# timemory-run launch process

This example walks through the usage of `timemory-run` tool for dynamically instrumenting and then launching an application binary. The instrumentation is inserted from the libtimemory.so by default using dyninst library.

## About timemory-run
The `timemory-run` tool provides a fine grained control over instrumentation insertion by allowing users to include/exclude functions, files, modules or libraries from instrumentation, choose instrumentation modes, and enable loop level instrumentation. The `timemory-run` tool also allows instrumentation of MPI and/or OpenMP applications as well. **NOTE:** The instrumentation settings such as time or memory measurement units, floating point precision and so on are controlled by setting appropriate environment variables either in system or by passing them as `--env VARIABLE=VALUE` to `timemory-run`.

## Usage
**NOTE:** Make sure the libtimemory.so is in the `LD_LIBRARY_PATH` environment variable before running `timemory-run`.
```
$ timemory-run [OPTIONS] -- <BINARY> <ARGUMENTS>
```

### Available Options
```
Options:
    -h, --help                     Shows this page
    -v, --verbose                  Verbose output
    --debug                        Debug output
    -e, --error                    All warnings produce runtime errors
    -o, --output                   Enable binary-rewrite to new executable
    -I, -R, --function-include     Regex for selecting functions
    -E, --function-exclude         Regex for excluding functions
    -MI, -MR, --module-include     Regex for selecting modules/files/libraries
    -ME, --module-exclude          Regex for excluding modules/files/libraries
    -m, --main-function            The primary function to instrument around, e.g. 'main'
    -l, --instrument-loops         Instrument at the loop level
    -C, --collection               Include text file(s) listing function to include or exclude (prefix with '!' to exclude)
    -P, --collection-path          Additional path(s) to folders containing collection files
    -j, --jump                     Instrument with function pointers in TIMEMORY_JUMP_LIBRARY (default: jump/libtimemory.so)
    -s, --stubs                    Instrument with library stubs for LD_PRELOAD
    -L, --library                  Libraries with instrumentation routines (default: "libtimemory")
    -S, --stdlib                   Enable instrumentation of C++ standard library functions. Use with caution! May causes deadlocks/segfaults because timemory uses the STL internally. Use with '-E/--function-exclude' option as needed
    -p, --pid                      Connect to running process
    -d, --default-components       Default components to instrument
    -M, --mode [ region | trace ]  Instrumentation mode. 'trace' mode is immutable, 'region' mode is mutable by timemory library interface
    --env                          Environment variables to add to the runtime in form VARIABLE=VALUE
    --prefer [ shared | static ]   Prefer this library types when available
    --mpi                          Enable MPI support (requires timemory built w/ MPI and GOTCHA support)
    --label [ args | file | line | return ]  Labeling info for functions
    --mpip                         Enable MPI profiling via GOTCHA
    --ompt                         Enable OpenMP profiling via OMPT
    --load                         Supplemental instrumentation library names w/o extension (e.g. 'libinstr' for 'libinstr.so' or 'libinstr.a')
    --init-functions               Initialization function(s) for supplemental instrumentation libraries
    --fini-functions               Finalization function(s) for supplemental instrumentation libraries
    -c, --command                  Input executable and arguments (if '-- <CMD>' not provided)
    --dyninst-options [ BaseTrampDeletion | DelayedParsing | InstrStackFrames | MergeTramp | SaveFPR | TrampRecursive | TypeChecking ]  Advanced dyninst options: BPatch::set<OPTION>(bool), e.g. bpatch->setTrampRecursive(true)
```

## Example
```
$ timemory-run -- /usr/bin/lscpu

instrumentation target: /usr/bin/lscpu
Before processCreate
loading library: 'libtimemory.so'...
#--------------------- tim::manager initialized [rank=0][id=0][pid=20367] ---------------------#

Instrumenting with 'timemory_push_trace' and 'timemory_pop_trace'...
Parsing module: lscpu
Parsing module: RTcommon.c
Parsing module: RTposix.c
Parsing module: RTlinux.c
Parsing module: RTmemEmulator.c
.
.
.
Instrumenting |> [ usleep.c ] -> [ usleep ]
Instrumenting |> [ efgcvt_r.c ] -> [ ecvt_r ]
Instrumenting |> [ qefgcvt_r.c ] -> [ qecvt_r ]
Dumping 'available_module_functions.txt'...
Dumping 'instrumented_module_functions.txt'...
Executing...
> [timemory_trace_init@'../source/trace.cpp':594] rank = 0, pid = 20367, thread = 0, args = wall_clock...
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
CPU MHz:             1200.501
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
[wall]|0> Outputting 'timemory-lscpu-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-lscpu-output/wall.json'...
[wall]|0> Outputting 'timemory-lscpu-output/wall.txt'...
Opening 'timemory-lscpu-output/wall.jpeg' for output...
Closed 'timemory-lscpu-output/wall.jpeg'...

|---------------------------------------------------------------------------------------------------------|
|                                 REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                |
|---------------------------------------------------------------------------------------------------------|
|    LABEL      | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|---------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> dcgettext |     26 |      0 | wall   | sec    |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |  100.0 |
|---------------------------------------------------------------------------------------------------------|


[metadata::manager::finalize]> Outputting 'timemory-lscpu-output/metadata.json'...