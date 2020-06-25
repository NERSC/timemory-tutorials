#!/bin/bash

timemory-run -o ./basic.inst -- ../../apps/build/basic/basic
./basic.inst

timemory-run -d wall_clock cpu_util peak_rss -o ./basic.inst -- ../../apps/build/basic/basic
./basic.inst