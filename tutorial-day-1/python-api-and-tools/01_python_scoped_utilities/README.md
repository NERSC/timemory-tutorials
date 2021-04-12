# Python Scoped Utilities

This example walks through the scoped utilities available in timemory Python package. These utilities include `auto_timer`, `marker`, `rss_usage` and `timer`, and provide instrumentation capabilities either as decorators or context managers. The `marker` utility allows instrumentation using a custom user-provided list of timemory components whereas the `auto_timer` and `rss_usage` utilities use timemory components relevant to timing, and rss measurements respectively. The following code snippets demonstrates the use of `timemory.util.marker` utility as decorator and context manager respectively. In both cases, the marker uses "wall_clock" and "peak_rss" components components for instrumentation.

## Decorator

```python
from timemory.util import marker

@marker(components=("wall_clock", timemory.component.peak_rss))
def fib_decorator(n=35):
    return fibonacci(n)
```

## Context Manager

```python
def main():
    with marker(get_config(["wall_clock", "peak_rss"]), key="marker_ctx_manager"):
        fibonacci(35)
```

## Starting code

Simple code performing fibonacci calculation to demonstrate using decorators and context managers.
You can instrument the fibonacci function or the run function.

```python
def fib(n):
    return n if n < 2 else (fib(n-1) + fib(n-2))

def run(n):
    ''' Decorator and context manager for high-level pre-defined collection '''
    fib(n)

if __name__ == "__main__":
    n = int(sys.argv[1] if len(sys.argv) > 1 else 10)
    run(n)
```

## Exercise

* Import timemory

```python
import timemory
from timemory.util import marker
```

* Add timemory finalization at the end of main

```python
timemory.finalize()
```

* Use decorator either fib or run function with a marker
* The first argument for a marker should be a list of components

```python
@marker(["wall_clock"])
def run(n):
    # ...
```

* Execute code

```console
./example.py
```

* Configure default settings

```python
timemory.settings.scientific = True
timemory.settings.flat_profile = True
timemory.settings.timeline_profile = True
timemory.settings.cout_output = False
```

* Add argument parser support

```python
import argparse

parser = argparse.ArgumentParser("example")
parser.add_argument(
    "-n", "--fibonacci-value", default=15, type=int, help="Starting fibonacci value"
)
timemory.add_arguments(parser)

args = parser.parse_args()

run(args.fibonacci_value)
```

* View timemory arguments

```console
./example.py -h
./example.py timemory-config -h
./example.py -n 20 timemory-config --flat-profile=False
```

* Use different components

```python
simple_set = ["wall_clock", "peak_rss"]
extended_set = ["cpu_clock", "page_rss", "virtual_memory"]

@marker(simple_set)
def fib(n):
    # ...
```

* Use context-manager

```python
def run(n):
    with marker(simple_set + extended_set, f"run({n})"):
        fib(n)
```

```python
timemory.settings.scientific = True
timemory.settings.flat_profile = True
timemory.settings.timeline_profile = True
timemory.settings.cout_output = False
```

## Example Output

### Basic Example

```console
$ ./basic.py
[wall]|0> Outputting 'timemory-basic-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-basic-output/wall.json'...
[wall]|0> Outputting 'timemory-basic-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-basic-output/wall.txt'...

|------------------------------------------------------------------------------------------------------------------|
|                                     REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                     |
|------------------------------------------------------------------------------------------------------------------|
| LABEL   | COUNT  | DEPTH  | METRIC | UNITS  |   SUM     |   MEAN    |   MIN     |   MAX     |  STDDEV   | % SELF |
|---------|--------|--------|--------|--------|-----------|-----------|-----------|-----------|-----------|--------|
| >>> run |      1 |      0 | wall   | sec    | 2.700e-05 | 2.700e-05 | 2.700e-05 | 2.700e-05 | 0.000e+00 |  100.0 |
|------------------------------------------------------------------------------------------------------------------|
```

### Help Output

```console
$ ./intermediate.py -h
usage: example [-h] [-n FIBONACCI_VALUE] {timemory-config} ...

positional arguments:
  {timemory-config}     sub-command help

optional arguments:
  -h, --help            show this help message and exit
  -n FIBONACCI_VALUE, --fibonacci-value FIBONACCI_VALUE
                        Starting fibonacci value
```

