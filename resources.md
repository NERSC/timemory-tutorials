# Tutorial Resources

If you already an active NERSC account, add your username in the `#m3502` channel in the
[timemory slack](https://timemory.slack.com/)
(if you haven't joined, [signup here](https://join.slack.com/t/timemory/signup)).
If you do not want to join the slack channel,
[create an issue in the timemory-tutorials github](https://github.com/NERSC/timemory-tutorials/issues/new).

## NERSC Training Account

If you do not have a NERSC account,
you can register for a NERSC training account at [iris.nersc.gov/train](https://iris.nersc.gov/train).
The training code should have been provided to you in an email. Once you have registered, signed the
User Agreement Form, you will be given a training account of the form `trainxxx` and the account
will be activated immediately. You can login into the compute systems with the provided password without MFA.

The timemory ECP tutorial has reserved 4 Haswell nodes and 8 GPU nodes for the tutorial. The reservations are:

| Day | System  | Reservation Name      |
| --- | ------- | --------------------- |
| 1   | Haswell | timemory-ecp2021_hsw1 |
|     | GPU     | timemory-ecp2021_gpu1 |
| 2   | Haswell | timemory-ecp2021_hsw2 |
|     | GPU     | timemory-ecp2021_gpu2 |

These reservations will be active 30 minutes before the start of the tutorial (8:30 AM PDT) until 30 minutes after (12:30 PM PDT).

The nodes are shared resources so please be sure to use the exact commands provided below:

### Cori Haswell

Two hours of 4 physical CPU cores (8 hyperthreads).

```console
git clone -b ecp-2021 https://github.com/NERSC/timemory-tutorials.git
cd timemory-tutorials
module swap PrgEnv-intel PrgEnv-gnu
module load timemory
salloc --reservation=<name> -A ntrain -q shared -N 1 -n 1 -c 8 -t 02:00:00
```

Proceed to [Testing Installation](testing.md).

### Cori GPU

```console
git clone -b ecp-2021 https://github.com/NERSC/timemory-tutorials.git
cd timemory-tutorials
module purge
module load cgpu gcc
module load timemory
salloc --reservation=<name> -A ntrain -N 1 -C gpu -G 1 -n 1 -c 20 -t 02:00:00
```

Proceed to [Testing Installation](testing.md).

## Docker Container

The docker containers are Ubuntu 18.04 images with the GCC 7.5 compiler.
Both containers have timemory installed with all the necessary dependencies for the exercises.
Some exercises may try to collect hardware counters via PAPI. If you are running docker
on Linux, the file `/proc/sys/kernel/perf_event_paranoid` must contain a value of 1 or less
in order for HW counter collection to work. Changing this value requires sudo access.
If you do not have sudo access or are running the docker container on macOS or Windows,
you will not be able to collect hardware counters.

### CPU only Image

```console
git clone -b ecp-2021 https://github.com/NERSC/timemory-tutorials.git
docker pull nersc/timemory:ecp-2021-cpu
docker run -it --rm -v $PWD:/home/timemory-tutorials nersc/timemory:ecp-2021-cpu
```

### GPU Image

> Requires `nvidia-docker`

```console
git clone -b ecp-2021 https://github.com/NERSC/timemory-tutorials.git
docker pull nersc/timemory:ecp-2021-gpu
nvidia-docker run -it --rm -v $PWD:/home/timemory-tutorials nersc/timemory:ecp-2021-gpu
```


### Local Installation Instructions

If your native system is Linux, consider using spack to install dyninst and/or PAPI:

```console
mkdir scripts/tools
cd scripts/tools
git clone https://github.com/spack/spack.git
source ./spack/share/spack/setup-env.sh
spack compiler find
spack external find
spack install boost~shared+pic dyninst@10.2.1%gcc
spack load boost dyninst
```

#### Linux

```console
PYTHON_VERSION=3.8
git clone https://github.com/NERSC/timemory.git timemory-source
python${PYTHON_VERSION} -m pip install --user -r timemory-source/.requirements/runtime.txt
python${PYTHON_VERSION} -m pip install --user -r timemory-source/.requirements/mpi_runtime.txt
cmake -B timemory-build \
  -D CMAKE_INSTALL_PREFIX=timemory-install \
  -D CMAKE_BUILD_TYPE=RelWithDebInfo \
  -D TIMEMORY_USE_DYNINST=ON \
  -D TIMEMORY_USE_GOTCHA=ON \
  -D TIMEMORY_USE_MPI=ON \
  -D TIMEMORY_USE_PYTHON=ON \
  -D TIMEMORY_BUILD_TOOLS=ON \
  -D PYTHON_EXECUTABLE=$(which python${PYTHON_VERSION})
cmake --build timemory-build --target all --parallel 8
cmake --build timemory-build --target install --parallel 8
export CMAKE_PREFIX_PATH=$PWD/timemory-install:${CMAKE_PREFIX_PATH}
export PATH=$PWD/timemory-install/bin:${PATH}
export LD_LIBRARY_PATH=$PWD/timemory-install/lib64:$PWD/timemory-install/lib:${LD_LIBRARY_PATH}
export PYTHONPATH=$PWD/timemory-install/lib/python${PYTHON_VERSION}/site-packages:${PYTHONPATH}
```

#### macOS

In general, the only timemory feature from the tutorial that is not available on macOS is the dynamic instrumentation (`timemory-run`).

```console
PYTHON_VERSION=3.8
git clone https://github.com/NERSC/timemory.git timemory-source
python${PYTHON_VERSION} -m pip install --user -r timemory-source/.requirements/runtime.txt
python${PYTHON_VERSION} -m pip install --user -r timemory-source/.requirements/mpi_runtime.txt
cmake -B timemory-build \
  -D CMAKE_INSTALL_PREFIX=timemory-install \
  -D CMAKE_BUILD_TYPE=RelWithDebInfo \
  -D TIMEMORY_USE_MPI=ON \
  -D TIMEMORY_USE_PYTHON=ON \
  -D TIMEMORY_BUILD_TOOLS=ON \
  -D PYTHON_EXECUTABLE=$(which python${PYTHON_VERSION})
cmake --build timemory-build --target all --parallel 8
cmake --build timemory-build --target install --parallel 8
export CMAKE_PREFIX_PATH=$PWD/timemory-install:${CMAKE_PREFIX_PATH}
export PATH=$PWD/timemory-install/bin:${PATH}
export DYLD_LIBRARY_PATH=$PWD/timemory-install/lib:${DYLD_LIBRARY_PATH}
export PYTHONPATH=$PWD/timemory-install/lib/python${PYTHON_VERSION}/site-packages:${PYTHONPATH}
```

> If you do not have python 3.8, replace `PYTHON_VERSION` with the appropriate python 3.x version.

#### Windows

Not generally recommended because `timem`, compiler instrumentation, and dynamic instrumentation are not available.
However, you can follow along with some of the other command-line tools and execute all the Python exercises.
