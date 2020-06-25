#!/bin/bash

cd ../../apps/build/basic

timemory-run -o ./basic.inst -- ./basic
./basic.inst

timemory-run -d wall_clock cpu_util peak_rss -o ./basic.inst -- ./basic
./basic.inst