```console
$ ./intermediate.py timemory-config -h
usage: example timemory-config [-h] [-C VALUE] [--suppress-config VALUE]
                               [--suppress-parsing VALUE] [--enabled VALUE]
                               [--verbose VALUE] [--debug VALUE]
                               [--flat-profile VALUE]
                               [--timeline-profile VALUE] [--max-depth VALUE]
                               [--global-components VALUE]
                               [--ompt-components VALUE]
                               [--mpip-components VALUE]
                               [--ncclp-components VALUE]
                               [--trace-components VALUE]
                               [--profiler-components VALUE]
                               [--kokkos-components VALUE]
                               [--components VALUE] [--auto-output VALUE]
                               [--cout-output VALUE] [--file-output VALUE]
                               [--text-output VALUE] [--json-output VALUE]
                               [--tree-output VALUE] [--dart-output VALUE]
                               [--time-output VALUE] [--plot-output VALUE]
                               [--diff-output VALUE]
                               [--flamegraph-output VALUE]
                               [--ctest-notes VALUE] [--output-path VALUE]
                               [--output-prefix VALUE] [--input-path VALUE]
                               [--input-prefix VALUE]
                               [--input-extensions VALUE]
                               [--time-format VALUE] [--precision VALUE]
                               [--width VALUE] [--max-width VALUE]
                               [--scientific VALUE] [--timing-units VALUE]
                               [--memory-units VALUE]
                               [--collapse-threads VALUE]
                               [--collapse-processes VALUE]
                               [--cpu-affinity VALUE] [--mpi-init VALUE]
                               [--mpi-finalize VALUE] [--mpi-thread VALUE]
                               [--mpi-thread-type VALUE] [--upcxx-init VALUE]
                               [--upcxx-finalize VALUE] [--node-count VALUE]
                               [--papi-multiplexing VALUE]
                               [--papi-fail-on-error VALUE]
                               [--papi-quiet VALUE] [--papi-events VALUE]
                               [--papi-overflow VALUE]
                               [--cupti-activity-level VALUE]
                               [--cupti-activity-kinds VALUE]
                               [--cupti-events VALUE] [--cupti-metrics VALUE]
                               [--cupti-device VALUE] [--python-exe VALUE]
                               [--roofline-mode VALUE]
                               [--cpu-roofline-mode VALUE]
                               [--gpu-roofline-mode VALUE]
                               [--add-secondary VALUE]
                               [--throttle-count VALUE]
                               [--throttle-value VALUE]
                               [--enable-signal-handler VALUE]
                               [--allow-signal-handler VALUE]
                               [--destructor-report VALUE]
                               [--stack-clearing VALUE] [--dart-type VALUE]
                               [--dart-count VALUE] [--dart-label VALUE]
                               [--args [OPTION=VALUE [OPTION=VALUE ...]]]

Configure settings for timemory

optional arguments:
  -h, --help            show this help message and exit
  -C VALUE, --config VALUE
                        Configuration file for timemory [env:
                        TIMEMORY_CONFIG_FILE, type: std::string] (default: /Us
                        ers/jrmadsen/.timemory.cfg;/Users/jrmadsen/.timemory.j
                        son;/Users/jrmadsen/.config/timemory.cfg;/Users/jrmads
                        en/.config/timemory.json)
  --suppress-config VALUE, --no-config VALUE
                        Disable processing of setting configuration files
                        [env: TIMEMORY_SUPPRESS_CONFIG, type: bool] (default:
                        false)
  --suppress-parsing VALUE
                        Disable parsing environment [env:
                        TIMEMORY_SUPPRESS_PARSING, type: bool] (default:
                        false)
  --enabled VALUE       Activation state of timemory [env: TIMEMORY_ENABLED,
                        type: bool] (default: true)
  --verbose VALUE       Verbosity level [env: TIMEMORY_VERBOSE, type: int]
                        (default: 0)
  --debug VALUE         Enable debug output [env: TIMEMORY_DEBUG, type: bool]
                        (default: false)
  --flat-profile VALUE  Set the label hierarchy mode to default to flat [env:
                        TIMEMORY_FLAT_PROFILE, type: bool] (default: true)
  --timeline-profile VALUE
                        Set the label hierarchy mode to default to timeline
                        [env: TIMEMORY_TIMELINE_PROFILE, type: bool] (default:
                        true)
  --max-depth VALUE     Set the maximum depth of label hierarchy reporting
                        [env: TIMEMORY_MAX_DEPTH, type: unsigned short]
                        (default: 65535)
  --global-components VALUE
                        A specification of components which is used by
                        multiple variadic bundlers and user_bundles as the
                        fall-back set of components if their specific variable
                        is not set. E.g. user_mpip_bundle will use this if
                        TIMEMORY_MPIP_COMPONENTS is not specified [env:
                        TIMEMORY_GLOBAL_COMPONENTS, type: std::string]
                        (default: )
  --ompt-components VALUE
                        A specification of components which will be added to
                        structures containing the 'user_ompt_bundle'.
                        Priority: TRACE_COMPONENTS -> PROFILER_COMPONENTS ->
                        COMPONENTS -> GLOBAL_COMPONENTS [env:
                        TIMEMORY_OMPT_COMPONENTS, type: std::string] (default:
                        )
  --mpip-components VALUE
                        A specification of components which will be added to
                        structures containing the 'user_mpip_bundle'.
                        Priority: TRACE_COMPONENTS -> PROFILER_COMPONENTS ->
                        COMPONENTS -> GLOBAL_COMPONENTS [env:
                        TIMEMORY_MPIP_COMPONENTS, type: std::string] (default:
                        )
  --ncclp-components VALUE
                        A specification of components which will be added to
                        structures containing the 'user_ncclp_bundle'.
                        Priority: MPIP_COMPONENTS -> TRACE_COMPONENTS ->
                        PROFILER_COMPONENTS -> COMPONENTS -> GLOBAL_COMPONENTS
                        [env: TIMEMORY_NCCLP_COMPONENTS, type: std::string]
                        (default: )
  --trace-components VALUE
                        A specification of components which will be used by
                        the interfaces which are designed for full profiling.
                        These components will be subjected to throttling.
                        Priority: COMPONENTS -> GLOBAL_COMPONENTS [env:
                        TIMEMORY_TRACE_COMPONENTS, type: std::string]
                        (default: )
  --profiler-components VALUE
                        A specification of components which will be used by
                        the interfaces which are designed for full python
                        profiling. This specification will be overridden by a
                        trace_components specification. Priority: COMPONENTS
                        -> GLOBAL_COMPONENTS [env:
                        TIMEMORY_PROFILER_COMPONENTS, type: std::string]
                        (default: )
  --kokkos-components VALUE
                        A specification of components which will be used by
                        the interfaces which are designed for kokkos
                        profiling. Priority: TRACE_COMPONENTS ->
                        PROFILER_COMPONENTS -> COMPONENTS -> GLOBAL_COMPONENTS
                        [env: TIMEMORY_KOKKOS_COMPONENTS, type: std::string]
                        (default: )
  --components VALUE    A specification of components which is used by the
                        library interface. This falls back to
                        TIMEMORY_GLOBAL_COMPONENTS. [env: TIMEMORY_COMPONENTS,
                        type: std::string] (default: )
  --auto-output VALUE   Generate output at application termination [env:
                        TIMEMORY_AUTO_OUTPUT, type: bool] (default: true)
  --cout-output VALUE   Write output to stdout [env: TIMEMORY_COUT_OUTPUT,
                        type: bool] (default: false)
  --file-output VALUE   Write output to files [env: TIMEMORY_FILE_OUTPUT,
                        type: bool] (default: true)
  --text-output VALUE   Write text output files [env: TIMEMORY_TEXT_OUTPUT,
                        type: bool] (default: true)
  --json-output VALUE   Write json output files [env: TIMEMORY_JSON_OUTPUT,
                        type: bool] (default: true)
  --tree-output VALUE   Write hierarchical json output files [env:
                        TIMEMORY_TREE_OUTPUT, type: bool] (default: true)
  --dart-output VALUE   Write dart measurements for CDash [env:
                        TIMEMORY_DART_OUTPUT, type: bool] (default: false)
  --time-output VALUE   Output data to subfolder w/ a timestamp (see also:
                        TIMEMORY_TIME_FORMAT) [env: TIMEMORY_TIME_OUTPUT,
                        type: bool] (default: false)
  --plot-output VALUE   Generate plot outputs from json outputs [env:
                        TIMEMORY_PLOT_OUTPUT, type: bool] (default: false)
  --diff-output VALUE   Generate a difference output vs. a pre-existing output
                        (see also: TIMEMORY_INPUT_PATH and
                        TIMEMORY_INPUT_PREFIX) [env: TIMEMORY_DIFF_OUTPUT,
                        type: bool] (default: false)
  --flamegraph-output VALUE
                        Write a json output for flamegraph visualization (use
                        chrome://tracing) [env: TIMEMORY_FLAMEGRAPH_OUTPUT,
                        type: bool] (default: true)
  --ctest-notes VALUE   Write a CTestNotes.txt for each text output [env:
                        TIMEMORY_CTEST_NOTES, type: bool] (default: false)
  --output-path VALUE   Explicitly specify the output folder for results [env:
                        TIMEMORY_OUTPUT_PATH, type: std::string] (default:
                        timemory-intermediate-output)
  --output-prefix VALUE
                        Explicitly specify a prefix for all output files [env:
                        TIMEMORY_OUTPUT_PREFIX, type: std::string] (default: )
  --input-path VALUE    Explicitly specify the input folder for difference
                        comparisons (see also: TIMEMORY_DIFF_OUTPUT) [env:
                        TIMEMORY_INPUT_PATH, type: std::string] (default: )
  --input-prefix VALUE  Explicitly specify the prefix for input files used in
                        difference comparisons (see also:
                        TIMEMORY_DIFF_OUTPUT) [env: TIMEMORY_INPUT_PREFIX,
                        type: std::string] (default: )
  --input-extensions VALUE
                        File extensions used when searching for input files
                        used in difference comparisons (see also:
                        TIMEMORY_DIFF_OUTPUT) [env: TIMEMORY_INPUT_EXTENSIONS,
                        type: std::string] (default: json,xml)
  --time-format VALUE   Customize the folder generation when
                        TIMEMORY_TIME_OUTPUT is enabled (see also: strftime)
                        [env: TIMEMORY_TIME_FORMAT, type: std::string]
                        (default: %F_%I.%M_%p)
  --precision VALUE     Set the global output precision for components [env:
                        TIMEMORY_PRECISION, type: short] (default: -1)
  --width VALUE         Set the global output width for components [env:
                        TIMEMORY_WIDTH, type: short] (default: -1)
  --max-width VALUE     Set the maximum width for component label outputs
                        [env: TIMEMORY_MAX_WIDTH, type: int] (default: 120)
  --scientific VALUE    Set the global numerical reporting to scientific
                        format [env: TIMEMORY_SCIENTIFIC, type: bool]
                        (default: true)
  --timing-units VALUE  Set the units for components with 'uses_timing_units'
                        type-trait [env: TIMEMORY_TIMING_UNITS, type:
                        std::string] (default: )
  --memory-units VALUE  Set the units for components with 'uses_memory_units'
                        type-trait [env: TIMEMORY_MEMORY_UNITS, type:
                        std::string] (default: )
  --collapse-threads VALUE
                        Enable/disable combining thread-specific data [env:
                        TIMEMORY_COLLAPSE_THREADS, type: bool] (default: true)
  --collapse-processes VALUE
                        Enable/disable combining process-specific data [env:
                        TIMEMORY_COLLAPSE_PROCESSES, type: bool] (default:
                        true)
  --cpu-affinity VALUE  Enable pinning threads to CPUs (Linux-only) [env:
                        TIMEMORY_CPU_AFFINITY, type: bool] (default: false)
  --mpi-init VALUE      Enable/disable timemory calling MPI_Init /
                        MPI_Init_thread during certain timemory_init(...)
                        invocations [env: TIMEMORY_MPI_INIT, type: bool]
                        (default: false)
  --mpi-finalize VALUE  Enable/disable timemory calling MPI_Finalize during
                        timemory_finalize(...) invocations [env:
                        TIMEMORY_MPI_FINALIZE, type: bool] (default: false)
  --mpi-thread VALUE    Call MPI_Init_thread instead of MPI_Init (see also:
                        TIMEMORY_MPI_INIT) [env: TIMEMORY_MPI_THREAD, type:
                        bool] (default: true)
  --mpi-thread-type VALUE
                        MPI_Init_thread mode: 'single', 'serialized',
                        'funneled', or 'multiple' (see also: TIMEMORY_MPI_INIT
                        and TIMEMORY_MPI_THREAD) [env:
                        TIMEMORY_MPI_THREAD_TYPE, type: std::string] (default:
                        )
  --upcxx-init VALUE    Enable/disable timemory calling upcxx::init() during
                        certain timemory_init(...) invocations [env:
                        TIMEMORY_UPCXX_INIT, type: bool] (default: false)
  --upcxx-finalize VALUE
                        Enable/disable timemory calling upcxx::finalize()
                        during timemory_finalize() [env:
                        TIMEMORY_UPCXX_FINALIZE, type: bool] (default: false)
  --node-count VALUE    Total number of nodes used in application. Setting
                        this value > 1 will result in aggregating N processes
                        into groups of N / NODE_COUNT [env:
                        TIMEMORY_NODE_COUNT, type: int] (default: 0)
  --papi-multiplexing VALUE
                        Enable multiplexing when using PAPI [env:
                        TIMEMORY_PAPI_MULTIPLEXING, type: bool] (default:
                        true)
  --papi-fail-on-error VALUE
                        Configure PAPI errors to trigger a runtime error [env:
                        TIMEMORY_PAPI_FAIL_ON_ERROR, type: bool] (default:
                        false)
  --papi-quiet VALUE    Configure suppression of reporting PAPI
                        errors/warnings [env: TIMEMORY_PAPI_QUIET, type: bool]
                        (default: false)
  --papi-events VALUE   PAPI presets and events to collect (see also:
                        papi_avail) [env: TIMEMORY_PAPI_EVENTS, type:
                        std::string] (default: )
  --papi-overflow VALUE
                        Value at which PAPI hw counters trigger an overflow
                        callback [env: TIMEMORY_PAPI_OVERFLOW, type: int]
                        (default: 0)
  --cupti-activity-level VALUE
                        Default group of kinds tracked via CUpti Activity API
                        [env: TIMEMORY_CUPTI_ACTIVITY_LEVEL, type: int]
                        (default: 1)
  --cupti-activity-kinds VALUE
                        Specific cupti activity kinds to track [env:
                        TIMEMORY_CUPTI_ACTIVITY_KINDS, type: std::string]
                        (default: )
  --cupti-events VALUE  Hardware counter event types to collect on NVIDIA GPUs
                        [env: TIMEMORY_CUPTI_EVENTS, type: std::string]
                        (default: )
  --cupti-metrics VALUE
                        Hardware counter metric types to collect on NVIDIA
                        GPUs [env: TIMEMORY_CUPTI_METRICS, type: std::string]
                        (default: )
  --cupti-device VALUE  Target device for CUPTI hw counter collection [env:
                        TIMEMORY_CUPTI_DEVICE, type: int] (default: 0)
  --python-exe VALUE    Configure the python executable to use [env:
                        TIMEMORY_PYTHON_EXE, type: std::string] (default:
                        /opt/conda/envs/timemory/bin/python)
  --roofline-mode VALUE
                        Configure the roofline collection mode. Options: 'op'
                        'ai'. [env: TIMEMORY_ROOFLINE_MODE, type: std::string]
                        (default: op)
  --cpu-roofline-mode VALUE
                        Configure the roofline collection mode for CPU
                        specifically. Options: 'op', 'ai', 'mp' [env:
                        TIMEMORY_ROOFLINE_MODE_CPU, type: std::string]
                        (default: mp)
  --gpu-roofline-mode VALUE
                        Configure the roofline collection mode for GPU
                        specifically. Options: 'op' 'ai'. [env:
                        TIMEMORY_ROOFLINE_MODE_GPU, type: std::string]
                        (default: op)
  --add-secondary VALUE
                        Enable/disable components adding secondary (child)
                        entries when available. E.g. suppress individual CUDA
                        kernels, etc. when using Cupti components [env:
                        TIMEMORY_ADD_SECONDARY, type: bool] (default: true)
  --throttle-count VALUE
                        Minimum number of laps before checking whether a key
                        should be throttled [env: TIMEMORY_THROTTLE_COUNT,
                        type: unsigned long] (default: 10000)
  --throttle-value VALUE
                        Average call time in nanoseconds when # laps >
                        throttle_count that triggers throttling [env:
                        TIMEMORY_THROTTLE_VALUE, type: unsigned long]
                        (default: 10000)
  --enable-signal-handler VALUE
                        Enable signals in timemory_init [env:
                        TIMEMORY_ENABLE_SIGNAL_HANDLER, type: bool] (default:
                        false)
  --allow-signal-handler VALUE
                        Allow signal handling to be activated [env:
                        TIMEMORY_ALLOW_SIGNAL_HANDLER, type: bool] (default:
                        true)
  --destructor-report VALUE
                        Configure default setting for auto_{list,tuple,hybrid}
                        to write to stdout during destruction of the bundle
                        [env: TIMEMORY_DESTRUCTOR_REPORT, type: bool]
                        (default: false)
  --stack-clearing VALUE
                        Enable/disable stopping any markers still running
                        during finalization [env: TIMEMORY_STACK_CLEARING,
                        type: bool] (default: true)
  --dart-type VALUE     Only echo this measurement type (see also:
                        TIMEMORY_DART_OUTPUT) [env: TIMEMORY_DART_TYPE, type:
                        std::string] (default: )
  --dart-count VALUE    Only echo this number of dart tags (see also:
                        TIMEMORY_DART_OUTPUT) [env: TIMEMORY_DART_COUNT, type:
                        unsigned long long] (default: 1)
  --dart-label VALUE    Echo the category instead of the label (see also:
                        TIMEMORY_DART_OUTPUT) [env: TIMEMORY_DART_LABEL, type:
                        bool] (default: true)
  --args [OPTION=VALUE [OPTION=VALUE ...]]
                        A generic option for any setting. Each argument MUST
                        be passed in form: 'NAME=VALUE'. E.g. --args
                        "papi_events=PAPI_TOT_INS,PAPI_TOT_CYC"
                        text_output=off [type: str] (default: None)
```

