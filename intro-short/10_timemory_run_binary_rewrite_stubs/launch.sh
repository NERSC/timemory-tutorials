#!/bin/bash

cd ../../apps/build/basic
timemory-run --stubs -- ./basic
./basic.inst

LD_PRELOAD=libtimemory.so ./basic.inst