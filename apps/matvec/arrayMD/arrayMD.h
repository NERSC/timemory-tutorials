/*
 * ArrayMD class for multidimensional arrays
 *
 * Rahulkumar Gayatri
 * */

#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#if __NVCC__
#    include <cuda.h>
#    include <cuda_runtime.h>
#    include <helper_cuda.h>
#    include <helper_functions.h>
#    define CUDA_HOSTDEV __host__ __device__
#else
#    define CUDA_HOSTDEV
#endif

template <bool B, typename T>
using enable_if_t = typename std::enable_if<B, T>::type;

enum Device
{
    cpu = 1,
    gpu = 2
};

template <size_t dim, typename T>
inline void
compute_offsets(std::array<size_t, dim>& offsets, size_t& size, T arg)
{
    offsets[dim - 1] = arg;
    size *= arg;
}

template <size_t dim, typename T, typename... Idx>
inline void
compute_offsets(std::array<size_t, dim>& offsets, size_t& size, T arg, Idx... args)
{
    offsets[dim - (sizeof...(args)) - 1] = arg;
    size *= arg;
    compute_offsets(offsets, size, args...);
}

template <size_t dim, typename T>
inline void
compute_args(std::array<size_t, dim>& offsets, size_t& index, T arg)
{
    index += arg;
}

template <size_t dim, typename T, typename... Idx>
inline void
compute_args(std::array<size_t, dim>& offsets, size_t& index, T arg, Idx... args)
{
    auto index_eval = arg;
    for(int i = dim - sizeof...(args); i < dim; ++i)
        index_eval *= offsets[i];
    index += index_eval;

    compute_args(offsets, index, args...);

    return;
}

template <size_t dim, typename T>
void
compute_subarray_offset(std::array<size_t, dim>& offsets, size_t& subarray_offset, T arg)
{
    subarray_offset += arg * offsets[dim - 1];
}

template <size_t dim, typename T, typename... Idx>
void
compute_subarray_offset(std::array<size_t, dim>& offsets, size_t& subarray_offset, T arg,
                        Idx... args)
{
    size_t tmp = 1;
    for(int i = 0; i < dim - 1; ++i)
        tmp *= offsets[dim - (sizeof...(args)) - i];
    subarray_offset += arg * tmp;

    compute_subarray_offset(offsets, subarray_offset, args...);
}

template <typename T>
CUDA_HOSTDEV void
compute_subarray_offset_gpu(size_t* offsets, size_t& subarray_offset, size_t dim, T arg)
{
    subarray_offset += arg * offsets[dim - 1];
}

template <typename T, typename... Idx>
void CUDA_HOSTDEV
     compute_subarray_offset_gpu(size_t* offsets, size_t& subarray_offset, size_t dim, T arg,
                                 Idx... args)
{
    size_t tmp = 1;
    for(int i = 0; i < dim - 1; ++i)
        tmp *= offsets[dim - (sizeof...(args)) - i];
    subarray_offset += arg * tmp;

    compute_subarray_offset_gpu(offsets, subarray_offset, dim, args...);
}

template <typename T>
CUDA_HOSTDEV inline void
compute_args_gpu(size_t* offsets, size_t& index, size_t dim, T arg)
{
    index += arg;
}

template <typename T, typename... Idx>
CUDA_HOSTDEV inline void
compute_args_gpu(size_t* offsets, size_t& index, size_t dim, T arg, Idx... args)
{
    auto index_eval = arg;

    for(int i = dim - sizeof...(args); i < dim; ++i)
    {
        index_eval *= offsets[i];
    }

    index += index_eval;

    compute_args_gpu(offsets, index, dim, args...);
}

template <typename T, size_t dim, size_t device>
struct ArrayMD
{
    std::array<size_t, dim> m_offsets;
    size_t*                 d_offsets;
    size_t                  size;
    size_t                  subarray_offset;
    T*                      dptr;

    ArrayMD() = default;

    //  template<typename... Idx>

    template <typename... Idx, size_t D = device,
              enable_if_t<(D == Device::cpu), int> = 0>
    ArrayMD(Idx... args)
    {
        const auto N = sizeof...(args);
        static_assert(N == dim, "Dimensionality passed does not match the argument list");
        size = 1;
        m_offsets.fill(0);

        compute_offsets(m_offsets, size, args...);

        // Allocate memory for size number of T-elements
        dptr = new T[size];
    }

