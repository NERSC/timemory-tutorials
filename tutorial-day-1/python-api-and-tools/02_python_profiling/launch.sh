#!/bin/bash -e

# default component
: ${COMPONENT:=cpu_clock}

if [ -n "${1}" ]; then
    COMPONENT=$1
    shift
fi

python -m timemory.trace    -c ${COMPONENT} -d timemory-trace-output   -- ./solution/unmodified.py
python -m timemory.profiler -c ${COMPONENT} -d timemory-profile-output -- ./solution/unmodified.py

# Require @profile
python -m timemory.trace         -c ${COMPONENT} -b -d timemory-builtin-trace-output   -- ./solution/with-builtins.py
python -m timemory.profiler      -c ${COMPONENT} -b -d timemory-builtin-profile-output -- ./solution/with-builtins.py
python -m timemory.line_profiler -c ${COMPONENT} -v                                    -- ./solution/with-builtins.py
