/*
* This file evaluates eviction parameters
*
* Copyright (C) 2024-2025 Cédrick Austa <cedrick.austa@ulb.be>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation,
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <dlfcn.h>
#include <sched.h>

#include "board.h"
#include "asm_functions.h"
#include "evict.h"

/* CONSTANTS */
/* MACROS */
/* Macros used for the experiments */

// TODO:
// - remove D_MAX
// - replace by using s_max - 1 in loop below
// - adapt NUM_L1_STRATS, NUM_L2_STRATS and NUM_L3_STRATS to use "S_MAX_Lx-1"
//   instead instead of "D_MAX_Lx"
//   => some cases will never be encountered because d < s to satisfy
// - create arrays for parameters
// - print one row (the array) for each parameter
// - initialize parameters arrays with -1
// - in cases where a parameter (in printing functions) would be equal to -1,
//   skip the printing of the strategy, parameters, histograms
#define C_MIN       1
#define C_MAX       5
#define L_MIN       1
#define L_MAX       3
#define D_MIN       1
#define S_MIN_L1    L1_ASSOC
#define S_MAX_L1    2*L1_ASSOC
#define D_MAX_L1    S_MIN_L1 - 1
#if LLC > L1
#define S_MIN_L2    L2_ASSOC
#define S_MAX_L2    2*L2_ASSOC
#define D_MAX_L2    S_MIN_L2 - 1
#endif // LLC > L1
#if LLC > L2
#define S_MIN_L3    L3_ASSOC
#define S_MAX_L3    2*L3_ASSOC
#define D_MAX_L3    S_MIN_L3 - 1
#endif // LLC > L2


#ifdef NO_L3_CACHE
#if defined(L2_ASSOC)
#define NUM_CANDIDATES 16*L2_ASSOC
#endif
#else
#if defined(L3_ASSOC)
#define NUM_CANDIDATES 16*L3_ASSOC
#endif
#endif /* ifdef NO_L3_CACHE */


#define MAX_CYCLE 2500  // a reasonable maximum number of cycles
#define NUM_L1_STRATS   (C_MAX+1-C_MIN)\
                        *(L_MAX+1-L_MIN)\
                        *(D_MAX_L1+1-D_MIN)\
                        *(S_MAX_L1+1-S_MIN_L1)
#define NUM_L2_STRATS   (C_MAX+1-C_MIN)\
                        *(L_MAX+1-L_MIN)\
                        *(D_MAX_L2+1-D_MIN)\
                        *(S_MAX_L2+1-S_MIN_L2)
#ifdef NO_L3_CACHE
#define NUM_L3_STRATS   0
#else
#define NUM_L3_STRATS   (C_MAX+1-C_MIN)\
                        *(L_MAX+1-L_MIN)\
                        *(D_MAX_L3+1-D_MIN)\
                        *(S_MAX_L3+1-S_MIN_L3)
#endif /* ifdef NO_L3_CACHE */
#define NUM_STRATEGIES  NUM_L1_STRATS+NUM_L2_STRATS+NUM_L3_STRATS

#define START_ARR_SIZE  NUM_CANDIDATES+1


/* VARIABLES */
/* Constants */
const char* cycles_str = "Cycles";
const char* cache_mapping[] = {
    "L1_CACHE",
    "L2_CACHE",
    "L3_CACHE"
};
const int threshold = 200;
const int test_cnt = 10000;

/* Variables */
char* probe;
char __attribute__((aligned(4096))) probe_arr[SIZE_GT_LLC]; // ensure to have an array longer than LLC size

int histogram[NUM_STRATEGIES][MAX_CYCLE]={0};
int coarse_histogram[NUM_STRATEGIES][MAX_CYCLE/10]={0};

char** target_addresses;
char* start_L1[START_ARR_SIZE];
char* start_L2[START_ARR_SIZE];
char* start_L3[START_ARR_SIZE];
char* conflict_set[NUM_CANDIDATES];
char* lines[NUM_CANDIDATES];

int t=0;

int conflict_bar = 0;

long long ave_time_cycle_arr[NUM_STRATEGIES] =  {0};

int min_time_cycle_arr[NUM_STRATEGIES] = {0};
int min_time_freq_arr[NUM_STRATEGIES] = {0};


/**
 * Initialize the array used for the delay measurement, i.e. `probe_arr`,
 * and the associated `probe` pointer.
 */
