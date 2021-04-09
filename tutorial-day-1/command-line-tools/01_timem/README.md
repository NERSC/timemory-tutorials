# timem

This example walks through the usage of `timem` tool. The `timem` binary can be used to profile and obtain performance measurements of an application binary. The measurements include wall clock time, user time, system time, cpu time, percentage cpu utilization, peak resident dataset size, consumed virtual memory, major and minor page faults and so on. CAUTION: Using timem in MPI context may lead to unexpected errors.

## Usage

### Default Usage:

```console
$ timem [options...] -- <CMD> <ARGS>
Options:
    -h, --help                     Shows this page
    --debug                        Debug output
    -v, --verbose                  Verbose output
    -d, --sample-delay             Set the delay before the sampler starts (seconds)
    -f, --sample-freq              Set the frequency of the sampler (1/seconds)
    -e, --events                   Set the hardware counter events to record
    -s, --shell                    Enable launching command via a shell command (if no arguments, $SHELL is used)
    --shell-flags                  Set the shell flags to use (pass as single string as leading dashes can confuse parser) [default: -i]
```

### Example

Profile a C++ application that says hello world and then add two vectors

```console
$ timem -- hello_and_vector_addition
Hello World !!
Addition Done !!
[timem] measurement totals:
        4.496019 sec wall
        3.320000 sec user
        0.260000 sec sys
        3.580000 sec cpu
       79.626006 % cpu_util
      392.128000 MB peak_rss
        0.000000 MB page_rss
        0.000000 MB virtual_memory
               0 major_page_flts
          293163 minor_page_flts
               3 prio_cxt_swch
               2 vol_cxt_swch
        6.940000 KB read
        1.543602 KB/sec read
        0.016000 KB written
        0.003559 KB/sec written
     12859539690 Total cycles
     15085135241 Instr completed
```