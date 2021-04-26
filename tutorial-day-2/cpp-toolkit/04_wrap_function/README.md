# Wrapping Functions

> Requires Linux and [GOTCHA](https://github.com/LLNL/GOTCHA) support

Similar to [03_replace_function](../03_replace_function), wrapping external
functions is easy. Timemory will automatically call several functions around
the wrapped function:

```cpp
bundle_type _obj{ _data.tool_id }; // function name
_obj.construct(_args...);
_obj.start();
_obj.audit(_data, audit::incoming{}, _args...);

// call real function
Ret _ret =  ...;

_obj.audit(_data, audit::outgoing{}, _ret);
_obj.stop();
```

## Exercise

In [exercise.cpp](example.cpp), wrap the `set_seed` and `execute_matmul` functions
and audit their function arguments with a custom `argument_logger` component.

```cpp
// argument logger component
struct argument_logger : tim::component::base<argument_logger, void>
{
    template <typename Arg, typename... Args>
    void audit(const tim::component::gotcha_data& _data, tim::audit::incoming, Arg arg,
               Args... args);
};

// gotcha handle for wrapping argument logger around a max of 8 functions
using gotcha_t =
    tim::component::gotcha<8, tim::component_tuple<argument_logger>, tim::api::mm>;

// function which returns a handle to the wrapper
std::unique_ptr<tim::component_tuple<gotcha_t>>
wrap_functions();

int
main(int argc, char** argv)
{
    // ...
    auto _handle = wrap_functions();
    _handle->start();

    set_seed(nseed);
    auto sum = execute_matmul(dsize, nrepeat, nitr);

    _handle->stop();
    // ...
}
```

Instead of explicitly generating the wrappers, wrap them in an initializer:

```cpp
gotcha_t::get_initializer() = []() {
    TIMEMORY_C_GOTCHA(gotcha_t, 0, set_seed);
    TIMEMORY_CXX_GOTCHA(gotcha_t, 1, execute_matmul);
};
return std::make_unique<tim::component_tuple<gotcha_t>>();
```

Using the initializer syntax allows you to control when the functions are wrapped via
the `start()` and `stop()` member functions of `tim::component_tuple<gotcha_t>`.
To activate the function wrappers, call `_handle->start()`. To deactivate the
function wrappers, call `_handle->stop()`.

Store the arguments by their function name via the `tim::manager::add_metadata` function:

```cpp
using gotcha_data = tim::component::gotcha_data;
using incoming    = tim::audit::incoming;

template <typename Arg, typename... Args>
void
argument_logger::audit(const gotcha_data& _data, incoming, Arg arg, Args... args)
{
    std::cout << "logging arguments for " << _data.tool_id << std::endl;
    if(sizeof...(Args) > 0)
        tim::manager::add_metadata(_data.tool_id, std::make_tuple(arg, args...));
    else
        tim::manager::add_metadata(_data.tool_id, arg);
}
```

A successful implementation will have the following entries in `<output-folder>/metadata.json`:

```console
{
 "timemory": {
  "metadata": {
   "info": {
    ...
    "set_seed": 5464515,
    "execute_matmul(long, long, long)": {
     "tuple_element0": 250,
     "tuple_element1": 25,
     "tuple_element2": 10
    },
   },
```

and the following output:

```console
$ example-04
logging arguments for set_seed
logging arguments for execute_matmul(long, long, long)

Running 25 MM on 250 x 250
MM sum = 33711.2
[wall]|0> Outputting 'timemory-example-04-output/wall.json'...
[wall]|0> Outputting 'timemory-example-04-output/wall.tree.json'...
[wall]|0> Outputting 'timemory-example-04-output/wall.txt'...
```

## Additional Exercises

Create another component which looks for any pointers in the arguments
and checks for null pointers:

```cpp
struct pointer_checker : tim::component::base<pointer_checker, void>
{
    using data_t = tim::component::gotcha_data;

    template <typename T>
    void check(const data_t&, T v,
               std::enable_if_t<std::is_pointer<std::decay_t<T>>::value, int> = 0)
    {
        if(v == nullptr)
        {
            auto _bt = tim::get_backtrace<24>();
            std::cerr << "Backtrace:\n";
            for(auto itr : _bt)
            {
                if(itr && strlen(itr) > 0)
                    std::cerr << "    " << itr << "\n";
            }
            throw std::runtime_error("found null pointer");
        }
    }

    template <typename T>
    void check(const data_t&, T&&,
               std::enable_if_t<!std::is_pointer<std::decay_t<T>>::value, int> = 0)
    {}

    template <typename... Args>
    void audit(const data_t& _data, tim::audit::incoming, Args&&... args)
    {
        std::cout << "checking arguments for " << _data.tool_id << std::endl;
        TIMEMORY_FOLD_EXPRESSION(check(_data, std::forward<Args>(args)));
    }
};
```
