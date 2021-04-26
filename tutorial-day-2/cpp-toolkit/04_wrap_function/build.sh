#!/bin/bash -e

run-verbose()
{
    echo -e "\n##### Executing \"$@\"... #####\n"
    eval $@
}

SCRIPT_DIR=$(dirname ${BASH_SOURCE[0]})

: ${SOURCE_DIR:=$(bash -c "cd ${SCRIPT_DIR} && pwd")}
: ${BINARY_DIR:=${SOURCE_DIR}/build.sh-$(basename ${SOURCE_DIR})}
: ${TARGET_DIR:=$(bash -c "cd ${SCRIPT_DIR}/../../.. && pwd")}

echo "Source directory: ${SOURCE_DIR}"
echo "Binary directory: ${BINARY_DIR}"
echo "Output directory: ${TARGET_DIR}"

if [ "${1}" = "clean" ]; then
    run-verbose rm -rf ${BINARY_DIR}
    exit 0
fi

run-verbose cmake -B ${BINARY_DIR} -DCMAKE_INSTALL_PREFIX=${TARGET_DIR} $@ ${SOURCE_DIR}
run-verbose cmake --build ${BINARY_DIR} --target all --parallel 4
run-verbose cmake --build ${BINARY_DIR} --target install --parallel 4
