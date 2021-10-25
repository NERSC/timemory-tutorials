#!/usr/bin/env python

import sys
import time
import argparse
import numpy as np
import timemory
from timemory.tools import function_wrappers

import mpi4py
from mpi4py import MPI
from mpi4py.MPI import Exception as MPIException

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()


def run(nitr, nsize):
    msgs = set()
    for i in range(nitr):
        # passing MPI datatypes explicitly
        try:
            if rank == 0:
                data = np.arange(nsize, dtype="i")
                comm.Send([data, MPI.INT], dest=1, tag=77)
            elif rank == 1:
                data = np.empty(nsize, dtype="i")
                comm.Recv([data, MPI.INT], source=0, tag=77)
        except MPIException as e:
            msgs.add(f"{e}")

        # automatic MPI datatype discovery
        try:
            if rank == 0 and size == 2:
                data = np.empty(nsize, dtype=np.float64)
                comm.Recv(data, source=1, tag=13)
            elif rank == 1:
                data = np.arange(nsize, dtype=np.float64)
                comm.Send(data, dest=0, tag=13)
        except MPIException as e:
            msgs.add(f"{e}")

    for i, itr in enumerate(msgs):
        sys.stderr.write("{}: {}\n".format(i, itr))


if __name__ == "__main__":
    timemory.init([__file__])
    timemory.enable_signal_detection()

    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-i",
        "--iterations",
        default=100,
        type=int,
        help="Iterations",
    )
    parser.add_argument(
        "-n",
        "--size",
        default=1000000,
        type=int,
        help="Array size",
    )
    parser.add_argument(
        "-w",
        "--wrappers",
        default=["mpi", "malloc"],
        choices=("mpi", "openmp", "malloc", "nccl"),
        type=str,
        help="Profiling library wrappers to activate",
        nargs="*",
    )

    args = parser.parse_args()

    ts = time.perf_counter()
    with function_wrappers(*args.wrappers):
        if size == 2:
            run(args.iterations, args.size)
    ts = time.perf_counter() - ts

    print("")
    print("Elapsed time (sec) :  {:12.6f}".format(ts))
    print("")
    sys.stdout.flush()

    timemory.finalize()
