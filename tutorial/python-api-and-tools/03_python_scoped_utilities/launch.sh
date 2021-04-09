#!/bin/bash -e

# path to installed executables
BIN_DIR=${PWD}/../../bin

# launch the basic app
python ${BIN_DIR}/basic.py

# launch the intermediate app
python ${BIN_DIR}/intermediate.py

