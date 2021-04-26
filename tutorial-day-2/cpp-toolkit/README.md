# C++ Toolkit Tutorial

In order to use timemory as a toolkit, it is important to understand how
composing operations on components works. A component can, in general,
do anything and you are free to mix in any and all of your own components.
Here are some examples of various things timemory-provided components do
aside from the obvious types for performance measurements:

- forward markers to third-party profilers (e.g. TAU, Caliper, NVTX, VTune)
- turn third-party profilers on and off
- wrap externally-defined functions with other components
  - e.g. calling `MPI_Allreduce` anywhere will actually bundle timemory calls around `MPI_Allreduce`
- replace the definition of externally-defined functions
  - e.g. `cos` anywhere will actually call `cosf`
- operate on the arguments and return values of wrapped function calls
- define a fixed interface for executing callbacks

## Components

Key characteristics of components:

- Recommend inheriting from template base class
  - `base<ComponentT, ValueT>`
    - e.g. `struct foo : base<foo, int64_t>`
  - The data type is how you want to store intermediate data
- Define one measurement instance
- Usually store configuration data in static member functions
- Only member function __names__ matter (in general)
  - `double get() const`
  - `std::vector<double> get() const`
  - `void start()`
  - `void start(int)`
- Can implement other components
- Components should not directly add themselves to "storage"
  - The idea is that components can be used for intermediate measurements

When you need to store configuration info for the component, use a static
function which returns a reference or pointer instead of a static variable.
This style ensures there are no static initialization fiasco.

### Bad Static Initialization

```cpp
struct Foo
{
    static FooConfig config;
};

FooConfig Foo::config = {};
```

### Good Static Initialization

```cpp
struct Foo
{
    static FooConfig& get_config()
    {
        static FooConfig config;
        return config;
    }
};
```

## Overview of Bundlers

These classes essentially provide a common way to call all the components.
They use "operations" to figure out how to call components.
There are four primary type of bundlers:

1. `component_tuple<T...>`
   - these component types are "always-on"
   - data-type: `std::tuple<T...>`
   - example: `component_tuple<wall_clock, cpu_clock>`
     - both components will always be collected
   - `start(args..)` and `stop(args...)` implicitly add components to global storage
2. `component_list<T...>`
   - these component types are "optional"
   - data-type: `std::tuple<T*...>`
   - example: `component_list<wall_clock, cpu_clock>`
     - these components will activate based on initializer for type and/or initializer passed to ctor
   - `start(args..)` and `stop(args...)` implicitly add components to global storage
3. `component_bundle<ApiT, T...>`
   - these component types combine capabilities of `component_tuple` and `component_list`
   - Specify "always-on" components normally, e.g. `wall_clock`
   - Specify "optional" components with pointer, e.g. `wall_clock*`
   - example: `component_bundle<project::timemory, wall_clock, cpu_clock*>`
     - if `project::timemory` is marked as unavailable at compile-time or run-time, nothing will be collected
     - `wall_clock` will always be activated (when `ApiT` is active)
     - `cpu_clock` will be active based on initializer for type and/or initializer passed to ctor
   - `start(args..)` and `stop(args...)` implicitly add components to global storage
4. `lightweight_tuple<T...>`
   - essentially same as `component_tuple<T...>` but do not automatically push/pop to storage

There are also `auto_tuple<T...>`, `auto_list<T...>`, and `auto_bundle<ApiT, T...>` classes.
There are essentially the same as the `component_*` types except they automatically call
`start()` in their constructor. All types will call `stop()` in their destructor if there
were started.

### Bundler Examples

Below provides a comparison of using different bundlers to perform the same set of operations.
Assume the following:

```cpp
using tim::component::wall_clock;
using tim::component::cpu_clock;
using tim::component::user_global_bundle;
namespace scope = tim::scope;

#define TIMEMORY_API tim::project::timemory
```

#### Component Tuple

```cpp
using bundle_t = tim::component_tuple<wall_clock, cpu_clock, user_global_bundle>;
void foo()
{
    bundle_t _bundle{ "foo" };
    _bundle.start();
    // ...
    _bundle.stop();  // not strictly necessary at end of variable scope
}
```

#### Component List

