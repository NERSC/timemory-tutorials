#!/bin/bash -e
#
# NOTE: add '--mpi' after timemory-run to applications built with MPI
#

# path to installed executables
BIN_DIR=${PWD}/../../bin

# copy over executable(s) to modify
cp ${BIN_DIR}/basic.trace ./10_stubs

# instrumentation using --stubs mode.
timemory-run --stubs -o ./10_stubs.inst -- ./10_stubs
# run. no instrumentation data since only stubs
./10_stubs.inst

# preload libtimemory and then run again for instrumentation data
LD_PRELOAD=libtimemory.so ./10_stubs.inst