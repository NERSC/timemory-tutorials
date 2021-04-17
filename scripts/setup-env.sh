#!/bin/bash -e

# repository root folder
REPO_DIR=$(dirname $(bash -c "cd $(dirname ${BASH_SOURCE[0]}) && pwd"))

# add apps bin install to path
export PATH=${REPO_DIR}/bin:${PATH}

# add apps lib install to library path
if [ "$(uname)" = "Darwin" ]; then
    export DYLD_LIBRARY_PATH=${REPO_DIR}/lib:${DYLD_LIBRARY_PATH}
else
    export LD_LIBRARY_PATH=${REPO_DIR}/lib:${LD_LIBRARY_PATH}
fi