```cpp
using bundle_t = tim::component_list<wall_clock, cpu_clock, user_global_bundle>;

void foo()
{
    // applies to all instances
    bundle_t::get_initializer() = [](bundle_t& v) { v.initialize<wall_clock>(); };
    // applies to local instance (when passed to ctor)
    auto _local_init = [](bundle_t& v) { v.initialize<cpu_clock, user_global_bundle>(); };

    bundle_t _bundle{ "foo", scope::get_default(), _local_init };
    _bundle.start();
    // ...
    _bundle.stop();  // not strictly necessary at end of variable scope
}
```

#### Component Bundle

```cpp
// wall-clock is "always-on"
// user_global_bundle is "optional"
using bundle_t = tim::component_bundle<TIMEMORY_API, wall_clock, user_global_bundle*>;

void foo()
{
    // configure user_global_bundle to collect cpu_clock
    // applies to all instances of user_global_bundle
    user_global_bundle::configure<cpu_clock>();
    // applies to local instance (when passed to ctor)
    auto _local_init = [](bundle_t& v) { v.initialize<user_global_bundle>(); };

    bundle_t _bundle{ "foo", scope::get_default(), _local_init };
    _bundle.start();
    // ...
    _bundle.stop();  // not strictly necessary at end of variable scope
}
```

#### Lightweight Tuple

```cpp
using bundle_t = tim::lightweight_tuple<wall_clock, cpu_clock, user_global_bundle>;

void foo()
{
    bundle_t _bundle{ "foo" };
    _bundle.push();
    _bundle.start();
    // ...
    _bundle.stop();
    _bundle.pop();
}
```

## Bundler Member Functions

Nearly all bundler member functions are defined like this:

```cpp
template <typename... Args>
this_type& sample(Args&&...);
{
    return invoke<operation::sample>(std::forward<Args>(args)...);
}
```

This essentially means that the member functions take any and all arguments
and make them available to the components when calling the sample member function
of the components.

For example, given these excerpts from the following components:

```cpp
struct Foo
{
    void sample();
};

struct Bar
{
    template <size_t N>
    void sample(const std::array<const char*, N>& _call_stack);
};

struct Spam
{
    void start();
    void stop();
};
```

you can bundle together `Foo`, `Bar`, and `Spam` with `component_tuple<Foo, Bar, Spam>` and
then call:

```cpp
void signal_handler(int sig)
{
    using bundle_t = component_tuple<Foo, Bar, Spam>;
    static bundle_t _bundle{};

    // backtrace of 8 frames
    std::array<const char*, 8> _bt = tim::get_backtrace<8>();

    _bundle.sample(_bt);
}
```

and timemory will call:

- `Foo::sample()`
- `Bar::sample(const std::array<const char*, 8>&)`

You may ask yourself the following questions:

1. Why is `Foo::sample()` called when the arguments are an array?
2. What about `Spam`? It doesn't have a `sample(...)` member function
3. What if I just wanted to only call the `Bar::sample(...)` and not `Foo::sample()`?

The answer to #1 and #2 require understanding how bundlers use "operations" to
figure out how to call components for a given set of arguments.
The answer to #3 is to pass `tim::mpl::piecewise_select<Bar>{}` as the first argument,
e.g. `_bundle.sample(piecewise_select<Bar>{}, _bt)`.

## Operations

Operation classes define how to execute an operation for a given component. These
are templated on the component type and, generally, take a component instance as
the first argument to their constructor and/or their function call operator.
Operation classes then perform a series of compile-time checks to "figure out"
whether the component member function can (A) be called with the given set of arguments,
(B) be called with no arguments, or (C) not called at all. These checks rely on
a mix of SFINAE (substitution failure is not an error) and overload resolution rules.

### SFINAE

