#!/bin/bash -e
#
# NOTE: remove '--mpi --mpip' after timemory-run if application built without MPI
#

# path to installed executables
BIN_DIR=${PWD}/../../bin

# copy over executable(s) to modify
cp ${BIN_DIR}/basic.trace ./10_mpi

# add mpi and mpip instrumentation
timemory-run --mpi --mpip -o ./10_mpi.inst -- ./10_mpi

# run the application
mpirun -n 8 ./10_mpi.inst
