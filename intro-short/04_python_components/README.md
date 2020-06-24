# python_components

This example walks through the usage of individual timemory components for instrumentation purposes. The individual components are available in timemory.components sub-package. The timemory components can be instantiated and can be used to in-source instrument any Python code using the available class functions. For instance, the following code snippet creates an instances of `wall_clock` and `cpu_util` components for corresponding instrumentation.

## Example
```
''' contents of components.py '''

import timemory
import time

wc = timemory.components.wall_clock("timer1")
wc.push()
wc.start()
time.sleep(1)
wc.stop()
wc.pop()
data = wc.get()

cutil = timemory.components.cpu_util("cpu_util1")
cutil.push()
cutil.start()
time.sleep(1)
cutil.stop()
cutil.pop()
data2 = cutil.get()

timemory.finalize()
```
## Expected Output
```
$ python components.py
#--------------------- tim::manager initialized [rank=0][id=0][pid=14545] ---------------------#

[cpu_util]|0> Outputting 'timemory-components-output/cpu_util.json'...
[cpu_util]|0> Outputting 'timemory-components-output/cpu_util.txt'...
Opening 'timemory-components-output/cpu_util.jpeg' for output...
Closed 'timemory-components-output/cpu_util.jpeg'...

|------------------------------------------------------------------------------------------------|
|                     PERCENTAGE OF CPU-CLOCK TIME DIVIDED BY WALL-CLOCK TIME                    |
|------------------------------------------------------------------------------------------------|
|    LABEL      | COUNT | DEPTH | METRIC   | UNITS | SUM  | MEAN | MIN  | MAX  | STDDEV | % SELF |
|---------------|-------|-------|----------|-------|------|------|------|------|--------|--------|
| >>> cpu_util1 |     1 |     0 | cpu_util | %     |  0.0 |  0.0 |  0.0 |  0.0 |    0.0 |    0.0 |
|------------------------------------------------------------------------------------------------|

[wall]|0> Outputting 'timemory-components-output/wall.flamegraph.json'...
[wall]|0> Outputting 'timemory-components-output/wall.json'...
[wall]|0> Outputting 'timemory-components-output/wall.txt'...
Opening 'timemory-components-output/wall.jpeg' for output...
Closed 'timemory-components-output/wall.jpeg'...

|------------------------------------------------------------------------------------------------------|
|                               REAL-CLOCK TIMER (I.E. WALL-CLOCK TIMER)                               |
|------------------------------------------------------------------------------------------------------|
|   LABEL    | COUNT  | DEPTH  | METRIC | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|------------|--------|--------|--------|--------|--------|--------|--------|--------|--------|--------|
| >>> timer1 |      1 |      0 | wall   | sec    |  1.001 |  1.001 |  1.001 |  1.001 |  0.000 |  100.0 |
|------------------------------------------------------------------------------------------------------|


[metadata::manager::finalize]> Outputting 'timemory-components-output/metadata.json'...

#--------------------- tim::manager destroyed [rank=0][id=0][pid=14545] ---------------------#
```