void initialize_probe_array()
{
    // initialize each pointer from the probe array with its index
    // -> each pointer is pointing to a byte address being equal
    //    to its current index
    for (int i = 0; i < SIZE_GT_LLC; ++i)
        probe_arr[i] = i;

    // probe initialization creates a double indirection:
    // - first level of indirection: it points to the beginning of probe_arr
    // - second level of indirection: each element of probe_arr is a pointer
    probe = probe_arr;  // probe: char*, probe_arr: char []
}

/**
 * Initialize the arrays used for the delay measurement, i.e. `probe_arr`,
 * and the associated `probe` pointer, and the `start` array.
 */
void initialize_pointer_arrays()
{
    for(int i=0; i < START_ARR_SIZE; i++)
    {
        start_L1[i]=&probe[i*L1_STRIDE];
    }
    for(int i=0; i < START_ARR_SIZE; i++)
    {
        start_L2[i]=&probe[i*L2_STRIDE];
    }
#ifndef NO_L3_CACHE
    for(int i=0; i < START_ARR_SIZE; i++)
    {
        start_L3[i]=&probe[i*L3_STRIDE];
    }
#endif /* ifndef NO_L3_CACHE */
}


unsigned long test_strategy(int c, int d, int l, int s)
{
    unsigned long t=0;

    mread(target_addresses[0]);
    evict(target_addresses, c, d, l, s);

    //measure latency
    return mread_timing(target_addresses[0]);
}


/**
 * Evaluate each eviction strategy, from the simplest (sequential)
 * to more complex ones.
 */
void evaluate_strategies()
{
    unsigned int t;
    int counter_hist=0;

    for (int cache_level = L1; cache_level <= LLC; cache_level++)
    {
        printf("Generating histograms for L%d caches...\n", cache_level+1);

        uint8_t s_min, c_min, d_min, l_min;
        uint8_t s_max, c_max, d_max, l_max;
        d_min = D_MIN;
        c_min = C_MIN;
        l_min = L_MIN;
        c_max = C_MAX;
        l_max = L_MAX;
        if (cache_level == L1)
        {
            d_max = D_MAX_L1;
            s_min = S_MIN_L1;
            s_max = S_MAX_L1;
            target_addresses = start_L1;
        }
        else if (cache_level == L2)
        {
            d_max = D_MAX_L2;
            s_min = S_MIN_L2;
            s_max = S_MAX_L2;
            target_addresses = start_L2;
        }
# if LLC > L2
        else if (cache_level == L3)
        {
            d_max = D_MAX_L3;
            s_min = S_MIN_L3;
            s_max = S_MAX_L3;
            target_addresses = start_L3;
        }
#endif /* if LLC > L2 */

        for (int l = l_min; l <= l_max; l++)
        {
            for (int c = c_min; c <= c_max; c++)
            {
                for (int d = d_min; d <= d_max; d++)
                {
                    for (int s = s_min; s <= s_max; s++)
                    {
                        printf("L%d cache: ", cache_level+1);
                        printf("testing P-%d-%d-%d-%d strategy...\n", c, d, l, s);
                        for (int i=0;i<test_cnt;i++)
                        {
                            t = test_strategy(c, d, l, s);
                            ave_time_cycle_arr[counter_hist] += t;
                            if(t<MAX_CYCLE)
                            {
                                histogram[counter_hist][t]++;
                                coarse_histogram[counter_hist][t/10]++;
                            }
                            else
                            {
                                histogram[counter_hist][MAX_CYCLE-1]++;
                                coarse_histogram[counter_hist][MAX_CYCLE/10-1]++;
                            }
                        }
                        // Update histogram index for each scenario.
                        counter_hist++;
                    }
                }
            }
        }
    }
}

