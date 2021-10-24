#!/bin/bash -e
#
# NOTE: remove '--mpi' after timemory-run if applications built without MPI
#

# path to installed executables
BIN_DIR=${PWD}/../../bin

# path to installed custom library
LIB_DIR=${PWD}/../../lib

# add path to custom lib in LD_LIBRARY_PATH
export LD_LIBRARY_PATH=${LIB_DIR}:${LD_LIBRARY_PATH}

# copy over executable(s) to modify
cp ${BIN_DIR}/basic.trace ./12_custom

# instrument using custom dynamic instrumentation library
timemory-run --load libcustom_dynamic_instr --mpi -o ./12_custom.inst -- ./12_custom

# run the instrumented binary
mpirun -n 8 ./12_custom.inst
