# Replace Function

> Requires Linux and [GOTCHA](https://github.com/LLNL/GOTCHA) support

Replacing externally defined functions with a new implementations via timemory is easy.
You need two things:

1. the function signature, i.e. return type and arguments
2. the name of the symbol in the binary

For functions with C linkage, #2 is easy: it is simply the name of the function, e.g.
`double cos(double)` will exist as `"cos"` in the binary. For functions with C++
linkage, you might need to look up the name of the function via `nm`. If the
C++ function is not templated, timemory can usually determine the mangled name.
For example, the mangled symbol names for:

```cpp
double get_random();
namespace foo
{
    double get_other_random();
}
```

can be found via:

```console
$ nm build.sh-03_replace_function/libmm.so | egrep 'get_random|get_other_random'
0000000000009c10 T _Z10get_randomv
0000000000009cc0 T _ZN3foo16get_other_randomEv
```

where `_Z10get_randomv` and `_ZN3foo16get_other_randomEv` are the symbol names.

## Function replacement component

First, inherit from `tim::concepts::component` and define the `value_type` and `base_type`
aliases to void. Then provide an `operator()` whose return value and arguments match
the function you wish to replace. In the case of `get_random()`, this is returning a double
and no arguments:

```cpp
struct replace_get_random : tim::concepts::component
{
    using value_type = void;
    using base_type  = void;

    double operator()()
    {
        static std::default_random_engine rng{};
        return -1.0 * std::generate_canonical<double, 16>(rng);
    }
};
```

Then create a separate function you can (optionally) call which defines a `gotcha` component
and generates the wrapper. The template parameters for a replacement gotcha are:

1. The maximum number of functions that will be replaced
2. The components to wrap around the function (this must be empty)
3. The component type which whose functor will replace the function

```cpp
void replace_function()
{
    static constexpr size_t Nmax = 1;
    using gotcha_t = tim::component::gotcha<Nmax, std::tuple<>, replace_get_random>;
    // ... the following are all equivalent
    TIMEMORY_CXX_GOTCHA(gotcha_t, 0, get_random);
    TIMEMORY_DERIVED_GOTCHA(gotcha_t, 0, get_random, "_Z10get_randomv");
    gotcha_t::template instrument<0, double>::generate("_Z10get_randomv");
    // 0 above is the index. It must be less than Nmax
}
```

The `TIMEMORY_CXX_GOTCHA` macro deduces the return type and arguments from `get_random`
and attempts to generate the mangled function name ("_Z10get_randomv").
The `TIMEMORY_DERIVED_GOTCHA` macro deduces the return type and arguments from `get_random`
but instead of try to generate the mangled function name, it uses the string provided
by the user. The `gotcha_t::template instrument<0, double>::generate` is the
raw invocation. The first template parameter is the index, the second template parameter
is the return type, and other template parameters are the function argument types.

## Exercise

Create an option in [example.cpp](example.cpp) which allows you to use `--replace-get-random`
from the command-line and when it is true, you call `replace_function()`.
Drop this into the definition of `double operator()()` to print a message the first time
it executes:

```cpp
static bool _once = (puts("using replace_get_random"), true);
(void) _once;
```

Your option should look similar to the `--disable-mm-api` option but you may need to
capture your variable into the lambda:

```cpp
// ...

bool _replace = false;
_parser.add_argument(...)
    .action([&_replace](argparser_t& p) { ... };

// ...
tim::timemory_init(...);

if(_replace)
    replace_function();
```

The value of `MM sum` should be changed when used:

```console
$ example-03 --replace-get-random --

Running 25 MM on 250 x 250
using replace_get_random
MM sum = 7.8134e+09
```
