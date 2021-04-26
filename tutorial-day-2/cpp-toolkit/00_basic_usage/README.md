# Basic Usage

## Initialization and Finalization

Let's add initialization, finalization, and argument parsing support to [example.cpp](example.cpp).
Additionally, let's tweak the settings to some new defaults before initialization:

```cpp
// ...

#include <timemory/timemory.hpp>

int main(int argc, char** argv)
{
    tim::settings::flamegraph_output()  = false;
    tim::settings::cout_output()        = false;
    tim::settings::scientific()         = true;
    tim::settings::collapse_processes() = false;

    tim::timemory_init(argc, argv);
    tim::timemory_argparse(&argc, &argv);

    // ...

    tim::timemory_finalize();
    return 0;
}
```

## Instrumentation

Let's add some instrumentation to the [mm.cpp](mm.cpp) file. Let's always enable
a wall-clock timer and use the `user_global_bundle` to make runtime injection of
components available. Additionally, let's use the `tim::quirk::auto_start` to
automatically call `start()` when an object is constructed.

### Bundle with custom API and quirk

```cpp
#include <timemory/timemory.hpp>

TIMEMORY_DEFINE_API(mm)

namespace quirk = tim::quirk;
namespace comp  = tim::component;
namespace api   = tim::api;
using bundle_t =
    tim::component_bundle<api::mm, comp::wall_clock, comp::user_global_bundle,
                          quirk::config<quirk::auto_start>>;
// ...

double
execute_matmul(int64_t s, int64_t imax, int64_t nitr)
{
    bundle_t _bundle{ __FUNCTION__ };
    // ...
}
```

Rebuild and validate you see identical outputs:

```console
$ example-00 --help
[example-00] Usage: example-00 [ --help (count: 0, dtype: bool)
                                 --timemory-config (count: unlimited)
                                 --timemory-suppress-config (max: 1, dtype: bool)
                                 --timemory-suppress-parsing (max: 1, dtype: bool)
# etc.
```

```console
$ example-00

Running 25 MM on 250 x 250
MM sum = 16172.9
[wall]|0> Outputting 'timemory-example-00-output/wall.json'...
[wall]|0> Outputting 'timemory-example-00-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-example-00-output/wall.txt'...
```

```console
$ example-00 --timemory-enabled=no --

Running 25 MM on 250 x 250
MM sum = 16172.9
```

```console
$ example-00 --timemory-enabled=no -- 100 10

Running 10 MM on 100 x 100
MM sum = -2991.61
```

```console
$ example-00 --timemory-cout-output=on --

Running 25 MM on 250 x 250
MM sum = 16172.9
[wall]|0> Outputting 'timemory-example-00-output/wall.json'...
[wall]|0> Outputting 'timemory-example-00-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-example-00-output/wall.txt'...

|-----------------------------------------------------------------------------------------------------------------------------|
|                                           REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                          |
|-----------------------------------------------------------------------------------------------------------------------------|
|       LABEL        | COUNT  | DEPTH  | METRIC | UNITS  |   SUM     |   MEAN    |   MIN     |   MAX     |  STDDEV   | % SELF |
|--------------------|--------|--------|--------|--------|-----------|-----------|-----------|-----------|-----------|--------|
| >>> execute_matmul |      1 |      0 | wall   | sec    | 3.824e+00 | 3.824e+00 | 3.824e+00 | 3.824e+00 | 0.000e+00 |  100.0 |
|-----------------------------------------------------------------------------------------------------------------------------|
```

### Global Initialization of `user_global_bundle`

Add the following after `timemory_argparse(...)` in [example.cpp](example.cpp)
to have the `user_global_bundle` re-initialize with the command-line arguments
when provided:

```cpp
// execute global initialization after argument parse
tim::component::user_global_bundle::global_init();
```

Verify the following now adds `peak_rss` and `cpu_clock` reports:

```console
$ example-00 --timemory-global-components peak_rss cpu_clock --

Running 25 MM on 250 x 250
MM sum = 16172.9
[cpu]|0> Outputting 'timemory-example-00-output/cpu.json'...
[cpu]|0> Outputting 'timemory-example-00-output/cpu.tree.json'...
[cpu]|0> Outputting 'timemory-example-00-output/cpu.txt'...

[wall]|0> Outputting 'timemory-example-00-output/wall.json'...
[wall]|0> Outputting 'timemory-example-00-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-example-00-output/wall.txt'...

[peak_rss]|0> Outputting 'timemory-example-00-output/peak_rss.json'...
[peak_rss]|0> Outputting 'timemory-example-00-output/peak_rss.tree.json'...
[peak_rss]|0> Outputting 'timemory-example-00-output/peak_rss.txt'...
```

