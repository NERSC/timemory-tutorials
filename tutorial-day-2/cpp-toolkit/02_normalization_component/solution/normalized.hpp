
#include "timemory/mpl/math.hpp"
#include "timemory/mpl/stl.hpp"
#include <timemory/components.hpp>
#include <timemory/components/base.hpp>
#include <timemory/mpl/types.hpp>

// for overload resolution, e.g. foo.store(normalize_tag{}, size_t)
struct normalization
{};

namespace tim
{
namespace component
{
template <typename T>
struct normalized;
}
}  // namespace tim

namespace tim
{
namespace trait
{
// set the derivation types
template <typename T>
struct derivation_types<component::normalized<T>>
{
    using type = type_list<type_list<T>>;
};
//
template <typename T>
struct statistics<component::normalized<T>>
{
    using type = typename statistics<T>::type;
};
//
template <typename T>
struct is_available<component::normalized<T>>
{
    using type                  = typename is_available<T>::type;
    static constexpr auto value = is_available<T>::value;
};
//
template <typename T>
struct base_has_accum<component::normalized<T>> : false_type
{};
//
template <typename T>
struct echo_enabled<component::normalized<T>> : false_type
{};
}  // namespace trait
}  // namespace tim

namespace tim
{
namespace component
{
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

    static const std::ios::fmtflags format_flags =
        std::ios_base::scientific | std::ios_base::dec | std::ios_base::showpoint;

    void store(normalization&&, size_t _sz);
    auto get() const;
    auto get_display() const;
    bool derive(T* comp);

private:
    size_t data_size = 1;
};

template <typename T>
std::string
normalized<T>::label()
{
    return TIMEMORY_JOIN("_", "normalized", T::label());
}

template <typename T>
std::string
normalized<T>::description()
{
    return TIMEMORY_JOIN(" ", T::description(), "normalized to data size");
}

template <typename T>
auto
normalized<T>::unit()
{
    return T::unit();
}

template <typename T>
auto
normalized<T>::display_unit()
{
    return T::display_unit();
}

template <typename T>
void
normalized<T>::store(normalization&&, size_t _sz)
{
    data_size = _sz;
}

template <typename T>
auto
normalized<T>::get() const
{
    // below might not be valid for more complex components
    return get_value().get() / data_size;

    // this will be valid for more complex components
    // auto _v            = get_value().get();
    // using compute_type = math::compute<decay_t<decltype(_v)>>;
    // compute_type::divide(_v, data_size);
    // return _v;
}

template <typename T>
auto
normalized<T>::get_display() const
{
    auto _v = get_value();
    // below might not be valid for more complex components
    _v /= data_size;

    // this will be valid for more complex components
    // using compute_type = math::compute<T>;
    // compute_type::divide(_v, data_size);

    return _v.get_display();
}

template <typename T>
bool
normalized<T>::derive(T* comp)
{
    if(comp)
        base_type::value = *comp;
    return (comp != nullptr);
}

}  // namespace component
}  // namespace tim
