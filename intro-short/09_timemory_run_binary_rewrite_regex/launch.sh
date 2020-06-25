#!/bin/bash

cd ../../apps/build/basic
timemory-run -E fibonacci* *reduce -o ./basic.inst -- ./basic
./basic.inst

timemory-run -E scatter* -o ./basic.inst -- ./basic
./basic.inst