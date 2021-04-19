# Custom Components

Timemory offers the flexibility of replacing the entire stack of components being recorded
or adding in your own custom components. This is achieved by replacing the `timemory_create_function`
and `timemory_delete` function with function pointers that you provide. Each function in the library API which leads to components being collected (e.g. `timemory_get_begin_record()`, `timemory_push_region()`, etc.) uses these function to start/stop the components.

```cpp
using toolset_t     = tim::auto_tuple_t<wall_clock>;
using toolset_ptr_t = std::shared_ptr<toolset_t>;
using record_map_t  = std::unordered_map<uint64_t, toolset_ptr_t>;

void
create_record(const char* name, uint64_t* id, int, int*)
{
    auto& _records = timemory_tl_static<record_map_t>();
    *id            = timemory_get_unique_id();
    _records.insert(std::make_pair(*id, std::make_shared<toolset_t>(name)));
}

void
delete_record(uint64_t nid)
{
    auto& _records = timemory_tl_static<record_map_t>();
    // erase key from map which stops recording when object is destroyed
    _records.erase(nid);
}

int main()
{
    timemory_create_function = (timemory_create_func_t) &create_record;
    timemory_delete_function = (timemory_delete_func_t) &delete_record;
    // ... etc. ...
}
```

In the above example `timemory_create_function` and `timemory_delete_function` are over written by our custom create and delete functions. You can note that in the above example we just have `wall_clock` component in our list but you can modify this to include any number of components. You can find the complete example in [example.cpp](example.cpp).

You can build and run this using:

```console
cmake -B build .
cmake --build build --target all
cd build

export TIMEMORY_ENABLED=true
./library_example 40
```

Expected output would look like this:

```console
$ ./library_example 40
Answer = 267914296
[wall]|0> Outputting 'timemory-library-example-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-library-example-output/wall.txt'...

|----------------------------------------------------------------------------------------------------------|
|                                 REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                                 |
|----------------------------------------------------------------------------------------------------------|
|     LABEL      | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|----------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> main/total |      1 |      0 | wall   | sec    |  1.707 |  1.707 |  1.707 |  1.707 |  0.000 |   37.9 |
| >>> |_nfib+1   |      1 |      1 | wall   | sec    |  1.059 |  1.059 |  1.059 |  1.059 |  0.000 |  100.0 |
|----------------------------------------------------------------------------------------------------------|
```
