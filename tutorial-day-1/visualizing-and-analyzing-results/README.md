# Visualizing and Analyzing Results

To use the interactive visualization capabilities in Hatchet, you'll need a
conda environment. You'll use this conda environment as Jupyter kernel. Note
that at the moment, Hatchet interactive visualization is only supported in
classic/legacy Jupyter notebooks, not in Jupyter-lab.

## Building your Hatchet conda environment and kernel

These directions are meant for NERSC users/systems. You may need to adapt for
your own system.

Note that we build Hatchet from source rather than pip-installing since the
timemory capabilities we will use are not yet in their most current release.

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

We'll use example data in `wall.tree.json` in this directory. Of course you can
use your own timemory output data, too.
