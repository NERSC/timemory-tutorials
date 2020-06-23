#!/bin/bash

mkdir -p tools
cd tools

module load git

# checkout required repos
git clone https://github.com/spack/spack.git

export PATH=$PWD/tools/spack/bin:$PATH

echo "Installing timemory and its dependencies..."

# install timemory and its dependencies
spack install timemory@develop +cupti +cuda ~caliper +mpi ~ompt +papi ~examples ~static +shared ~tau ~vtune +kokkos_build_config +dyninst 

# setup spack env
. ./tools/spack/share/spack/setup-env.sh

# load timemory module
spack load timemory

echo "All done !"