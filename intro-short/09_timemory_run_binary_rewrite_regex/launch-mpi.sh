#!/bin/bash -e
#
# NOTE: add '--mpi' after timemory-run to applications built with MPI
#

# path to installed executables
BIN_DIR=${PWD}/../../bin

# copy over executable(s) to modify
cp ${BIN_DIR}/basic.trace ./09_no_impl
cp ${BIN_DIR}/basic.trace ./09_no_impl_or_allreduce

# exclude all functions matching pattern 'impl_*'
timemory-run --mpi --mpip \
    -E 'impl_*' \
    -o ./09_no_impl.inst \
    -- ./09_no_impl
# run instrumented exe
mpirun -np 4 ./09_no_impl.inst

# exclude all functions which start with 'impl' or 'allreduce'
# enable instrumenting C++ standard library functions
timemory-run --mpi --mpip \
    -E '^(impl|allreduce)' \
    --stdlib \
    -o ./09_no_impl_or_allreduce.inst \
    -- ./09_no_impl_or_allreduce
# run instrumented exe
mpirun -np 4 ./09_no_impl_or_allreduce.inst
