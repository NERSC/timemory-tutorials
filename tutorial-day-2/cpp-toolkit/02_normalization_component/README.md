# Normalization Component

In the exercise, we are going to create a component which takes another component as
a template parameter and normalizes it's data based on a data-size provided in the application.
This is useful for creating a metric reporting system which is independent of the problem size.

For example, if we are use a `wall_clock` component to time a loop over data with size N, we
can define `normalized<wall_clock>` as part of the bundle, provide N to this component,
and have `normalized<wall_clock>` report the time per data size.

## Deriving Component Data

Components can set a type-trait (`trait::derivation_types`) and provide `derive` member functions in
order to extract data from an component it is bundled alongside. The type-trait is a type-list
of type-list (i.e. `type_list<type_list<...>, ...>`) where the inner type-lists are the
set of components that need to be present in order for the derivation to happen, e.g.
`cpu_util` can derive from `wall_clock` and `cpu_clock` or `monotonic_clock` and `cpu_clock`, thus
it's derivation types would be:

```cpp
using type = type_list<
                type_list<wall_clock, cpu_clock>,
                type_list<monotonic_clock, cpu_clock>
             >;
```

## Exercise

We are going to take a slightly modified version of the [00_basic_usage solution](../00_basic_usage).
The modifications are primarily removing the `quirk::auto_start` usage so that we can
call `<obj>.store(normalization{}, N)` before calling `start()`.

```cpp
using bundle_t =
    tim::component_bundle<api::mm,
                          comp::wall_clock,
                          // quirk::config<quirk::auto_start>, // REMOVED
                          comp::user_global_bundle>;
//

bundle_t _ibundle{ "reset" };
_ibundle.store(normalization{}, s * s).start();
```

The `normalization` struct is a dummy type used to further demo that timemory can take any data-type provided by the user.
It's definition is trivial:

```cpp
struct normalization
{};
```

### Forward Declaration

In [components.hpp](components.hpp), forward declare the component:

```cpp
namespace tim
{
namespace component
{
template <typename T>
struct normalized;
}
}  // namespace tim
```

This can be declared with the helper macro: `TIMEMORY_DECLARE_TEMPLATE_COMPONENT(normalized, typename T)`.

### Type-traits

In [components.hpp](components.hpp), specialize the type-traits in the `tim::trait` namespace:

1. `derivation_types`
   - set the `type`
2. `statistics`
   - derive the `type` from `statistics<T>`
3. `is_available`
   - derive the `type` and `value` from `is_available<T>`
4. `base_has_accum`
   - inherit from `false_type`
   - this removes the `accum` member value from the base class, leaving only `T value`

The default implementation of `trait::statistics` is:

```cpp
template <typename T>
struct statistics
{
    using type = void;
};
```

Timemory looks up the statistics type via `typename trait::statistics<T>::type`. Since
`T` in this case is `component::normalized<T>`, instead of explicitly defining `type`
like we might for `wall_clock`:

```cpp
template <>
struct statistics<component::wall_clock>
{
    using type = double;
};
```

we define `statistics<normalized<T>>` by looking up `typename statistics<T>::type`:

```cpp
template <typename T>
struct statistics<component::normalized<T>>
{
    using type = typename statistics<T>::type;
};
```

We repeat this form of type-dependent specialization for the remainder of our type-traits:

```cpp
template <typename T>
struct derivation_types<component::normalized<T>>
{
    using type = type_list<type_list<T>>;
};

template <typename T>
struct is_available<component::normalized<T>>
{
    using type                  = typename is_available<T>::type;
    static constexpr auto value = is_available<T>::value;
};

template <typename T>
struct base_has_accum<component::normalized<T>> : false_type
{};
```

### Component Declaration

#### Metadata

We provide 4 metadata functions and 1 metadata value:

- `static std::string label()`
  - single word identifier for component
  - hint: a "normalized" to `T::label()`
- `static std::string description()`
  - description of component
  - hint: add "normalized" to `T::description()`
