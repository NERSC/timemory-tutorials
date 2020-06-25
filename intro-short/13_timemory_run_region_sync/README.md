# timemory-run region synchronization

This example demonstrates `timemory-run` dynamic instrumentation synchronization with in-source instrumentation in case of 'trace' and 'region' instrumentation modes. In case of `trace` mode, `timemory-run` has no effect on the instrumentation of in-source used components whereas the instrumentation is added for components not used in-source. On the other hand, in `region` mode, the instrumentaion of in-source used components is synchronized with `timemory-run` added instrumentation.

## About timemory-run

See [About timemory-run in 06_timemory_run_launch_process](../06_timemory_run_launch_process/README.md#about-timemory-run).

## Usage

```console
$ timemory-run --mode=[region or trace] [OPTIONS] -o [INSTRUMENTED_BINARY] -- [BINARY] [ARGS]
```

## Example

The PEAK RSS table for basic.region example in trace mode is unaffected of `timemory-run`.

```console
$ timemory-run --mode=trace -o trace.inst -- basic.region && trace.inst

|----------------------------------------------------------------------------------------------------------------------------|
|     MEASURES CHANGES IN THE HIGH-WATER MARK FOR THE AMOUNT OF MEMORY ALLOCATED IN RAM. MAY FLUCTUATE IF SWAP IS ENABLED    |
|----------------------------------------------------------------------------------------------------------------------------|
|             LABEL              | COUNT  | DEPTH  | METRIC   | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|--------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>> ./13_trace.inst            |      1 |      0 | peak_rss | MB     |  0.604 |  0.604 |  0.604 |  0.604 |  0.000 |    0.0 |
| >>> |_generation-region        |      1 |      1 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_thread-region            |      1 |      1 | peak_rss | MB     |  0.604 |  0.604 |  0.604 |  0.604 |  0.000 |   33.8 |
| >>>   |_thread-parallel-region |     10 |      2 | peak_rss | MB     |  0.400 |  0.040 |  0.000 |  0.400 |  0.126 |  100.0 |
| >>>     |_fibonacci            |     10 |      3 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_mpi-region               |      1 |      1 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_fibonacci              |     20 |      2 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|----------------------------------------------------------------------------------------------------------------------------|
```

The PEAK RSS table for basic.region example in region mode is synchronized with `timemory-run`.

```console
$ timemory-run --mode=trace -o region.inst -- basic.region && region.inst

|------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                        MEASURES CHANGES IN THE HIGH-WATER MARK FOR THE AMOUNT OF MEMORY ALLOCATED IN RAM. MAY FLUCTUATE IF SWAP IS ENABLED                       |
|------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|                                LABEL                                 | COUNT  | DEPTH  | METRIC   | UNITS  |  SUM   | MEAN   |  MIN   |  MAX   | STDDEV | % SELF |
|----------------------------------------------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>> ./13_region.inst                                                 |      1 |      0 | peak_rss | MB     |  0.748 |  0.748 |  0.748 |  0.748 |  0.000 |    0.0 |
| >>> |_generation-region                                              |      1 |      1 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_thread-region                                                  |      1 |      1 | peak_rss | MB     |  0.748 |  0.748 |  0.748 |  0.748 |  0.000 |   87.2 |
| >>>   |_thread-parallel-region                                       |     10 |      2 | peak_rss | MB     |  0.096 |  0.010 |  0.000 |  0.096 |  0.030 |    0.0 |
| >>>     |_fibonacci                                                  |     10 |      3 | peak_rss | MB     |  0.096 |  0.010 |  0.000 |  0.096 |  0.030 |    0.0 |
| >>>       |_fibonacci                                                |     10 |      4 | peak_rss | MB     |  0.096 |  0.010 |  0.000 |  0.096 |  0.030 |    0.0 |
| >>>         |_impl_fibonacci                                         |     25 |      5 | peak_rss | MB     |  0.096 |  0.004 |  0.000 |  0.096 |  0.019 |    0.0 |
| >>>           |_impl_fibonacci                                       |     40 |      6 | peak_rss | MB     |  0.096 |  0.002 |  0.000 |  0.096 |  0.015 |  100.0 |
| >>>             |_impl_fibonacci                                     |     50 |      7 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>               |_impl_fibonacci                                   |     42 |      8 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|----------------------------------------------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>>                 |_impl_fibonacci                                 |     35 |      9 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                   |_impl_fibonacci                               |     16 |     10 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                     |_impl_fibonacci                             |     10 |     11 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                       |_impl_fibonacci                           |      2 |     12 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                         |_impl_fibonacci                         |      1 |     13 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>> |_mpi-region                                                     |      1 |      1 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_fibonacci                                                    |     20 |      2 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     |_fibonacci                                                  |     20 |      3 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>       |_impl_fibonacci                                           |    100 |      4 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>         |_impl_fibonacci                                         |    330 |      5 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|----------------------------------------------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>>           |_impl_fibonacci                                       |    825 |      6 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>             |_impl_fibonacci                                     |   1584 |      7 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>               |_impl_fibonacci                                   |   2640 |      8 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                 |_impl_fibonacci                                 |   3432 |      9 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                   |_impl_fibonacci                               |   4290 |     10 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                     |_impl_fibonacci                             |   4004 |     11 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                       |_impl_fibonacci                           |   4004 |     12 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                         |_impl_fibonacci                         |   2730 |     13 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                           |_impl_fibonacci                       |   2275 |     14 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                             |_impl_fibonacci                     |   1120 |     15 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|----------------------------------------------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>>                               |_impl_fibonacci                   |    800 |     16 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                 |_impl_fibonacci                 |    272 |     17 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                   |_impl_fibonacci               |    170 |     18 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                     |_impl_fibonacci             |     36 |     19 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                       |_impl_fibonacci           |     20 |     20 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                         |_impl_fibonacci         |      2 |     21 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                           |_impl_fibonacci       |      1 |     22 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_allreduce                                                    |      1 |      2 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_impl_fibonacci                                               |     12 |      2 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     |_impl_fibonacci                                             |     78 |      3 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|----------------------------------------------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>>       |_impl_fibonacci                                           |    286 |      4 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>         |_impl_fibonacci                                         |   1001 |      5 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>           |_impl_fibonacci                                       |   2002 |      6 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>             |_impl_fibonacci                                     |   5005 |      7 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>               |_impl_fibonacci                                   |   6435 |      8 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                 |_impl_fibonacci                                 |  12870 |      9 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                   |_impl_fibonacci                               |  11440 |     10 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                     |_impl_fibonacci                             |  19448 |     11 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                       |_impl_fibonacci                           |  12376 |     12 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                         |_impl_fibonacci                         |  18564 |     13 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|----------------------------------------------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>>                           |_impl_fibonacci                       |   8568 |     14 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                             |_impl_fibonacci                     |  11628 |     15 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                               |_impl_fibonacci                   |   3876 |     16 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                 |_impl_fibonacci                 |   4845 |     17 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                   |_impl_fibonacci               |   1140 |     18 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                     |_impl_fibonacci             |   1330 |     19 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                       |_impl_fibonacci           |    210 |     20 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                         |_impl_fibonacci         |    231 |     21 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                           |_impl_fibonacci       |     22 |     22 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                             |_impl_fibonacci     |     23 |     23 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|----------------------------------------------------------------------|--------|--------|----------|--------|--------|--------|--------|--------|--------|--------|
| >>>                                               |_impl_fibonacci   |      1 |     24 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>                                                 |_impl_fibonacci |      1 |     25 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>   |_scatter_gather                                               |     10 |      2 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
| >>>     |_create_rand_nums                                           |     10 |      3 | peak_rss | MB     |  0.000 |  0.000 |  0.000 |  0.000 |  0.000 |    0.0 |
|------------------------------------------------------------------------------------------------------------------------------------------------------------------|
```