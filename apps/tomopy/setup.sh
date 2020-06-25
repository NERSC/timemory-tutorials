#!/bin/bash

if [ ! -f tomopy-source/CMakeLists.txt ];
then
    git submodule update --recursive --init tomopy-source
fi

cd tomopy-source

python -m pip install .
