# timemory-run custom components

This example walks through the usage of custom instrumentation library for `timemory-run` dynamic instrumentation. The custom instrumentation library may also implement and use one or more custom instrumentation components.

## About custom components

A custom component can utilize the static polymorphic base class `tim::component::base` to inherit many features but ultimately, the goal is to not require a specific base class. The `tim::component::base class` provides the integration into the API and requires two template parameters:
1. component type (i.e. itself)
2. the data type being stored

## About timemory-run

See [About timemory-run in 06_timemory_run_launch_process](../06_timemory_run_launch_process/README.md#about-timemory-run).

## Usage

```console
$ timemory-run --load [libCUSTOM] [OPTIONS] -o [INSTRUMENTED_BINARY] -- [BINARY] [ARGS]
```