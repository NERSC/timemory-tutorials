# Visualizing and Analyzing Results

## About Hatchet

In this part of the tutorial we'll be using the
[Hatchet](https://github.com/hatchet/hatchet) library to load and visualize our
data in Jupyter. 

Hatchet is a library that provides a pandas-like interface for filtering,
scaling, and plotting your profiling/tracing data. In addition to timemory it
supports many other formats like HPCToolkit and Caliper. In this short tutorial
we can't cover all the capabilities of Hatchet but you can find more
information in their
[docs](https://hatchet.readthedocs.io/en/latest/index.html).

## Using Hatchet at NERSC

These directions are meant for NERSC users/systems. You may need to adapt for
your own system.

Note that we build Hatchet from source rather than pip-installing since the
timemory capabilities we will use are not yet in their most current release.

First let's build our custom Hatchet environment. We'll use this environment as
a Jupyter kernel.

```
module load python
conda create -n hatchet python=3.8 -y
source activate hatchet
cd $HOME
git clone https://github.com/hatchet/hatchet.git
cd hatchet
conda install --file requirements.txt -c conda-forge
conda install ipykernel
python -m ipykernel install --user --name hatchet --display-name hatchet
python setup.py build_ext --inplace
python setup.py install
```

This will build and configure your hatchet environment and jupyter kernel. It
should now be ready to use.

## Visualizing timemory data

We'll use the notebook in this directory called [timemory-hatchet.ipynb](timemory-hatchet.ipynb).

We'll visualize profiling and tracing data from our `example.py` in the previous tutorial section. These
files are in the [example-data](example-data) folder.

We'll also visualize another example which is currently availble from
[Hatchet](https://github.com/hatchet/hatchet/blob/49c58526478f3e0ddfd9366345c70f2eedb701e4/hatchet/tests/data/timemory/wall.tree.json).
This file is also in the [example-data](example-data) folder.

Note that at the moment, Hatchet interactive visualization is only supported in
classic/legacy Jupyter notebooks, not in Jupyter-lab. Hatchet developers are aware
of this and working on a [fix](https://github.com/hatchet/hatchet/issues/361).

## Running this example yourself

1. Build your custom conda environment/hatchet kernel as we describe above.
1. Log on to `jupyter.nersc.gov`. Select a `Shared CPU` node.
1. Under the `Help` menu, select `Launch Classic Notebook`.
1. Navigate through the file browser to wherever you have cloned this repository.
1. Open the `timemory-hatchet.ipynb` notebook.
1. Make sure you have selected the `hatchet` kernel in the top right corner of your notebook.
1. You can advance manually through the cells by clicking the `Run` button or by hitting `Shift + Enter`.
1. To change the type of data you display, you can edit the path in Cell 2. Sample files are located in the [example-data](example-data) folder.

