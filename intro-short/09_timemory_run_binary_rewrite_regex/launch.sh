#!/bin/bash

timemory-run -E 'fibonacci*' '*reduce' -o ./basic.inst -- ../../apps/build/basic/basic
./basic.inst