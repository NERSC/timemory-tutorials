
#include <Kokkos_Core.hpp>

#include <interface.hpp>
#include <common.hpp>

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

using team_policy = Kokkos::TeamPolicy<Kokkos::DefaultExecutionSpace>;
using member_type = team_policy::member_type;

template <typename T>
using vector_t = Kokkos::View<T*, Kokkos::LayoutRight>;

template <typename T>
using matrix_t = Kokkos::View<T**, Kokkos::LayoutRight>;

template <typename T>
using cube_t = Kokkos::View<T***, Kokkos::LayoutRight>;

template <typename T>
KOKKOS_INLINE_FUNCTION auto
dot(const member_type& team, vector_t<T> m, vector_t<T> x, int N)
{
    T result{};
    Kokkos::parallel_reduce(Kokkos::ThreadVectorRange(team, N),
                            [&](const int k, T& update) { update += m(k) * cos(x(k)); },
                            result);
    return result;
}

template <typename T>
KOKKOS_INLINE_FUNCTION void
matvec(const member_type& team, matrix_t<T> m, vector_t<T> x, vector_t<T> y, int N)
{
    Kokkos::parallel_for(Kokkos::TeamThreadRange(team, 0, N), KOKKOS_LAMBDA(const int j) {
        y(j) += dot<T>(team, Kokkos::subview(m, j, Kokkos::ALL), x, N);
    });
}

template <typename T>
void
batched_matrix_vector(cube_t<T> m, matrix_t<T> x, matrix_t<T> y, int N,
                      int vector_size = 32)
{
    team_policy policy(m.extent(0), Kokkos::AUTO, vector_size);

    Kokkos::parallel_for(__FUNCTION__, policy, KOKKOS_LAMBDA(const member_type& team) {
        int i = team.league_rank();
        matvec<T>(team, Kokkos::subview(m, i, Kokkos::ALL, Kokkos::ALL),
                  Kokkos::subview(x, i, Kokkos::ALL), Kokkos::subview(y, i, Kokkos::ALL),
                  N);
    });
}

using data_type = double;

void
driver(int N, int repeat)
{
    tools::perf_data pd{ "initialize time" };
    pd.start().store(tools::normalize_tag{}, N * N * N);

    matrix_t<data_type> y("array-y", N, N);
    matrix_t<data_type> x("array-x", N, N);
    cube_t<data_type>   m("matrix-m", N, N, N);

    auto h_m = create_mirror_view(Kokkos::HostSpace(), m);
    auto h_x = create_mirror_view(Kokkos::HostSpace(), x);
    auto h_y = create_mirror_view(Kokkos::HostSpace(), y);

    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            h_x(i, j) = common::generate_random_in_range<double>(M_PI);
            h_y(i, j) = 0.0;
            auto val  = common::generate_canonical();
            for(int k = 0; k < N; ++k)
            {
                h_m(i, j, k) = exp(val);
            }
        }
    }

    Kokkos::deep_copy(m, h_m);
    Kokkos::deep_copy(x, h_x);
    Kokkos::deep_copy(y, h_y);

    pd.stop().print().reset("compute time").start().store(tools::normalize_tag{}, repeat * N * N * N);

    for(int r = 0; r < repeat; ++r)
        batched_matrix_vector<data_type>(m, x, y, N);

    pd.stop().print().reset("output time").start().store(tools::normalize_tag{}, N * N);

    h_y = Kokkos::create_mirror_view_and_copy(Kokkos::HostSpace(), y);

    std::ofstream ofs{ "matvec-kokkos.dat" };
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
            ofs << "y(" << i << ", " << j << ") = " << std::setprecision(6) << std::fixed
                << h_y(i, j) << "\n";
    }

    pd.stop().print();
}

int
main(int argc, char** argv)
{
    std::cout << "Running the kokkos version." << std::endl;
    tools::initialize(argc, argv);
    Kokkos::initialize(argc, argv);

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
    Kokkos::finalize();

    return 0;
}
