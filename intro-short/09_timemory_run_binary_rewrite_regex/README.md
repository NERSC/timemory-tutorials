# timemory-run binary rewrite regex

This example walks through the usage of regular expressions for including/excluding functions, modules, files and so on from the `timemory-run`'s dynamic instrumentation. The functions/methods are included or excluded from instrumentation by providing them as `-I (or) -R (or) --function-include <REGEX1> <REGEX2>..` and `-E (or) --function-exclude <REGEX3> <REGEX4> ...` respectively. Similarly, the libraries, modules, and can be included or excluded from dynamic instrumentation by providing them as `-MI <LIB1> ...` and `-ME <LIB2> ...` respectively.

## About timemory-run
The `timemory-run` tool provides a fine grained control over instrumentation insertion by allowing users to include/exclude functions, files, modules or libraries from instrumentation, choose instrumentation modes, and enable loop level instrumentation. The `timemory-run` tool also allows instrumentation of MPI and/or OpenMP applications as well. **NOTE:** The instrumentation settings such as time or memory measurement units, floating point precision and so on are controlled by setting appropriate environment variables either in system or by passing them as `--env VARIABLE=VALUE` to `timemory-run`.

## Usage: 
**NOTE:** Make sure the libtimemory-stubs.so is in the `LD_LIBRARY_PATH` environment variable before running `timemory-run`.
```
$ timemory-run -I <regex1> <regex2> -E <regex3> -MI <regex5> -ME <regex6> <OPTIONS> -o <INSTRUMENTED_BINARY> -- <BINARY>
```