    template <typename... Idx, size_t D = device,
              enable_if_t<(D == Device::gpu), int> = 0>
    ArrayMD(Idx... args)
    {
        const auto N = sizeof...(args);
        static_assert(N == dim, "Dimensionality passed does not match the argument list");
        size = 1;
        m_offsets.fill(0);

        compute_offsets(m_offsets, size, args...);

#if __NVCC__
        // Allocate memory for size number of T-elements on the GPU
        checkCudaErrors(cudaMalloc(&dptr, size * sizeof(T)));

        // Allocate and Copy the offsets
        checkCudaErrors(cudaMalloc(&d_offsets, N * sizeof(size_t)));
        checkCudaErrors(cudaMemcpy(d_offsets, m_offsets.data(), dim * sizeof(size_t),
                                   cudaMemcpyHostToDevice));
#endif
    }

    template <typename... Idx, size_t D = device,
              enable_if_t<(D == Device::cpu), int> = 0>
    inline T& operator()(Idx... args)
    {
        const auto N = sizeof...(args);
        static_assert(N == dim, "parameters passed exceed the dimensionality");
        size_t index = 0;

        compute_args(m_offsets, index, args...);

        return dptr[index];
    }

    template <typename... Idx, size_t D = device,
              enable_if_t<(D == Device::gpu), int> = 0>
    CUDA_HOSTDEV inline T& operator()(Idx... args)
    {
        const auto N = sizeof...(args);
        static_assert(N == dim, "parameters passed exceed the dimensionality");
        size_t index = 0;

        compute_args_gpu(d_offsets, index, dim, args...);

        return dptr[index];
    }

    // template <size_t D = device, enable_if_t<(D == Device::cpu), int> = 0>
    CUDA_HOSTDEV
    ArrayMD(const ArrayMD& p)
    {
        m_offsets = p.m_offsets;
        size      = 0;
        dptr      = p.dptr;

        if(device == Device::gpu)
        {
            d_offsets = p.d_offsets;
        }
    }

    ArrayMD& operator=(ArrayMD&&) = default;

    template <size_t D = device, enable_if_t<(D == Device::cpu), int> = 0>
    ArrayMD& operator=(const ArrayMD& p)
    {
        m_offsets = p.m_offsets;
        size      = 0;
        dptr      = p.dptr;

        return *this;
    }

    template <size_t D = device, enable_if_t<(D == Device::gpu), int> = 0>
    CUDA_HOSTDEV ArrayMD& operator=(const ArrayMD& p)
    {
        m_offsets = p.m_offsets;
        d_offsets = p.d_offsets;
        size      = 0;
        dptr      = p.dptr;

        return *this;
    }

    template <typename... Idx, size_t D = device,
              enable_if_t<(D == Device::cpu), int> = 0>
    T* subArray(Idx... args)
    {
        size_t     subarray_offset = 0;
        const auto N               = sizeof...(args);
        static_assert(N == dim - 1,
                      "parameters passed to subArray should be 1 less than actual "
                      "dimension of the arrayMD");

        compute_subarray_offset(m_offsets, subarray_offset, args...);

        return (dptr + subarray_offset);
    }

    template <typename... Idx, size_t D = device,
              enable_if_t<(D == Device::gpu), int> = 0>
    CUDA_HOSTDEV T* subArray(Idx... args)
    {
        size_t     subarray_offset = 0;
        const auto N               = sizeof...(args);
        static_assert(N == dim - 1,
                      "parameters passed to subArray should be 1 less than actual "
                      "dimension of the arrayMD");

        compute_subarray_offset_gpu(d_offsets, subarray_offset, dim, args...);

        return (dptr + subarray_offset);
    }

    template <typename... Idx, size_t D = device,
              enable_if_t<(D == Device::cpu), int> = 0>
    void resize(Idx... args)
    {
        const auto N = sizeof...(args);
        static_assert(N == dim, "Dimensionality passed does not match the argument list");
        size = 1;
        m_offsets.fill(0);

        compute_offsets(m_offsets, size, args...);

        // Allocate memory for size number of T-elements
        dptr = new T[size];
    }

    template <size_t D = device, enable_if_t<(D == Device::cpu), int> = 0>
    void destroy_data()
    {
        if(size && dptr && device == Device::cpu)
            delete[] dptr;
    }

    template <size_t D = device, enable_if_t<(D == Device::gpu), int> = 0>
    void destroy_data()
    {
#if __NVCC__
        if(size && dptr && device == Device::gpu)
            checkCudaErrors(cudaFree(dptr));
#endif
    }

    ~ArrayMD() { destroy_data(); }
};