## Runtime Disabling

In [example.cpp](example.cpp), create a runtime option to disable `api::mm`:

```cpp
TIMEMORY_DECLARE_API(mm)

using argparser_t = tim::argparse::argument_parser;

int main(int argc, char** argv)
{
    // ...

    tim::timemory_init(argc, argv);

    argparser_t _parser{ "example" };
    _parser.add_argument({ "--disable-mm-api" }, "Disable api::mm")
        .max_count(1)
        .action([](argparser_t& p) {
            tim::trait::runtime_enabled<tim::api::mm>::set(!p.get<bool>("disable-mm-api"));
        });

    tim::timemory_argparse(&argc, &argv, &_parser);

    // ...
}
```

Verify it works via:

```console
$ example-00 --disable-mm-api=y --

Running 25 MM on 250 x 250
MM sum = 16172.9
```

## Compile-time Disabling

Add this pre-processor block in [mm.cpp](mm.cpp):

```cpp
TIMEMORY_DEFINE_API(mm)

#if defined(DISABLE_MM_API)
TIMEMORY_DEFINE_CONCRETE_TRAIT(is_available, api::mm, false_type)
#endif
```

In [CMakeLists.txt](CMakeLists.txt), you will find:

```cmake
option(DISABLE_MM_API "Disable api::mm" OFF)
if(DISABLE_MM_API)
    target_compile_definitions(mm PUBLIC DISABLE_MM_API)
endif()
```

Run `./build.sh -DDISABLE_MM_API=ON` and then execute and verify that no component output is enabled:

```console
$ example-00

Running 25 MM on 250 x 250
MM sum = 16172.9
```

## Advanced

### Serialization over MPI

In [example.cpp](example.cpp), insert `MPI_Init` and `MPI_Finalize` directly or use timemory's
generic wrappers:

```cpp
int main(int argc, char** argv)
{
    tim::mpi::initialize(argc, argv);

    // ...

    tim::mpi::finalize();
    return 0;
}
```

In [mm.cpp](mm.cpp), use `operation::serialization<T>` directly to combine the resulting matrix
for each MPI rank and write it to JSON:

```cpp
template <typename Tp>
void
save_data(const char* fname, const char* dlabel, int64_t s, int64_t imax, int64_t nitr,
          const Tp& _data)
{
    using serialize_t = tim::operation::serialization<std::decay_t<Tp>>;
    using mpi_data_t  = typename serialize_t::mpi_data; // for tag dispatch
    auto all_data     = serialize_t{}(mpi_data_t{}, tim::mpi::comm_world_v, _data);

    if(tim::mpi::rank() == 0)
    {
        using json_type = tim::cereal::PrettyJSONOutputArchive;
        auto metadata   = [&](json_type& ar) {
            ar(tim::cereal::make_nvp("s", s), tim::cereal::make_nvp("imax", imax),
               tim::cereal::make_nvp("nitr", nitr));
        };
        tim::generic_serialization<json_type>(fname, all_data, "mm", dlabel, metadata);
    }
}

double
execute_matmul(int64_t s, int64_t imax, int64_t nitr)
{
    // ...
    auto _a = std::vector<double>(s * s, 0.0);

    // ...
    save_data("a.json", "a_matrix", s, imax, nitr, _a);
    return sum;
}
```

The resulting json file from `mpirun -n 2 example-00 2 10 10` should be:

```console
{
 "mm": {
  "s": 2,
  "imax": 10,
  "nitr": 10,
  "a_matrix": [
   [
    2.0041133839265119,
    2.4619752383812267,
    -3.5808951375542548,
    -3.307713067121331
   ],
   [
    2.0041133839265119,
    2.4619752383812267,
    -3.5808951375542548,
    -3.307713067121331
   ]
  ]
 }
}
```

### Sub-measurements

In [mm.cpp](mm.cpp), add sub-measurements:

```cpp
double
execute_matmul(int64_t s, int64_t imax, int64_t nitr)
{
    // ...

    for(int64_t i = 0; i < nitr; ++i)
    {
        bundle_t _ibundle{ "reset" };
        mm_reset(s, a, b, c);

        bundle_t _mbundle{ "mm", quirk::config<quirk::timeline_scope, quirk::stop_last_bundle>{} };
        for(int64_t iter = 0; iter < imax; iter++)
            mm(s, a, b, c);

        bundle_t _sbundle{ "sum", quirk::config<quirk::stop_last_bundle>{} };
        sum += mm_sum(s, a);
    }
    // ...
    return sum;
}
```

