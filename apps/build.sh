#!/bin/bash -e

run-verbose()
{
    echo -e "\n##### Executing \"$@\"... #####\n"
    eval $@
}

SCRIPT_DIR=$(dirname ${BASH_SOURCE[0]})

: ${SOURCE_DIR:=$(bash -c "cd ${SCRIPT_DIR} && pwd")}
: ${BINARY_DIR:=${SOURCE_DIR}/build.sh-apps}
: ${TARGET_DIR:=$(bash -c "cd ${SCRIPT_DIR}/.. && pwd")}

echo "Source directory: ${SOURCE_DIR}"
echo "Binary directory: ${BINARY_DIR}"
echo "Output directory: ${TARGET_DIR}"

if [ "${1}" = "clean" ]; then
    run-verbose rm -rf ${BINARY_DIR}
    exit 0
fi

# create build directory
run-verbose mkdir -p ${BINARY_DIR}
run-verbose cd ${BINARY_DIR}

# remove any previous installs
# rm -rf $(find ../../bin | grep -v gitignore)
# rm -rf $(find ../../lib | grep -v gitignore)

# install to root folder
run-verbose cmake -DCMAKE_INSTALL_PREFIX=${TARGET_DIR} $@ ${SOURCE_DIR}
run-verbose cmake --build ${BINARY_DIR} --target all -- -j8
run-verbose cmake --build ${BINARY_DIR} --target install -- -j8
