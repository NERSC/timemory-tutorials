#!/bin/bash -e
#
# NOTE: add '--mpi' after timemory-run to applications built with MPI
#

# path to installed executables
BIN_DIR=${PWD}/../../bin

# copy over executable(s) to modify
cp ${BIN_DIR}/basic.trace ./08_wc
cp ${BIN_DIR}/basic.trace ./08_wc_cpu_peak

# simple binary re-write. only uses wall_clock for instrumentation
timemory-run -o ./08_wc.inst -MI 'basic*' -- ./08_wc
# run
./08_wc.inst

# binary rewrite but uses wall_clock cpu_util and peak rss for instrumentation
timemory-run -d wall_clock cpu_util peak_rss -MI 'basic*' -o ./08_wc_cpu_peak.inst -- ./08_wc_cpu_peak
# run
./08_wc_cpu_peak.inst