```console
$ mpirun -n 2 example-00 && cat timemory-example-00-output/wall.txt

Running 25 MM on 250 x 250

Running 25 MM on 250 x 250
MM sum = 16172.9
MM sum = 16172.9
[wall]|0> Outputting 'timemory-example-00-output/wall.json'...
[wall]|0> Outputting 'timemory-example-00-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-example-00-output/wall.txt'...

|-------------------------------------------------------------------------------------------------------------------------------|
|                                            REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                           |
|-------------------------------------------------------------------------------------------------------------------------------|
|        LABEL         | COUNT  | DEPTH  | METRIC | UNITS  |   SUM     |   MEAN    |   MIN     |   MAX     |  STDDEV   | % SELF |
|----------------------|--------|--------|--------|--------|-----------|-----------|-----------|-----------|-----------|--------|
| |0>>> execute_matmul |      1 |      0 | wall   | sec    | 3.878e+00 | 3.878e+00 | 3.878e+00 | 3.878e+00 | 0.000e+00 |    1.9 |
| |0>>> |_reset        |     10 |      1 | wall   | sec    | 4.534e-03 | 4.534e-04 | 4.043e-04 | 6.596e-04 | 8.230e-05 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 3.726e-01 | 3.726e-01 | 3.726e-01 | 3.726e-01 | 0.000e+00 |  100.0 |
| |0>>> |_sum          |     10 |      1 | wall   | sec    | 5.862e-04 | 5.862e-05 | 5.745e-05 | 6.106e-05 | 1.159e-06 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 4.034e-01 | 4.034e-01 | 4.034e-01 | 4.034e-01 | 0.000e+00 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 3.967e-01 | 3.967e-01 | 3.967e-01 | 3.967e-01 | 0.000e+00 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 3.698e-01 | 3.698e-01 | 3.698e-01 | 3.698e-01 | 0.000e+00 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 3.685e-01 | 3.685e-01 | 3.685e-01 | 3.685e-01 | 0.000e+00 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 3.883e-01 | 3.883e-01 | 3.883e-01 | 3.883e-01 | 0.000e+00 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 3.672e-01 | 3.672e-01 | 3.672e-01 | 3.672e-01 | 0.000e+00 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 3.709e-01 | 3.709e-01 | 3.709e-01 | 3.709e-01 | 0.000e+00 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 3.934e-01 | 3.934e-01 | 3.934e-01 | 3.934e-01 | 0.000e+00 |  100.0 |
| |0>>> |_mm           |      1 |      1 | wall   | sec    | 3.673e-01 | 3.673e-01 | 3.673e-01 | 3.673e-01 | 0.000e+00 |  100.0 |
| |1>>> execute_matmul |      1 |      0 | wall   | sec    | 3.828e+00 | 3.828e+00 | 3.828e+00 | 3.828e+00 | 0.000e+00 |    0.6 |
| |1>>> |_reset        |     10 |      1 | wall   | sec    | 4.383e-03 | 4.383e-04 | 4.043e-04 | 6.208e-04 | 6.670e-05 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 3.722e-01 | 3.722e-01 | 3.722e-01 | 3.722e-01 | 0.000e+00 |  100.0 |
| |1>>> |_sum          |     10 |      1 | wall   | sec    | 6.570e-04 | 6.570e-05 | 5.733e-05 | 1.276e-04 | 2.179e-05 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 4.028e-01 | 4.028e-01 | 4.028e-01 | 4.028e-01 | 0.000e+00 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 3.967e-01 | 3.967e-01 | 3.967e-01 | 3.967e-01 | 0.000e+00 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 3.708e-01 | 3.708e-01 | 3.708e-01 | 3.708e-01 | 0.000e+00 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 3.685e-01 | 3.685e-01 | 3.685e-01 | 3.685e-01 | 0.000e+00 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 3.890e-01 | 3.890e-01 | 3.890e-01 | 3.890e-01 | 0.000e+00 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 3.677e-01 | 3.677e-01 | 3.677e-01 | 3.677e-01 | 0.000e+00 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 3.711e-01 | 3.711e-01 | 3.711e-01 | 3.711e-01 | 0.000e+00 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 3.930e-01 | 3.930e-01 | 3.930e-01 | 3.930e-01 | 0.000e+00 |  100.0 |
| |1>>> |_mm           |      1 |      1 | wall   | sec    | 3.671e-01 | 3.671e-01 | 3.671e-01 | 3.671e-01 | 0.000e+00 |  100.0 |
|-------------------------------------------------------------------------------------------------------------------------------|
```