- `static auto unit()`
  - e.g. `unit::sec`, `unit::megabyte`, etc.
  - hint: return `T::unit()`
- `static auto display_unit()`
  - e.g. "sec", "MB", etc.
  - hint: return `T::display_unit()`
- `static const std::ios::fmtflags format_flags`
  - Since we are potentially dividing by large numbers, we want to default to using scientific notation
  - hint: return `std::ios_base::scientific`

```cpp
template <typename T>
struct normalized : public base<normalized<T>, T>
{
    using value_type = T;
    using base_type  = base<normalized<T>, value_type>;

    using base_type::get_value;

    static std::string label();
    static std::string description();
    static auto        unit();
    static auto        display_unit();

    static const std::ios::fmtflags format_flags = ...;

    // ...
};
```

#### Storing Data Size

We know from inheriting from `base<normalized<T>, T>` that `value` will be of type `T` but
we need `normalized<T>` to store the data size too. Thus, we add this as a member value
and provide the aforementioned `store(...)` member function:

```cpp
template <typename T>
struct normalized : public base<normalized<T>, T>
{
    // ...

    void store(normalization&&, size_t _sz);

private:
    size_t data_size = 1;
};
```

In the implementation of `store`, assign `_sz` to `data_size`.

#### Remaining Member Functions

In general, we need three more member functions:

```cpp
template <typename T>
struct normalized : public base<normalized<T>, T>
{
    // ...

    auto get() const;
    auto get_display() const;
    bool derive(T* comp);

    // ...
};
```

The `get()` member function needs to return `value.get() / data_size` (e.g. `wall_clock.get() / data_size`).
In `get_display()`, we want to create a copy of `value`, divide by `data_size` and then call `get_display()`
on our copy because some more complex components will return a string from `get_display()`. E.g.

```cpp
auto _v = value;
_v /= data_size;
return _v.get_display();
```

In `derive(T* v)` we simply want to check that is not a null pointer and copy it into `base_type::value`:

```cpp
if(v)
    base_type::value = *v;
```

### Usage in [mm.cpp](mm.cpp)

Now add in `comp::normalized<comp::wall_clock>` to your bundle:

```cpp
using bundle_t =
    tim::component_bundle<api::mm,
                          comp::wall_clock,
                          comp::normalized<comp::wall_clock>,
                          comp::user_global_bundle>;
using stop_last_config_t = quirk::config<quirk::stop_last_bundle>;
```

and modify the instrumentation to call `.store(...).start()`:

```cpp
double
execute_matmul(int64_t s, int64_t imax, int64_t nitr)
{
    bundle_t _bundle{ __FUNCTION__ };
    _bundle
        .store(normalization{},
               nitr * ((s * s) + (s * s * s * imax) + (s * s)))
        .start();

    // ...
    for(int64_t i = 0; i < nitr; ++i)
    {
        bundle_t _ibundle{ "reset" };
        _ibundle.store(normalization{}, s * s).start();
        // ...

        bundle_t _mbundle{ "mm", stop_last_config_t{} };
        _mbundle.store(normalization{}, s * s * s * imax).start();
        // ...

        bundle_t _sbundle{ "sum", stop_last_config_t{} };
        _sbundle.store(normalization{}, s * s).start();
        // ...
    }

    return sum;
}
```

You can verify that normalization now provides relatively consistent values
as you modify the data size:

