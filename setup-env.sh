#!/bin/bash -e

# directory containing this file
THIS_DIR=$(dirname ${BASH_SOURCE[0]})

# add it to modulepath
export MODULEPATH=${MODULEPATH}:${THIS_DIR}/modulefiles

# add apps bin install to path
export PATH=${THIS_DIR}/bin:${PATH}

# add apps lib install to library path
if [ "$(uname)" = "Darwin" ]; then
    export DYLD_LIBRARY_PATH=${THIS_DIR}/lib:${DYLD_LIBRARY_PATH}
else
    export LD_LIBRARY_PATH=${THIS_DIR}/lib:${LD_LIBRARY_PATH}
fi
