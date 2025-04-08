================================================================================
This folder contains the code of the benchmark suite for evaluating 
caches' vulnerability to timing attacks.  It contains the code for:
1. Generate histogram for specific x86 machines, corresponding to Figure 1 
in the paper.
2. Generate vulnerability types (especially Strong and Weak type), 
corresponding to Figure 2 and Figure 3.
3. Generating and evaluating benchmark code, corresponding to Figure 4, 
Figure 5, and Figure 6, as well as Table 3, Table 4, and Table 5.
 

For more details, please read our paper:

Shuwen Deng, Wenjie Xiong, and Jakub Szefer, "A Benchmark Suite for Evaluating 
Caches’ Vulnerability to Timing Attacks", in Proceedings of the International 
Conference on Architectural Support for Programming Languages and Operating 
Systems (ASPLOS), March 2020.

This code can be downloaded from:

https://caslab.csl.yale.edu/code/cache-security-benchmarks/


================================================================================
0. Setup:

   Currently 1 and 3 are only tested on Intel and AMD x86 machines with Linux
   environment.  Please use a recent version of Linux for testing.

================================================================================
1. Generate histograms for specific x86 machines, corresponding to Figure 1 
in the paper. 

$ cd PATH_TO_README
$ ./gen_histogram.sh HIST_INFO
   
   Add HIST_INFO as the argument for setting the output machine and corresponding 
   information such as the histogram name.

   This code is especially for the machine with CACHE_LINE 0x40, L1_CACHE_SIZE 
   0x8000, L1_ASSOC 8, L2_CACHE_SIZE 0x40000, L2_ASSOC 8 case. These parameters 
   can be configured in timing_histogram.c to suit for your system.

   The generated numbers for drawing the histograms will be output to folder 
   histogram_output.
   

================================================================================
2. Generate vulnerability types (especially Strong and Weak type), 
corresponding to Figure 2 and Figure 3.

$ python cache_three_step_simulator.py NUM
   
   Use different NUM as the argument for specifying type of vulnerability
   to print: 0 for Strong type, 2 for Weak type, 1 for ineffective type.
   Timing list containing 66 types of timings can be changes if you want to
   consider different coherence models or differentiate more types of timings.


================================================================================
3. Generating and evaluating benchmark code, corresponding to Figure 4, 
Figure 5, and Figure 6, as well as Table 3, Table 4, and Table 5.

   Generate benchmark code and run the code on the machine

$ ./run.sh $machine $max_cycle $cache_size $cache_asso $cache_line $num_test 
$vic_num $probe_size $each_run $output_name > /dev/null

   $machine: currently suppport "x86" only.
   $max_cycle: maximum cycles for a specific memory related operation 
   (contain 8 sets of operations), e.g., 8000.
   $cache_size: bytes number of the L1 data cache, e.g., 32768.
   $cache_asso: set associativity of the L1 data cache, e.g., 8.
   $cache_line: cache line byte size, e.g., 64.
   $num_test * $each_run total number of tests run for a three-step 
   case, e.g., 10 * 30.
   $vic_num: machine number, it corresponds to 9 types of machine configurations
   we tested, mainly used for setting the core numbers of local and remote victim
   and attacker.
   (0: Intel Xeon E5-1620; 1: Intel Core i5-4570; 2: Intel Xeon E5-2690;  
   3: Intel Xeon E5-2667 on-chip; 4: Intel Xeon E5-2667 inter-chip;
   5: AMD EPYC 7571 ; 6: Broadwell E5-2686; 7: Intel Skylake P-8175; 8: AMD FX-8150.)
   You can also edit around line 1679 of gen_vulnerabilties_clean.c to configure
   for your own machines:
   cpu[0] for local attacker core number, cpu[1] for local victim core number,
   cpu[2] for remote attacker core number, cpu[3] for remote victim core number.
   $probe_size: currently set it to be "3" for 3 different candidates of "u":
   "a", "a_alias", "NIB".
   $output_name: a string for setting the output machine and corresponding 
   information as the output result file name, e.g, Intel_Xeon_E5-1620.
   /dev/null: it will output a lot of related information message for debugging, 
   normally can ignore it. In case of debugging, you can output it to a file, 
   e.g., > output.txt.

   Example: ./run.sh x86 8000 32768 8 64 10 0 3 30 try_machine > /dev/null 

   The generated benchmark codes and binaries will be output to folder gen_output.
   The generated benchmark result for the specific machines will be output to
   folder benchmark_output.

$ python plot_res.py RES_INFO
   
   Add RES_INFO as the argument for setting the corresponding result information.
   
   The default plot_res.py will plot original Figure 5 and Figure 6 using the output
   from 9 machine configurations discussed in the paper. To plot for your own 
   result of our own machine, configure result file names in around line 120 of 
   file plot_res.py.

   The generated benchmark result diagrams will be output to folder gen_figure.
