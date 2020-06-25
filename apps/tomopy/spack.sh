#!/bin/bash -e

PACKAGES="python@3.7.7 py-dxchange py-h5py intel-mkl opencv py-numexpr py-numpy py-pywavelets py-scikit-build py-scikit-image py-scipy"

# pass in the compiler spec if necessary
INSTALL=""
for i in ${PACKAGES}
do
    INSTALL="${INSTALL} ${i}${@}"
done

spack install -v -j8 ${INSTALL}
