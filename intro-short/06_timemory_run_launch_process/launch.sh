#!/bin/bash

timemory-run -- ../../apps/build/basic/basic

timemory-run -d wall_clock cpu_util peak_rss -- ../../apps/build/basic/basic