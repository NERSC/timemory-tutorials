# timemory-run attach process

This example walks through the usage of `timemory-run` tool for attaching and dynamically inserting instrumentation in an already running application process. The application process to attach and instrument is specified by `-p <PID>` option to `timemory-run`. The instrumentation is inserted from the libtimemory.so by default using dyninst library.

## About timemory-run
The `timemory-run` tool provides a fine grained control over instrumentation insertion by allowing users to include/exclude functions, files, modules or libraries from instrumentation, choose instrumentation modes, and enable loop level instrumentation. The `timemory-run` tool also allows instrumentation of MPI and/or OpenMP applications as well. **NOTE:** The instrumentation settings such as time or memory measurement units, floating point precision and so on are controlled by setting appropriate environment variables either in system or by passing them as `--env VARIABLE=VALUE` to `timemory-run`.

## Usage
**NOTE:** Make sure the libtimemory.so is in the `LD_LIBRARY_PATH` environment variable before running `timemory-run`.
```
$ timemory-run [OPTIONS] -p <PID> -- <BINARY> <ARGS>
```
