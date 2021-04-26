
#include "common.hpp"

template <typename T>
constexpr T one = T(1.0);

template <typename T>
constexpr T two = T(2.0);

namespace common
{
//
std::default_random_engine&
get_rng()
{
    static auto _rng = []()
    {
        std::default_random_engine _tmp{};
        _tmp.seed(256532453);
        return _tmp;
    }();
    return _rng;
}
//
double
generate_canonical()
{
    return std::generate_canonical<double, 16>(get_rng());
}
//
template <typename T>
T generate_random_in_range(T _scale)
{
    return _scale * (two<T> * std::generate_canonical<T, 16>(get_rng()) - one<T>);
}
//
}

template double
common::generate_random_in_range<double>(double);
//
template float
common::generate_random_in_range<float>(float);
