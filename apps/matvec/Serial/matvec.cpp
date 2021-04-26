

#include <arrayMD.h>
#include <interface.hpp>
#include <common.hpp>

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

extern template class ArrayMD<double, 2, Device::cpu>;
extern template class ArrayMD<double, 3, Device::cpu>;

template <typename T>
using matrix_t = ArrayMD<T, 2, Device::cpu>;

template <typename T>
using cube_t = ArrayMD<T, 3, Device::cpu>;

template <typename T>
auto
dot(T* m, T* x, int N)
{
    T result{};
    for(int k = 0; k < N; ++k)
        result += m[k] * cos(x[k]);
    return result;
}

template <typename T>
void
matvec(int i, cube_t<T>& m, matrix_t<T>& x, T* y, int N)
{
    for(int j = 0; j < N; ++j)
        y[j] += dot(m.subArray(i, j), x.subArray(i), N);
}

template <typename T>
void
batched_matrix_vector(cube_t<T>& m, matrix_t<T>& x, matrix_t<T>& y, int N)
{
    for(int i = 0; i < N; ++i)
        matvec(i, m, x, y.subArray(i), N);
}

using data_type = double;

void
driver(int N, int repeat)
{
    tools::perf_data pd{ "initialize time" };
    pd.start().store(tools::normalize_tag{}, N * N * N);

    matrix_t<data_type> y(N, N);
    matrix_t<data_type> x(N, N);
    cube_t<data_type>   m(N, N, N);

    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            x(i, j)  = common::generate_random_in_range<double>(M_PI);
            y(i, j)  = 0.0;
            auto val = common::generate_canonical();
            for(int k = 0; k < N; ++k)
            {
                m(i, j, k) = exp(val);
            }
        }
    }

    pd.stop()
        .print()
        .reset("compute time")
        .start()
        .store(tools::normalize_tag{}, repeat * N * N * N);

    for(int i = 0; i < repeat; ++i)
        batched_matrix_vector<data_type>(m, x, y, N);

    pd.stop().print().reset("output time").start().store(tools::normalize_tag{}, N * N);

    // Print out the output. Comment out unless needed.
    std::ofstream ofs{ "matvec-serial.dat" };
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
            ofs << "y(" << i << ", " << j << ") = " << std::setprecision(6) << std::fixed
                << y(i, j) << "\n";
    }

    pd.stop().print();
}

int
main(int argc, char** argv)
{
    std::cout << "Running the basic sequential version." << std::endl;
    tools::initialize(argc, argv);

    int N      = 250;
    int repeat = 100;
    if(argc > 1)
        N = std::stoi(argv[1]);
    if(argc > 2)
        repeat = std::stoi(argv[2]);

    tools::add_metadata("N", N);
    tools::add_metadata("iterations", repeat);

    tools::perf_data pd{ "driver" };
    pd.start().store(tools::normalize_tag{}, repeat * N * N * N);

    driver(N, repeat);

    pd.stop().print();

    tools::finalize();
    return 0;
}
