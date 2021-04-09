#!/bin/bash 

FILES=$(find . -type f | egrep -v '\.sh$|README.md')
FOLDERS=$(find . -type d | egrep '^(./timemory)')

echo -e "Remove these files/folders?\n\n${FILES}\n${FOLDERS}\n"
echo "[y/n]?"
read answer

if [ "${answer}" == "y" ]; then
    rm -rf ${FILES}
    rm -rf ${FOLDERS}
else
    echo "Cleanup aborted";
fi;