```console
$ TIMEMORY_OUTPUT_PREFIX=00 example-02 200 10

Running 10 MM on 200 x 200
MM sum = 4188.21
[normalized_wall]|0> Outputting 'timemory-example-02-output/00-normalized_wall.json'...
[normalized_wall]|0> Outputting 'timemory-example-02-output/00-normalized_wall.tree.json'...
[normalized_wall]|0> Outputting 'timemory-example-02-output/00-normalized_wall.txt'...

[wall]|0> Outputting 'timemory-example-02-output/00-wall.json'...
[wall]|0> Outputting 'timemory-example-02-output/00-wall.tree.json'...
[wall]|0> Outputting 'timemory-example-02-output/00-wall.txt'...

$ TIMEMORY_OUTPUT_PREFIX=01 example-02 400 20

Running 20 MM on 400 x 400
MM sum = 149771
[normalized_wall]|0> Outputting 'timemory-example-02-output/01-normalized_wall.json'...
[normalized_wall]|0> Outputting 'timemory-example-02-output/01-normalized_wall.tree.json'...
[normalized_wall]|0> Outputting 'timemory-example-02-output/01-normalized_wall.txt'...

[wall]|0> Outputting 'timemory-example-02-output/01-wall.json'...
[wall]|0> Outputting 'timemory-example-02-output/01-wall.tree.json'...
[wall]|0> Outputting 'timemory-example-02-output/01-wall.txt'...

```

When I ran the solution, `example-02 200 10` runs for ~0.7 seconds
and `example-02 400 20` runs for ~12.5 seconds but the normalized metrics
are all within 10% of each other:

```console
$ cat timemory-example-02-output/00-normalized_wall.txt
|----------------------------------------------------------------------------------------------------------------------------------------|
|                                    REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER) NORMALIZED TO DATA SIZE                                    |
|----------------------------------------------------------------------------------------------------------------------------------------|
|        LABEL         | COUNT  | DEPTH  |     METRIC      | UNITS  |   SUM     |   MEAN    |   MIN     |   MAX     |  STDDEV   | % SELF |
|----------------------|--------|--------|-----------------|--------|-----------|-----------|-----------|-----------|-----------|--------|
| |0>>> execute_matmul |      1 |      0 | normalized_wall | sec    | 9.260e-10 | 9.260e-10 | 9.260e-10 | 9.260e-10 | 0.000e+00 |    0.0 |
| |0>>> |_reset        |     10 |      1 | normalized_wall | sec    | 6.771e-08 | 6.771e-09 | 6.327e-09 | 9.514e-09 | 9.675e-10 |  100.0 |
| |0>>> |_mm           |     10 |      1 | normalized_wall | sec    | 9.223e-09 | 9.223e-10 | 8.774e-10 | 1.119e-09 | 7.200e-11 |  100.0 |
| |0>>> |_sum          |     10 |      1 | normalized_wall | sec    | 9.196e-09 | 9.196e-10 | 8.966e-10 | 9.481e-10 | 1.611e-11 |  100.0 |
|----------------------------------------------------------------------------------------------------------------------------------------|
$ cat timemory-example-02-output/01-normalized_wall.txt
|----------------------------------------------------------------------------------------------------------------------------------------|
|                                    REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER) NORMALIZED TO DATA SIZE                                    |
|----------------------------------------------------------------------------------------------------------------------------------------|
|        LABEL         | COUNT  | DEPTH  |     METRIC      | UNITS  |   SUM     |   MEAN    |   MIN     |   MAX     |  STDDEV   | % SELF |
|----------------------|--------|--------|-----------------|--------|-----------|-----------|-----------|-----------|-----------|--------|
| |0>>> execute_matmul |      1 |      0 | normalized_wall | sec    | 9.859e-10 | 9.859e-10 | 9.859e-10 | 9.859e-10 | 0.000e+00 |    0.0 |
| |0>>> |_reset        |     10 |      1 | normalized_wall | sec    | 7.142e-08 | 7.142e-09 | 6.195e-09 | 1.112e-08 | 1.479e-09 |  100.0 |
| |0>>> |_mm           |     10 |      1 | normalized_wall | sec    | 9.850e-09 | 9.850e-10 | 9.658e-10 | 1.013e-09 | 1.443e-11 |  100.0 |
| |0>>> |_sum          |     10 |      1 | normalized_wall | sec    | 9.548e-09 | 9.548e-10 | 9.109e-10 | 9.716e-10 | 1.750e-11 |  100.0 |
|----------------------------------------------------------------------------------------------------------------------------------------|
```
