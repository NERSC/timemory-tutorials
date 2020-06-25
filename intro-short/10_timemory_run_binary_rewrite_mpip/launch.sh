#!/bin/bash

cd ../../apps/build/lulesh
timemory-run --mpi --mpip -o ./lulesh.inst -- ./lulesh
mpirun -n 8 ./lulesh.inst