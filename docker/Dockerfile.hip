FROM ubuntu:20.04 as builder

ENV SHELL /bin/bash
ENV BASH_ENV /etc/bash.bashrc
ENV DEBIAN_FRONTEND noninteractive
ENV PATH /opt/rocm/bin:/opt/conda/bin:${PATH}
ENV LD_LIBRARY_PATH /opt/rocm/lib:${LD_LIBRARY_PATH}
ENV CMAKE_PREFIX_PATH /opt/rocm

WORKDIR /tmp/build

RUN apt-get update && \
    apt-get dist-upgrade -y && \
    apt-get install -y software-properties-common wget curl gnupg2 kmod && \
    wget -q -O - https://repo.radeon.com/rocm/rocm.gpg.key | apt-key add - && \
    add-apt-repository -u -y ppa:ubuntu-toolchain-r/test && \
    echo 'deb [arch=amd64] https://repo.radeon.com/rocm/apt/4.3/ ubuntu main' | tee /etc/apt/sources.list.d/rocm.list && \
    apt-get update && \
    apt-get dist-upgrade -y && \
    apt-get install -y build-essential python3 google-perftools likwid git-core bash-completion rocm-dev libnuma-dev libboost1.71-all-dev libtbb-dev libgoogle-perftools-dev libunwind-dev libmpich-dev libpapi-dev libpfm4-dev libdw-dev

SHELL [ "/bin/bash", "--login", "-c" ]

ENV CC=cc
ENV CXX=hipcc
ENV FC=gfortran

RUN wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh && \
    bash miniconda.sh -b -p /opt/conda && \
    conda init bash && \
    conda config --set always_yes no --set changeps1 no && \
    conda update -y -c defaults -n base conda && \
    conda install -y -n base -c conda-forge -c defaults python=3.8 cmake=3.20.5 scikit-build numpy matplotlib-base pillow cython pip

RUN git clone https://github.com/NERSC/timemory.git timemory-source && \
    which -a python && \
    cd timemory-source && \
    python setup.py install \
        --enable-{caliper,hip,likwid,gotcha,mpi,gperftools,papi,perfetto,kokkos,kokkos-config,dyninst,build-dyninst} \
        --build-type=RelWithDebInfo \
        -G Unix\ Makefiles \
        -j 12 \
        -- -DDYNINST_BUILD_{ELFUTILS,LIBIBERTY}=ON -DCALIPER_WITH_{LIBUNWIND,LIBDW,LIBPFM,SAMPLER,MPI,MPIT,ROCM,PAPI}=ON && \
    python -m pip install -r .requirements/runtime.txt && \
    python -m pip install -r .requirements/mpi_runtime.txt

RUN apt-get autoclean && \
    apt-get autoremove --purge && \
    conda clean -a -y && \
    rm -rf /tmp/*

# Bare OS image to run the installed executables
FROM ubuntu:20.04

COPY --from=builder / /

RUN echo 'export PS1="\[$(tput bold)\]\[$(tput setaf 1)\][timemory]\[$(tput setaf 2)\] \u\[$(tput sgr0)\]:\w $ \[$(tput sgr0)\]"' >> ~/.bashrc

ENV HOME /home
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US
ENV LC_ALL C
ENV SHELL /bin/bash
ENV BASH_ENV /etc/bash.bashrc
ENV DEBIAN_FRONTEND noninteractive

WORKDIR /home
SHELL [ "/bin/bash", "--login", "-c" ]

# ENTRYPOINT [ "/runtime-entrypoint.sh" ]
ENTRYPOINT ["/bin/bash", "--rcfile", "/etc/profile", "-l"]
