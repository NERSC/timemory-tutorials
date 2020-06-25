#!/bin/bash

timemory-run --stubs -o ./basic.inst -- ../../apps/build/basic/basic
./basic.inst

LD_PRELOAD=libtimemory.so ./basic.inst