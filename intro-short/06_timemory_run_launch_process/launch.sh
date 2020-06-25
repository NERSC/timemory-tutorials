#!/bin/bash

cd ../../apps/build/basic
timemory-run -- ./basic
timemory-run -d wall_clock cpu_util peak_rss -- ./basic