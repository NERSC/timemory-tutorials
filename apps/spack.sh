#!/bin/bash -e

OPTS="+openmp +profiling +serial +shared"

# Uncomment if desired
# OPTS="${OPTS} +hwloc +memkind +numactl"

# Comment out if desired
OPTS="${OPTS} +cuda +cuda_uvm +cuda_lambda +wrapper cuda_arch=72"

spack spec kokkos ${OPTS} $@
spack install -v -j8 kokkos ${OPTS} $@