If you aren't familar with SFINAE, you can read about it
[here on cppreference.com](https://en.cppreference.com/w/cpp/language/sfinae).
__The gist is that it allows different function overloads to be selected at compile-time.__
Operations use what is called "expression SFINAE" -- this means it actually
forms the function call expression at compile-time and tests whether it is
a valid expression, which in the case of `Foo::sample()`, calling `sample(std::array<...>)`
is an invalid expression and, in the case of `Spam`, calling any member function
named `sample`, is an invalid expression.

### Overload Resolution Rules

C++ has rules for how it determines the "best match" for a given set of overloads.
C++ reference has a detailed guide [here](https://en.cppreference.com/w/cpp/language/overload_resolution).
The easiest to understand is for integer type overloads. For example, given the following
functions:

```cpp
void foo(int);
void foo(long);
```

If you call, `foo(0)`, the best match is `foo(int)` whereas with `foo(0L)`, the best match
is `foo(long)` but `foo(0UL)` is ambiguous. In other words, `foo(0)` and `foo(0L)` are exact matches
but `foo(0UL)` requires a conversion. If only `foo(long)` were to exist, `foo(0)` would undergo
as simple promotion to a `long`. __Thus, we can use overload resolution to define
the order of overload selection and use SFINAE to remove overloads from the selection
set.__

### Operation Example

This is slightly simplified definition of `tim::operation::sample<T>` class:

```cpp
namespace tim
{
namespace operation
{
template <typename Tp>
struct sample
{
    // constructor
    template <typename... Args>
    sample(Tp& obj, Args&&... args)
    {
        sfinae(obj, 0, std::forward<Args>(args)...)
    }

    // function call operator
    template <typename... Args>
    auto operator()(Tp& obj, Args&&... args)
    {
        return sfinae(obj, 0, std::forward<Args>(args)...)
    }

private:
    // has function and accepts arguments
    template <typename Up, typename... Args>
    auto sfinae(Up& obj, int, Args&&... args)                   // INT
        -> decltype(obj.sample(std::forward<Args>(args)...))    // SFINAE test
    {
        return obj.sample(std::forward<Args>(args)...);
    }

    // has function but does not accept arguments
    template <typename Up, typename... Args>
    auto sfinae(Up& obj, long, Args&&...)                       // LONG
        -> decltype(obj.sample())                               // SFINAE test
    {
        return obj.sample();
    }

    //  failed all previous tests
    template <typename Up, typename... Args>
    void sfinae(Up&, long long, Args&&...)                      // LONG LONG
    {}
};
}  // namespace operation
}  // namespace tim
```

In the case of `Foo`, the first overload of `sfinae` is invalid because
`decltype(obj.sample(std::forward<Args>(args)...))` is an invalid expression but
both the second and third overloads of the function `sfinae` are valid.
The second overload is selected because `0 -> long` is a smaller promotion
than `0 -> long long`. In the case of `Bar`, all three overloads are
valid but `0 -> int` is an exact match. Finally, in the case of `Spam`,
neither the first or second overloads of `sfinae` are valid and thuse the
third (which does nothing), is selected.

## Putting It All Together

Let's look at the line:

```cpp
_bundle.sample(_bt);
```

Assuming `_bundle` has a member variable `std::tuple<Foo, Bar, Spam>` called `data`,
this above line essentially expands to:

```cpp
operation::sample<Foo> { get<0>(data), _bt };
operation::sample<Bar> { get<1>(data), _bt };
operation::sample<Spam>{ get<2>(data), _bt };
```

and after the compile-time checks within `operation::sample<T>`, we essentially have:

```cpp
Foo& _foo = std::get<0>(data);
Bar& _bar = std::get<1>(data);

_foo.sample(bt);
_bar.sample();
```

## Additional Notes

- If you want to call a member function without pre-defined support, you have two options:
  1. Define your own operation classes and use the generic `invoke<OpT>(args...)` on bundles
     - e.g. `_bundle.invoke<operation::MyOperation>(args...)`
  2. Create an explicit specialization for an existing operation and a given type
     - e.g. `operation::start<MyComponent>` calling another member function
- Bundle operations are actually passed through `operation::generic_operation<T, OpT, ApiT>` where `T` is the component (e.g. `Foo`), `OpT` is the operation type (e.g. `sample<Foo>`), and `ApiT` is a common type such as the aforementioned `project::timemory`
  - This provides various features, such as checking for null pointers before dereferencing them in `OpT`
  - If you are using a non-standard `ApiT`, you can partially specialize this class to modify the behavior of all components using this API tag

## Exercises

- [00_basic_usage](00_basic_usage.md) will have you add various components
- [01_lightweight_usage](01_lightweight_usage.md) will have you collect temporary data and add it into another instance
- [02_normalization_component](02_normalization_component.md) will have you create a new component which derives data from other components and reports values independent of the problem size
- [03_replace_function](03_replace_function.md) will have you replace an externally-defined function with another implementation
- [04_wrap_function](04_wrap_function.md) will have you wrap a function
