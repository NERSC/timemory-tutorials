#!/bin/bash -e
#
# NOTE: Applications built with MPI may abort when launched via timemory-run
#

# path to installed executables
BIN_DIR=${PWD}/../../bin

# copy over executable(s) to modify
cp ${BIN_DIR}/basic.trace ./06_launch

# launch
timemory-run -- ./06_launch