### Intermediate Example

```console
$ ./intermediate.py
Outputting wall clock data to output.json

|------------------------------------------------------------------------------------------------------------------|
| LABEL   | COUNT  | DEPTH  | METRIC | UNITS  |   SUM     |   MEAN    |   MIN     |   MAX     |  STDDEV   | % SELF |
|---------|--------|--------|--------|--------|-----------|-----------|-----------|-----------|-----------|--------|
| >>> run |      1 |      0 | cpu    | sec    | 2.000e-02 | 2.000e-02 | 2.000e-02 | 2.000e-02 | 0.000e+00 |  100.0 |
|------------------------------------------------------------------------------------------------------------------|
|--------------------------------------------------------------------------------------------------------------------|
| LABEL   | COUNT  | DEPTH  | METRIC   | UNITS  |   SUM     |   MEAN    |   MIN     |   MAX     |  STDDEV   | % SELF |
|---------|--------|--------|----------|--------|-----------|-----------|-----------|-----------|-----------|--------|
| >>> run |      1 |      0 | page_rss | MB     | 1.208e+00 | 1.208e+00 | 1.208e+00 | 1.208e+00 | 0.000e+00 |  100.0 |
|--------------------------------------------------------------------------------------------------------------------|

[cpu]|0> Outputting 'timemory-intermediate-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-intermediate-output/cpu.json'...
[cpu]|0> Outputting 'timemory-intermediate-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-intermediate-output/cpu.txt'...

[wall]|0> Outputting 'timemory-intermediate-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-intermediate-output/wall.json'...
[wall]|0> Outputting 'timemory-intermediate-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-intermediate-output/wall.txt'...

[peak_rss]|0> Outputting 'timemory-intermediate-output/peak_rss.json'...
[peak_rss]|0> Outputting 'timemory-intermediate-output/peak_rss.tree.json'...
[peak_rss]|0> Outputting 'timemory-intermediate-output/peak_rss.txt'...

[page_rss]|0> Outputting 'timemory-intermediate-output/page_rss.json'...
[page_rss]|0> Outputting 'timemory-intermediate-output/page_rss.tree.json'...
[page_rss]|0> Outputting 'timemory-intermediate-output/page_rss.txt'...
```

