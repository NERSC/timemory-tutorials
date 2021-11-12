#!/bin/bash -e
#
# NOTE: timem only supports non MPI applications
#

# path to installed executables
BIN_DIR=${PWD}/../../bin

# copy over executable(s) to run
cp ${BIN_DIR}/basic.trace ./02_timem

# launch the app
timem ./02_timem
