#!/bin/bash

timemory-run --mpi --mpip -o ./lulesh.inst -- ../../apps/build/lulesh/lulesh
mpirun -n 8 ./lulesh.inst