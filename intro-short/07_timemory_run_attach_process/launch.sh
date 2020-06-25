#!/bin/bash -e
#
# NOTE: The attach mode may not work if system permissions are disabled.
#

# path to installed executables
BIN_DIR=${PWD}/../../bin

# copy over executable(s) to modify
cp ${BIN_DIR}/basic.trace ./07_attach

# launch and grab PID
./07_attach & PID=$!

# attach via timemory-run
timemory-run --pid ${PID} -- ./07_attach
