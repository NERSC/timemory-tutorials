#!/bin/bash -e
#
# NOTE: remove '--mpi' after timemory-run if applications built without MPI
#

# path to installed executables
BIN_DIR=${PWD}/../../bin

# copy over executable(s) to modify
cp ${BIN_DIR}/basic.region ./13_region
cp ${BIN_DIR}/basic.region ./13_trace

# add dynamic instrumentation in trace mode. no synchronization
timemory-run --mode=trace --mpi -MI 'basic*' -o ./13_trace.inst -- ./13_trace
# run trace mode instrumented binary
./13_trace.inst

# add dynamic instrumentation in region mode. synchronization applied
timemory-run --mode=region --mpi -MI 'basic*' -o ./13_region.inst -- ./13_region
# run region mode instrumented binary
./13_region.inst
