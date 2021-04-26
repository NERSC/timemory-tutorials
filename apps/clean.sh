#!/bin/bash -e

# directory containing this file
REPO_DIR=$(dirname $(bash -c "cd $(dirname ${BASH_SOURCE[0]}) && pwd"))

for i in bin lib include
do
    if [ -d ${REPO_DIR}/${i} ]; then
        FILES="$(find ${REPO_DIR}/${i} -type f | grep -v gitignore)"
        if [ -n "${FILES}" ]; then rm ${FILES}; fi
    fi
done

if [ -d $(dirname ${BASH_SOURCE[0]})/build.sh-apps ]; then
    rm -r $(dirname ${BASH_SOURCE[0]})/build.sh-apps
fi
