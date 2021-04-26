
#pragma once

#include <random>
namespace common
{
    std::default_random_engine& get_rng();
    //
    double generate_canonical();
    //
    template <typename T>
    T
    generate_random_in_range(T _scale);
}

extern template double
common::generate_random_in_range<double>(double);
//
extern template float
common::generate_random_in_range<float>(float);
