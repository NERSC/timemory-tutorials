#!/bin/bash -e

# default component
: ${COMPONENT:=cpu_clock}

if [ -n "${1}" ]; then
    COMPONENT=$1
    shift
fi

python -m timemory.trace    -c ${COMPONENT} -d timemory-trace-output   -- ./unmodified.py
python -m timemory.profiler -c ${COMPONENT} -d timemory-profile-output -- ./unmodified.py

# Require @profile
python -m timemory.trace         -c ${COMPONENT} -b -d timemory-builtin-trace-output   -- ./with-builtins.py
python -m timemory.profiler      -c ${COMPONENT} -b -d timemory-builtin-profile-output -- ./with-builtins.py
python -m timemory.line_profiler -c ${COMPONENT} -v                                    -- ./with-builtins.py
