# Python Toolkit

The timemory python bindings provide stand-alone classes for collecting data
in tools and full access to the storage for creating GUIs and/or analysis
tools in Python.

## Individual Components

Each individual component is available as a stand-alone Python class.
The intention of making these individual components available is such that
tools can use these types to create custom tools. Thus, unless `push()` and `pop()`
are called on these objects, instances of these classes
will not store any data in the timemory call-graph (when applicable).

```python
import time
from timemory.component import WallClock

# create wall clock component instance
wc = WallClock("<label>")

# start the clock
wc.start()

# sleep
time.sleep(2)

# stop the clock
wc.stop()

# get data
result = wc.get()
```

## Storage

Timemory allows direct access to the call-graph storage.
The call-graph storage is the accumulated data for components which occur through
push and pop operations. These push and pop operations are implicitly
performed via decorators, context-managers, the function profiler, and the
trace profiler and may be explicitly performed when using individual components.
Internally, timemory stores call-graph entries in a minimal representation
and then packs the results together with more information when the data is requested.
Thus, one should not expect manipulation of the data provided by these routines to
be propagated to timemory internally.

Call-graph storage is available in two different layouts. The first represents
the call-graph results for each process as a one-dimensional array where the hierarchy
is represented through indentation of the string identifiers, a depth field, and
an array of hash values. The second represents the call-graph entries as a nested
data structure where each entry has a value and list of children.

### Storage Example

```python
#!/usr/bin/env python

import time
import timemory
from timemory.util import marker
from timemory.component import WallClock
from timemory.storage import WallClockStorage

@marker(["wall_clock"])
def foo():
    """Generate some data for timemory"""
    time.sleep(1)

    wc = WallClock("bar")
    for i in range(0, 10):
        # push every even iteration
        if i % 2 == 0:
            wc.push()
        wc.start()
        time.sleep(0.1 * (i + 1))
        wc.stop()
        # pop every odd iteration
        if i % 2 == 1:
            wc.pop()


def print_result():
    """
    Print the call-graph storage via the flat layout
    """

    print("\n#{}#\n# Storage Result".format("-" * 40))

    indent = "  "
    for itr in WallClockStorage.get():
        print("#{}#".format("-" * 40))
        print("{}{:20} : {}".format(indent, "Thread id", itr.tid()))
        print("{}{:20} : {}".format(indent, "Process id", itr.pid()))
        print("{}{:20} : {}".format(indent, "Depth", itr.depth()))
        print("{}{:20} : {}".format(indent, "Hash", itr.hash()))
        print("{}{:20} : {}".format(indent, "Rolling hash", itr.rolling_hash()))
        print("{}{:20} : {}".format(indent, "Prefix", itr.prefix()))
        print("{}{:20} : {}".format(indent, "Hierarchy", itr.hierarchy()))
        print("{}{:20} : {}".format(indent, "Data object", itr.data()))
        print("{}{:20} : {}".format(indent, "Statistics", itr.stats()))


def print_tree(data=None, depth=0):
    """
    Print the call-graph storage via the nested layout
    """

    if data is None:
        print("\n#{}#\n# Storage Tree".format("-" * 40))
        data = WallClockStorage.get_tree()

    def print_value(itr, indent):
        print("{}{:20} : {}".format(indent, "Thread id", itr.tid()))
        print("{}{:20} : {}".format(indent, "Process id", itr.pid()))
        print("{}{:20} : {}".format(indent, "Depth", itr.depth()))
        print("{}{:20} : {}".format(indent, "Hash", itr.hash()))
        print("{}{:20} : {}".format(indent, "Inclusive data", itr.inclusive().data()))
        print("{}{:20} : {}".format(indent, "Inclusive stat", itr.inclusive().stats()))
        print("{}{:20} : {}".format(indent, "Exclusive data", itr.exclusive().data()))
        print("{}{:20} : {}".format(indent, "Exclusive stat", itr.exclusive().stats()))

    indent = "  " * depth
    for itr in data:
        print("{}#{}#".format(indent, "-" * 40))
        print_value(itr.value(), indent)
        print_tree(itr.children(), depth + 1)


if __name__ == "__main__":
    # disable automatic output
    timemory.settings.auto_output = False

    foo()
    print_result()
    print_tree()
```

### Storage Output

```console

#----------------------------------------#
# Storage Result
#----------------------------------------#
  Thread id            : 0
  Process id           : 4385
  Depth                : 0
  Hash                 : 9631199822919835227
  Rolling hash         : 9631199822919835227
  Prefix               : >>> foo
  Hierarchy            : [9631199822919835227]
  Data object          :    6.534 sec wall
  Statistics           : [sum: 6.53361] [min: 6.53361] [max: 6.53361] [sqr: 42.6881] [count: 1]
#----------------------------------------#
  Thread id            : 0
  Process id           : 4385
  Depth                : 1
  Hash                 : 11474628671133349553
  Rolling hash         : 2659084420343633164
  Prefix               : >>> |_bar
  Hierarchy            : [9631199822919835227, 11474628671133349553]
  Data object          :    5.531 sec wall
  Statistics           : [sum: 5.53115] [min: 0.307581] [max: 0.307581] [sqr: 7.71154] [count: 5]

#----------------------------------------#
# Storage Tree
#----------------------------------------#
Thread id            : {0}
Process id           : {4385}
Depth                : -1
Hash                 : 0
Prefix               : unknown-hash=0
Inclusive data       :    0.000 sec wall
Inclusive stat       : [sum: 0] [min: 0] [max: 0] [sqr: 0] [count: 0]
Exclusive data       :   -6.534 sec wall
Exclusive stat       : [sum: 0] [min: 0] [max: 0] [sqr: 0] [count: 0]
  #----------------------------------------#
  Thread id            : {0}
  Process id           : {4385}
  Depth                : 0
  Hash                 : 9631199822919835227
  Prefix               : foo
  Inclusive data       :    6.534 sec wall
  Inclusive stat       : [sum: 6.53361] [min: 6.53361] [max: 6.53361] [sqr: 42.6881] [count: 1]
  Exclusive data       :    1.002 sec wall
  Exclusive stat       : [sum: 1.00246] [min: 6.53361] [max: 6.53361] [sqr: 34.9765] [count: 1]
    #----------------------------------------#
    Thread id            : {0}
    Process id           : {4385}
    Depth                : 1
    Hash                 : 11474628671133349553
    Prefix               : bar
    Inclusive data       :    5.531 sec wall
    Inclusive stat       : [sum: 5.53115] [min: 0.307581] [max: 0.307581] [sqr: 7.71154] [count: 5]
    Exclusive data       :    5.531 sec wall
    Exclusive stat       : [sum: 5.53115] [min: 0.307581] [max: 0.307581] [sqr: 7.71154] [count: 5]
```

Note the first entry of storage tree has a negative depth and hash of zero. Nodes such of these
are "dummy" nodes which timemory keeps internally as bookmarks for root nodes and thread-forks
(parent call-graph location when a child thread was initialized or returned to "sea-level").
These entries can be discarded and a member function `is_dummy()` exists to help identify these nodes, e.g.:

```python
    for itr in data:
        if itr.value().is_dummy():
            print_tree(itr.children(), depth)
        else:
            print("{}#{}#".format(indent, "-" * 40))
            print_value(itr.value(), indent)
            print_tree(itr.children(), depth + 1)
```

Future versions of timemory may eliminate these reporting these nodes.
