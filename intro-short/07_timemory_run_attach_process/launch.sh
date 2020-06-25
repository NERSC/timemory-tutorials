#!/bin/bash 

../../apps/build/basic/basic & PID=$!

timemory-run --pid ${PID} -- ../../apps/build/basic/basic