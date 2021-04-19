# Python API and Tools

Here you'll find

[example.py](example.py), a sample script we'll use for our exercises.

[overview.md](overview.md), the document that will outline some of the Python capabilities in timemory. This the primary page we'll be using for the Python API portion of the timemory tutorial.

The [solutions](solutions) directory which contains the answers to exercises we discuss in
[overview.md](overview.md). We encourage you to try the exercises before you check the answers!


## We'll show/run these examples on NERSC Cori

Here are the directions to use timemory with Python on NERSC Cori. Users on
other systems will need to adjust these directions accordingly. 

We're only going to demonstrate CPU capabilities today, although GPU profiling
and tracing is also possible.

Let's load the timemory module and make sure our example.py runs:

```
stephey@cori07:~> module load timemory
```

Note that the timemory module automatically loads the python module:

```
stephey@cori07:~> which python
/usr/common/software/python/3.8-anaconda-2020.11/bin/python
```

Let's make sure our example script will run:

```
stephey@cori07:~> python example.py 
Running fibonacci...
Running inefficient...
Answer = 509443184046766.0
```

And lastly we'll test that timemory is working:

```
stephey@cori07:~> python -c "import timemory"
stephey@cori07:~> 
```

We'll demonstate some capabilities and provide some exercises in [overview.md](overview.md).
Solutions to the exercises can be found in [solutions](solutions).

## More information

For more information than we could cover in our relatively short tutorial,
please see the [extended](extended) directory in this folder. Here you'll find a lot more
details and examples.
