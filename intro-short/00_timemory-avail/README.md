# timemory-avail

This example walks through the usage of `timemory-avail` binary. The `timemory-avail` binary can be run with a combination of flags in order to display information about the timemory compoenents availability, descriptions, return types and enumeration IDs. The `timemory-avail` binary can also be used to display timemory's runtime settings (environment variables), hardware counter availability, and so on. The settings/information data can be displayed in markdown format as well as can be stored onto a file. 

USAGE: $> timemory-avail -h
RESULT: All available options for timemory-avail

SAMPLE: $> timemory-avail -h
Usage: timemory-avail [options...]

Options:
    -h, --help                     Shows this page
    -a, --all                      Print all available info
    -A, --alphabetical             Sort the output alphabetically
    -d, --description              Output the description for the component
    -e, --enum                     Display the enumeration ID
    -f, --filename                 Output the filename for the component
    -l, --language-types           Display the language-based alias/accessors
    -s, --string                   Display all acceptable string identifiers
    -v, --value                    Output the value type for the component
    -b, --brief                    Suppress availibility/value info
    -S, --settings                 Display the runtime settings
    -C, --components               Only display the components data
    -M, --markdown                 Write data in markdown
    -H, --hw-counters              Write the available hardware counters
    -O, --output                   Write results to file

// ----------------------------------------------------------------------------------------- //

DEFAULT: $> timemory-avail
RESULT: Information about component availability

SAMPLE: $> timemory-avail
|------------------------------------------|---------------|
|                 COMPONENT                |   AVAILABLE   |
|------------------------------------------|---------------|
| allinea_map                              |     false     |
| caliper                                  |     true      |
| cpu_clock                                |     true      |
| cpu_roofline<double>                     |     true      |
| cpu_roofline<float, double>              |     true      |
| cpu_roofline<float>                      |     true      |
| cpu_util                                 |     true      |
| craypat_counters                         |     false     |
| craypat_flush_buffer                     |     false     |
| craypat_heap_stats                       |     false     |
| craypat_record                           |     false     |
| craypat_region                           |     false     |
| cuda_event                               |     false     |
| cuda_profiler                            |     false     |
| cupti_activity                           |     false     |
| cupti_counters                           |     false     |
| current_peak_rss                         |     true      |
| data_rss                                 |     false     |
| gperftools_cpu_profiler                  |     true      |
| gperftools_heap_profiler                 |     true      |
| gpu_roofline<double>                     |     false     |
| gpu_roofline<cuda::half2, float, double> |     false     |
| gpu_roofline<cuda::half2>                |     false     |
| gpu_roofline<float>                      |     false     |
| kernel_mode_time                         |     true      |
| likwid_marker                            |     true      |
| likwid_nvmarker                          |     false     |
| malloc_gotcha                            |     true      |
| monotonic_clock                          |     true      |
| monotonic_raw_clock                      |     true      |
| num_io_in                                |     true      |
| num_io_out                               |     true      |
| num_major_page_faults                    |     true      |
| num_minor_page_faults                    |     true      |
| num_msg_recv                             |     false     |
| num_msg_sent                             |     false     |
| num_signals                              |     false     |
| num_swap                                 |     false     |
| nvtx_marker                              |     false     |
| ompt_handle<api::native_tag>             |     true      |
| page_rss                                 |     true      |
| papi_array<8ul>                          |     true      |
| papi_vector                              |     true      |
| peak_rss                                 |     true      |
| priority_context_switch                  |     true      |
| process_cpu_clock                        |     true      |
| process_cpu_util                         |     true      |
| read_bytes                               |     true      |
| stack_rss                                |     false     |
| system_clock                             |     true      |
| tau_marker                               |     false     |
| thread_cpu_clock                         |     true      |
| thread_cpu_util                          |     true      |
| trip_count                               |     true      |
| user_clock                               |     true      |
| user_bundle<10000ul, api::native_tag>    |     true      |
| user_bundle<11100ul, api::native_tag>    |     true      |
| user_mode_time                           |     true      |
| user_bundle<11111ul, api::native_tag>    |     true      |
| user_bundle<11110ul, api::native_tag>    |     true      |
| user_bundle<11000ul, api::native_tag>    |     true      |
| virtual_memory                           |     true      |
| voluntary_context_switch                 |     true      |
| vtune_event                              |     false     |
| vtune_frame                              |     false     |
| vtune_profiler                           |     false     |
| wall_clock                               |     true      |
| written_bytes                            |     true      |
|------------------------------------------|---------------|