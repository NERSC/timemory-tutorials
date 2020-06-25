#!/bin/bash

timemory-run --load libcustom_dynamic_instr -o ./lulesh.inst -- ../../apps/build/lulesh/lulesh
mpirun -n 1 ./lulesh.inst