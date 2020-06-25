#!/bin/bash

mkdir -p tools
cd tools

# checkout required repos
git clone https://github.com/spack/spack.git

export PATH=$PWD/tools/spack/bin:$PATH

echo "Installing timemory and its dependencies..."

# install timemory. Change any '~' to '+' or
# use 'spack info timemory'
spack install -v -j8 timemory@develop \
      build_type=RelWithDebInfo \
      cpu_target=skylake-avx512 \
      cuda_arch=volta \
      +shared \
      +python \
      +papi \
      +dyninst \
      +mpi +mpip_library +gotcha \
      +cuda +cupti \
      +kokkos_tools +kokkos_build_config \
      ~caliper \
      ~allinea_map \
      ~likwid 

# setup spack env
. ./tools/spack/share/spack/setup-env.sh

# load timemory module
spack load timemory

echo "All done !"