* Generate a difference output

```console
$ ./intermediate.py timemory-config --input-path timemory-intermediate-output --diff-output=1 --output-path timemory-intermediate-diff-output
Outputting wall clock data to output.json

|----------------------------------------------------------------------------------------------------------------------|
|   LABEL     | COUNT  | DEPTH  | METRIC | UNITS  |   SUM     |   MEAN    |   MIN     |   MAX     |  STDDEV   | % SELF |
|-------------|--------|--------|--------|--------|-----------|-----------|-----------|-----------|-----------|--------|
| >>> run(15) |      1 |      0 | cpu    | sec    | 2.000e-02 | 2.000e-02 | 2.000e-02 | 2.000e-02 | 0.000e+00 |  100.0 |
|----------------------------------------------------------------------------------------------------------------------|
|------------------------------------------------------------------------------------------------------------------------|
|   LABEL     | COUNT  | DEPTH  | METRIC   | UNITS  |   SUM     |   MEAN    |   MIN     |   MAX     |  STDDEV   | % SELF |
|-------------|--------|--------|----------|--------|-----------|-----------|-----------|-----------|-----------|--------|
| >>> run(15) |      1 |      0 | page_rss | MB     | 1.188e+00 | 1.188e+00 | 1.188e+00 | 1.188e+00 | 0.000e+00 |  100.0 |
|------------------------------------------------------------------------------------------------------------------------|

Checking for existing input at timemory-intermediate-output/cpu.json...
[cpu]|0> Reading 'timemory-intermediate-output/cpu.json'...
[cpu]|0> Outputting 'timemory-intermediate-diff-output/cpu.flamegraph.json'...
[cpu]|0> Outputting 'timemory-intermediate-diff-output/cpu.json'...
[cpu]|0> Outputting 'timemory-intermediate-diff-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-intermediate-diff-output/cpu.txt'...

[cpu]|0> Outputting 'timemory-intermediate-diff-output/cpu.diff.json'...
[cpu]|0> Outputting 'timemory-intermediate-diff-output/cpu.diff.txt'...

Checking for existing input at timemory-intermediate-output/wall.json...
[wall]|0> Reading 'timemory-intermediate-output/wall.json'...
[wall]|0> Outputting 'timemory-intermediate-diff-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-intermediate-diff-output/wall.json'...
[wall]|0> Outputting 'timemory-intermediate-diff-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-intermediate-diff-output/wall.txt'...

[wall]|0> Outputting 'timemory-intermediate-diff-output/wall.diff.json'...
[wall]|0> Outputting 'timemory-intermediate-diff-output/wall.diff.txt'...

Checking for existing input at timemory-intermediate-output/peak_rss.json...
[peak_rss]|0> Reading 'timemory-intermediate-output/peak_rss.json'...
[peak_rss]|0> Outputting 'timemory-intermediate-diff-output/peak_rss.json'...
[peak_rss]|0> Outputting 'timemory-intermediate-diff-output/peak_rss.tree.json'...
[peak_rss]|0> Outputting 'timemory-intermediate-diff-output/peak_rss.txt'...

[peak_rss]|0> Outputting 'timemory-intermediate-diff-output/peak_rss.diff.json'...
[peak_rss]|0> Outputting 'timemory-intermediate-diff-output/peak_rss.diff.txt'...

Checking for existing input at timemory-intermediate-output/page_rss.json...
[page_rss]|0> Reading 'timemory-intermediate-output/page_rss.json'...
[page_rss]|0> Outputting 'timemory-intermediate-diff-output/page_rss.json'...
[page_rss]|0> Outputting 'timemory-intermediate-diff-output/page_rss.tree.json'...
[page_rss]|0> Outputting 'timemory-intermediate-diff-output/page_rss.txt'...

[page_rss]|0> Outputting 'timemory-intermediate-diff-output/page_rss.diff.json'...
[page_rss]|0> Outputting 'timemory-intermediate-diff-output/page_rss.diff.txt'...
```
