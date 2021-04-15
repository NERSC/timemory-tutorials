# Python API and Tools

Here you'll find

[example.py](example.py), a sample script we'll use for our exercises.

[overview.md](overview.md), the document that will outline some of the Python capabilities in timemory.

The [solutions](solutions) directory which contains the answers to exercises we discuss in
[overview.md](overview.md). Don't look until you've tried!


## We'll show/run these examples on NERSC Cori

Here are the directions to use timemory with Python on NERSC Cori. Users on
other systems will need to adjust these directions accordingly. 

We're only going to demonstrate CPU capabilities today, although GPU profiling
and tracing is also possible.

Let's load the NERSC python and timemory modules and make sure our example.py runs:

```
module load python timemory
```

```
stephey@cori06:~> python example.py 
Running fibonacci...
Running inefficient...
Answer = 509443184046766.0
```

And we'll test that timemory is working:

```
stephey@cori06:~> python -c "import timemory"
stephey@cori06:~> 
```

We'll demonstate some capabilities and provide some exercises in [overview.md](overview.md).
Solutions to the exercises can be found in [solutions](solutions).

## More information

For more information than we could cover in our relatively short tutorial,
please see the [extended](extended) directory in this folder. Here you'll find a lot more
details and examples.
