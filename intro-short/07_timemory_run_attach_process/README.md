# timemory-run attach process

This example walks through the usage of `timemory-run` tool for attaching and dynamically inserting instrumentation in an already running application process. The application process to attach and instrument is specified by `-p <PID>` option to `timemory-run`. The instrumentation is inserted from the libtimemory.so by default using dyninst library.

## About timemory-run

See [About timemory-run in 06_timemory_run_launch_process](../06_timemory_run_launch_process/README.md#about-timemory-run).

## Usage

**NOTE:** Make sure the libtimemory.so is in the `LD_LIBRARY_PATH` environment variable before running `timemory-run`.

```console
$ timemory-run [OPTIONS] --pid <PID> -- <BINARY> <ARGS>
```
