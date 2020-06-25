#!/bin/bash -e

# create build directory
mkdir -p build-apps
cd build-apps

# remove any previous installs
# rm -rf $(find ../../bin | grep -v gitignore)
# rm -rf $(find ../../lib | grep -v gitignore)

# install to root folder
cmake -DCMAKE_INSTALL_PREFIX=../.. ..

make -j8
make -j8 install
