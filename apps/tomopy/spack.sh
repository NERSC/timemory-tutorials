#!/bin/bash -e

PACKAGES="python@3.7.7 py-dxchange py-h5py py-numexpr py-numpy py-pywavelets py-scikit-build py-scikit-image py-scipy py-pip"

# pass in the compiler spec if necessary
INSTALL=""
for i in ${PACKAGES}
do
    PACKAGE="${i} ${@}"
    spack spec ${PACKAGE}
    INSTALL="${INSTALL} ${PACKAGE}"
done

# spack install -v -j8 opencv
# spack install -v -j8 intel-mkl $@ +ilp64 threads=tbb
spack install -v -j8 ${INSTALL}

echo -e "Spack load command:\n\nspack load -r ${INSTALL}\n"

spack load -r ${INSTALL}

pip install mkl-devel