void print_to_files(FILE *fp, FILE* fp_coarse)
{
    // header
    // - CSV delimiter
    fprintf(fp, "sep=,\n");
    fprintf(fp_coarse, "sep=,\n");

    // - cache level
    fprintf(fp, "Level,");
    fprintf(fp_coarse, "Level,");
    for (int strat = 0; strat < NUM_STRATEGIES; strat++)
    {
        if (strat < NUM_L1_STRATS)
        {
            fprintf(fp, "L1,");
            fprintf(fp_coarse, "L1,");
        }
        else if (strat >= NUM_L1_STRATS && strat < NUM_L1_STRATS+NUM_L2_STRATS)
        {
            fprintf(fp, "L2,");
            fprintf(fp_coarse, "L2,");
        }
        else
        {
            fprintf(fp, "L3,");
            fprintf(fp_coarse, "L3,");
        }
    }
    fprintf(fp, "\n");
    fprintf(fp_coarse, "\n");

    // - strategies
    fprintf(fp, "Strategy,");
    fprintf(fp_coarse, "Strategy,");
    for (int cache_level = L1; cache_level <= LLC; cache_level++)
    {
        uint8_t s_min, c_min, d_min, l_min;
        uint8_t s_max, c_max, d_max, l_max;
        d_min = D_MIN;
        c_min = C_MIN;
        l_min = L_MIN;
        c_max = C_MAX;
        l_max = L_MAX;
        if (cache_level == L1)
        {
            d_max = D_MAX_L1;
            s_min = S_MIN_L1;
            s_max = S_MAX_L1;
        }
        else if (cache_level == L2)
        {
            d_max = D_MAX_L2;
            s_min = S_MIN_L2;
            s_max = S_MAX_L2;
        }
#if LLC > L2
        else if (cache_level == L3)
        {
            d_max = D_MAX_L3;
            s_min = S_MIN_L3;
            s_max = S_MAX_L3;
        }
#endif // LLC > L2

        for (int l = l_min; l <= l_max; l++)
        { for (int c = c_min; c <= c_max; c++)
            { for (int d = d_min; d <= d_max; d++)
                { for (int s = s_min; s <= s_max; s++)
        {
            fprintf(fp, "P-%d-%d-%d-%d,", c, d, l, s);
            fprintf(fp_coarse, "P-%d-%d-%d-%d,", c, d, l, s);
        }}}}
    }
    fprintf(fp, "\n");
    fprintf(fp_coarse, "\n");

    // histogram
    for(int i=0;i<MAX_CYCLE;i++)
    {
        fprintf(fp, "%d,", i);
        for (int j = 0; j < NUM_STRATEGIES; ++j)
        {
            fprintf(fp, "%d,", histogram[j][i]);
        }
        fprintf(fp, "\n");

        for (int j = 0; j < NUM_STRATEGIES; ++j)
        {
            if (histogram[j][i]>min_time_freq_arr[j])
            {
                min_time_freq_arr[j] = histogram[j][i];
                min_time_cycle_arr[j] = i;
            }
        }
    }
    // coarse histogram
    for(int i=0;i<MAX_CYCLE/10;i++)
    {
        fprintf(fp_coarse, "%d,", i*10);
        for (int j = 0; j < NUM_STRATEGIES; ++j)
        {
            fprintf(fp_coarse, "%d,", coarse_histogram[j][i]);
        }
        fprintf(fp_coarse, "\n");
    }
    for (int j = 0; j < NUM_STRATEGIES; ++j)
    {
        ave_time_cycle_arr[j] = ave_time_cycle_arr[j]/test_cnt;
    }

    fclose(fp);
    fclose(fp_coarse);
}

void print_cli_summary(char* file_info)
{
    printf("The maximum frequency cycle number of %d strategies timings are given below \n", NUM_STRATEGIES);
    for (int j = 0; j < NUM_STRATEGIES; ++j)
    {
        printf("%d,", min_time_cycle_arr[j]);
    }
    printf("\n");

    printf("The average cycle number of %d types of strategies are given below \n", NUM_STRATEGIES);
    for (int j = 0; j < NUM_STRATEGIES; ++j)
    {
        printf("%d,", ave_time_cycle_arr[j]);
    }
    printf("\n");

    printf("Histogram numbers are output to eviction_output/histogram_%s.csv and eviction_output/coarse_histogram_%s.csv\n", file_info, file_info);
}


int main(int argc, char *argv[])
{
    char* file_long[50];
    sprintf(file_long, "eviction_output/histogram_%s.csv", argv[1]);

    char* file_name[50];
    sprintf(file_name, "eviction_output/coarse_histogram_%s.csv", argv[1]);
    char* file_info = argv[1];


    FILE *fp = fopen(file_long, "w");
    FILE *fp_coarse = fopen(file_name, "w");


    initialize_probe_array();
    initialize_pointer_arrays();


    //--- STRATEGY EVALUATIONS ---
    evaluate_strategies();

    //--- PRINT RESULTS ---
    // to files
    print_to_files(fp, fp_coarse);
    // to cli
    print_cli_summary(file_info);

    return 0;
}
