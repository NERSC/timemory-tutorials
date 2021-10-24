#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""TomoPy script to reconstruct a built-in phantom."""

import os
import sys
import argparse
import traceback
import tomopy
import numpy as np

# miscellaneous output functions
import misc
import timemory
from timemory.bundle import marker
from timemory.profiler import profile
from timemory.trace import trace
from timemory.profiler import config as profile_config
from timemory.trace import config as trace_config

components = ["wall_clock"]
profile_config.only_filenames += ["(site|dist)-packages[/\\]+tomopy"]
trace_config.only_filenames += ["(site|dist)-packages[/\\]+tomopy"]
# trace_config.skip_filenames += ["[/\\]numpy", "(dtype|copy|os|_collections_abc)\\.py$"]
profile_config.skip_filenames += ["dtype.py$"]
trace_config.skip_filenames += ["dtype.py$"]

from timemory.component import WallClock

wc = WallClock("<optional label>")
wc.start()
# ...
wc.stop()
print(f"{wc}")

def get_components_env():
    return os.environ.get("COMPONENTS", "").split()


def get_components():
    return components + ["user_global_bundle"] + [get_components_env]

from timemory.bundle import marker

@marker(["wall_clock", "caliper_marker"])
def foo():
    # ....
    pass

@marker(get_components)
def generate(phantom, args):
    """Return the simulated data for the given phantom."""
    #with trace(get_components):
    obj = getattr(tomopy.misc.phantom, phantom)(size=args.size)
    obj = tomopy.misc.morph.pad(obj, axis=1, mode="constant")
    obj = tomopy.misc.morph.pad(obj, axis=2, mode="constant")

    if args.partial:
        data_size = obj.shape[0]
        subset = list(args.subset)
        subset.sort()
        nbeg, nend = subset[0], subset[1]
        if nbeg == nend:
            nend += 1
        if not args.no_center:
            ndiv = (nend - nbeg) // 2
            offset = data_size // 2
            nbeg = offset - ndiv
            nend = offset + ndiv
        print(
            "[partial]> slices = {} ({}, {}) of {}".format(
                nend - nbeg, nbeg, nend, data_size
            )
        )
        obj = obj[nbeg:nend, :, :]

    #with profile(get_components):
    ang = tomopy.angles(args.angles)
    prj = tomopy.project(obj, ang)

    print(
        "[dims]> projection = {}, angles = {}, object = {}".format(
            prj.shape, ang.shape, obj.shape
        )
    )
    return [prj, ang, obj]


@marker(get_components)
def run(phantom, algorithm, args):
    """Run reconstruction benchmarks for phantoms.

    Parameters
    ----------
    phantom : string
        The name of the phantom to use.
    algorithm : string
        The name of the algorithm to test.
    args : argparser args

    Returns
    -------
    Either rec or imgs
    rec : np.ndarray
        The reconstructed image.
    """

    prj, ang, obj = generate(phantom, args)
    proj = np.zeros(shape=[prj.shape[1], prj.shape[0], prj.shape[2]], dtype=np.float32)
    for i in range(0, prj.shape[1]):
        proj[i, :, :] = prj[:, i, :]

    # always add algorithm
    _kwargs = {"algorithm": algorithm}

    # assign number of cores
    _kwargs["ncore"] = args.ncores

    # don't assign "num_iter" if gridrec or fbp
    if algorithm not in ["fbp", "gridrec"]:
        _kwargs["num_iter"] = args.num_iter

    # use the accelerated version
    if algorithm in ["mlem", "sirt"]:
        _kwargs["accelerated"] = True

    print("kwargs: {}".format(_kwargs))
    with marker(get_components, key="recon"):
        # with trace(get_components):
        rec = tomopy.recon(prj, ang, **_kwargs)
    print("completed reconstruction...")

    rec = misc.normalize(rec)
    rec = misc.trim_border(
        rec,
        rec.shape[0],
        rec[0].shape[0] - obj[0].shape[0],
        rec[0].shape[1] - obj[0].shape[1],
    )

    misc.output_images(rec, algorithm, args.format)


if __name__ == "__main__":

    parser = argparse.ArgumentParser()

    # phantom choices
    phantom_choices = [
        "baboon",
        "cameraman",
        "barbara",
        "checkerboard",
        "lena",
        "peppers",
        "shepp2d",
        "shepp3d",
    ]
    algorithms = [
        "gridrec",
        "art",
        "fbp",
        "bart",
        "mlem",
        "osem",
        "sirt",
        "ospml_hybrid",
        "ospml_quad",
        "pml_hybrid",
        "pml_quad",
        "tv",
        "grad",
        "tikh",
    ]

    import multiprocessing as mp

    ncores = mp.cpu_count()

    parser.add_argument(
        "-p",
        "--phantom",
        help="Phantom to use",
        default="shepp2d",
        choices=phantom_choices,
        type=str,
    )
    parser.add_argument(
        "-a",
        "--algorithm",
        help="Select the algorithm",
        default="sirt",
        choices=algorithms,
        type=str,
    )
    parser.add_argument(
        "-A", "--angles", help="number of angles", default=1501, type=int
    )
    parser.add_argument("-s", "--size", help="size of image", default=512, type=int)
    parser.add_argument(
        "-n", "--ncores", help="number of cores", default=ncores, type=int
    )
    parser.add_argument(
        "-f", "--format", help="output image format", default="png", type=str
    )
    parser.add_argument(
        "-i", "--num-iter", help="Number of iterations", default=50, type=int
    )
    parser.add_argument(
        "--partial",
        help="Enable partial reconstruction of 3D data",
        action="store_true",
    )
    parser.add_argument(
        "-r",
        "--subset",
        help="Select subset (range) of slices (center enabled by default)",
        default=(0, 8),
        type=int,
        nargs=2,
    )
    parser.add_argument(
        "--no-center",
        help="When used with '--subset', do no center subset",
        action="store_true",
    )
    parser.add_argument(
        "-c",
        "--components",
        help="timemory component types",
        default=components,
        choices=timemory.component.get_available_types(),
        nargs="*",
        type=str,
    )

    args = parser.parse_args()

    components = args.components

    try:
        timemory.settings.cout_output = False
        timemory.settings.flamegraph_output = False
        timemory.init([__file__])
        run(args.phantom, args.algorithm, args)
        timemory.finalize()
    except Exception as e:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        traceback.print_exception(exc_type, exc_value, exc_traceback, limit=5)
        print("Exception - {}".format(e))
        sys.exit(1)
