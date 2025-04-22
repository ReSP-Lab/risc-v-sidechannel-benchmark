# Systematic Assessment of Cache Timing Vulnerabilities on RISC-V Processors

This repository contains the translated benchmark suite used for our
submission to ESORICS 2025.

Authors: Cédrick Austa, Jan Tobias Mühlberg, and Jean-Michel Dricot.

## Original work

Our benchmark suite is a refactoring and a translation for the RISC-V ISA from
the following work:

> Shuwen Deng, Wenjie Xiong, and Jakub Szefer, "A Benchmark Suite for Evaluating
> Caches’ Vulnerability to Timing Attacks", in Proceedings of the International
> Conference on Architectural Support for Programming Languages and Operating
> Systems (ASPLOS), March 2020.

Their code can be downloaded from:
https://caslab.csl.yale.edu/code/cache-security-benchmarks/

Their code is also available in the [original-benchmark](./original-benchmark/)
folder or in the `original-benchmark` branch.

## Usage

Our benchmark suite is composed of two main components from the previous
original work which were modified for our purpose:
1. an evaluation of the different timing types existing on the targets;
2. an evaluation of the different vulnerabilities, out of the 88 strong
    vulnerabilities identified in the original work, present on the targets.

### Setup

Before using the benchmark suite, ensure to add the target support by:
1. adding a dedicated board file into the [boards folder](./include/boards/),
    based on the [template.h](./include/boards/template.h) file;
2. adequately modifying the [board.h](./include/board.h) file;
3. adapting the [Makefile](./Makefile).

On the target board, enable support for memory mapping using huge pages:
```
# verify the support
cat /proc/meminfo | grep -i page
# verify if feature is already enabled (!=0)
cat /proc/sys/vm/nr_hugepages
# otherwise, enable it
echo 128 > /proc/sys/vm/nr_hugepages
```

To adapt cache configuration to your target, first consult the related
documentation, then use `getconf -a | grep -i cache` if information is missing.

When using Python for result processing scripts, use the [requirements.txt](./requirements.txt) file with Python 3.12.
We recommend the use of [pyenv](https://github.com/pyenv/pyenv/).


### Evaluation of timing types

To evaluate the different timing types, you have to:
1. run the related Makefile recipe (e.g., `make histogram-unmatched`);
2. upload the binary to the board (if it is not build on the target);
3. (optional, depends on 2.) run `mkdir -p histogram_output`;
4. run `timing_histogram [target_name]`;
5. download the output folder;
6. plot the results with `python scripts/plot_histogram.py [input_file] [output_file]`.


### Evaluation of vulnerabilities

To evaluate the existing vulnerabilities, you have to:
1. run the related Makefile recipe (e.g., `make benchmark-unmatched`);
2. run `sh benchmark_scripts/[target_symbol]_EachRun[...]_NumTest[...]_MaxCycle[...].sh 2> tmp1.txt &` (it applies previously generated compilation commands,
    target symbol is the macro you used while adding support for your board);
3. upload the `benchmark_output` and `scripts` folders to the board
    (if binaries were not build on the target, **strongly recommended**);
4. run `./scripts/run_benchmark.sh [target_symbol] [target_name] > tmp2.txt &`;
5. (optional) run `disown -h` (useful to avoid waiting for the end of execution, which can be monitored with `ps aux | grep -i run_benchmark`);
8. download the output folder;
9. adapt the CPU list in `scripts/plot_benchmark.py` and plot the results with `python scripts/plot_benchmark.py`.

