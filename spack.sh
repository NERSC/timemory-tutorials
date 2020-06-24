#!/bin/bash -e

OPTS="+shared +python +require_packages"
OPTS="${OPTS} +mpi"
OPTS="${OPTS} +mpip_library +gotcha"
OPTS="${OPTS} +dyninst"
OPTS="${OPTS} +papi"
# OPTS="${OPTS} +caliper"
# OPTS="${OPTS} +tau"
# OPTS="${OPTS} +vtune"
# OPTS="${OPTS} +kokkos_build_config"
# OPTS="${OPTS} +ompt +ompt_library +ompt_standalone"
# OPTS="${OPTS} +examples"
OPTS="${OPTS} ~ompt ~ompt_library ~ompt_standalone"

if [ -n "$(which nvcc)" ]; then
    OPTS="${OPTS} +cuda +cupti"
fi

# install timemory and its dependencies
spack spec timemory@develop ${OPTS} $@
spack install -v -j8 timemory@develop ${OPTS} $@
