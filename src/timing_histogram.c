/*
* This file generates histogram for specific machine
*
* Copyright (C) 2024-2025 Cédrick Austa <cedrick.austa@ulb.be>
* Copyright (C) 2020 Shuwen Deng <shuwen.deng@yale.edu>
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

#define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)
#define MAP_HUGE_1GB (30 << MAP_HUGE_SHIFT)

/* All cases leading to the different 66 timings in the article */
#define L1_HIT_CLEAN 0
#define L2_HIT_CLEAN 1
#define L3_HIT_CLEAN 2
#define REMOTE_L1_HIT_CLEAN 3
#define REMOTE_L2_HIT_CLEAN 4
#define REMOTE_L3_HIT_CLEAN 5

#define L1_HIT_DIRTY 6
#define L2_HIT_DIRTY 7
#define L3_HIT_DIRTY 8
#define REMOTE_L1_HIT_DIRTY 9
#define REMOTE_L2_HIT_DIRTY 10
#define REMOTE_L3_HIT_DIRTY 11
#define DRAM_HIT 12

#define L1_REMOTE_L1_HIT_CLEAN 13
#define L1_REMOTE_L2_HIT_CLEAN 14
#define L1_REMOTE_L3_HIT_CLEAN 15
#define L2_REMOTE_L1_HIT_CLEAN 16
#define L2_REMOTE_L2_HIT_CLEAN 17
#define L2_REMOTE_L3_HIT_CLEAN 18
#define L3_REMOTE_L1_HIT_CLEAN 19
#define L3_REMOTE_L2_HIT_CLEAN 20
#define L3_REMOTE_L3_HIT_CLEAN 21

#define READ 0
#define WRITE 1
#define FLUSH 2


/* MACROS */
/* Macros used for the experiments */
#ifdef NO_L3_CACHE
#if defined(L2_ASSOC)
#define NUM_CANDIDATES 16*L2_ASSOC   // 2560 // L2 assoc. * 16 (factor)
#endif
#else
#if defined(L3_ASSOC)
#define NUM_CANDIDATES 16*L3_ASSOC   // 2560 // L3 assoc. * 16 (factor)
#endif
#endif /* ifdef NO_L3_CACHE */

#ifdef NO_FLUSH
#define NUM_CALIBRE 44
#else
#define NUM_CALIBRE 66
#endif /* ifdef NO_FLUSH */
#define NUM_CALIBRE_SINGLE 22

#define MAX_CYCLE 1500  // a reasonable maximum number of cycles

#define START_ARR_SIZE  NUM_CANDIDATES+1


/* VARIABLES */
/* Constants */
const char* cycles_str = "Cycles";
const char* calibre_mapping[] = {
    "L1_CLEAN",
    "L2_CLEAN",
    "L3_CLEAN",
    "REMOTE_L1_CLEAN",
    "REMOTE_L2_CLEAN",
    "REMOTE_L3_CLEAN",
    "L1_DIRTY",
    "L2_DIRTY",
    "L3_DIRTY",
    "REMOTE_L1_DIRTY",
    "REMOTE_L2_DIRTY",
    "REMOTE_L3_DIRTY",
    "DRAM",
    "L1_REMOTE_L1_CLEAN",
    "L1_REMOTE_L2_CLEAN",
    "L1_REMOTE_L3_CLEAN",
    "L2_REMOTE_L1_CLEAN",
    "L2_REMOTE_L2_CLEAN",
    "L2_REMOTE_L3_CLEAN",
    "L3_REMOTE_L1_CLEAN",
    "L3_REMOTE_L2_CLEAN",
    "L3_REMOTE_L3_CLEAN",
};
const  char* timing_mapping[] = {
    "READ",
    "WRITE",
    "FLUSH"
};
const int threshold = 200;
const int test_cnt = 10000;

/* Variables */
char* probe;
#ifndef MMAP_ARRAY
char __attribute__((aligned(4096))) probe_arr[SIZE_GT_LLC]; // ensure to have an array longer than LLC size
#else
char* probe_arr;
#endif /* ifndef MMAP_ARRAY */

int histogram[NUM_CALIBRE][MAX_CYCLE]={0};
int coarse_histogram[NUM_CALIBRE][MAX_CYCLE/10]={0};

char** target_addresses;
char* start_L1[START_ARR_SIZE];
char* start_L2[START_ARR_SIZE];
char* start_L3[START_ARR_SIZE];
#ifndef NO_L3_CACHE
char* start_LLC = start_L3;
#else
char* start_LLC = start_L2;
#endif /* ifndef NO_L3_CACHE */
char* start[START_ARR_SIZE];
char* conflict_set[NUM_CANDIDATES];
char* lines[NUM_CANDIDATES];

cpu_set_t mycpuset;

int t=0;

int conflict_bar = 0;

long long ave_time_cycle_arr[NUM_CALIBRE] =  {0};

int min_time_cycle_arr[NUM_CALIBRE] =  {0};
int min_time_freq_arr[NUM_CALIBRE] = {0};



// NOTE: x86 version of the benchmark was using inline assembly here,
//  hence, for simplicity, inline assembly was preserved.
// WARNING: however, https://gcc.gnu.org/wiki/DontUseInlineAsm
// TODO: if possible, try to move into generated assembly files

/**
 * Shuffle the given array of given length.
 * It is performed in an undeterministic way, i.e. not reproducible.
 *
 * @param array an array to shuffle.
 * @param n the first `n` elements of the array to shuffle
 *  (should not exceed length).
 * @param length the length of the array.
 */
void shuffle(char **array, int n, int length)
{
    long i, j;
    char* tmp;
    srand(time(NULL));
    for(i = 0; i < n; i++){
        j = rand() % length;
        tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

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
 * Initialize the pointers arrays used for the delay measurement,
 * i.e. the `start` arrays.
 */
void initialize_pointer_arrays()
{
    for(int i=0; i < START_ARR_SIZE; i++)
    {
        // initialize each pointer from start array with
        // the address of the (i*l1_stride)th element from the
        // prob_arr array
        // -> segment bytes into blocks of "l1_stride" by
        //    only tracing these addresses
        start[i]=&probe[i*L1_STRIDE];
    }
    // to use with `target_addresses`
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

/**
 * Create a shuffled eviction set for the Last Level Cache.
 */
void create_shuffled_eviction_set()
{
    // create eviction set for LLC
    for (int i = 0; i < NUM_CANDIDATES; ++i)
    {
        lines[i]=start[i+1];
    }

    // randomize it
    shuffle(lines, NUM_CANDIDATES, NUM_CANDIDATES);
}


/**
 * Probe an array:
 * - first, load `candidate` from memory into register;
 * - second, load each element of `set` from memory into register;
 * - finally, measure timing to load `candidate` from memory again.
 * This probe function was provided by:
 *     F. Liu, Y. Yarom, Q. Ge, G. Heiser and R. B. Lee,
 *     "Last-Level Cache Side-Channel Attacks are Practical,"
 *     2015 IEEE Symposium on Security and Privacy,
 *     San Jose, CA, USA, 2015,
 *     pp. 605-622, doi: 10.1109/SP.2015.43.
 *
 * The goal of this function is to identify if a given set leads
 * to a conflict for a specific candidate.
 *
 * @param set  a set to use to probe the candidate.
 * @param size  the number of elements to probe from the set.
 * @param candidate  the address which should be evicted.
 *
 * @return The probing result: true if a cache miss occurs, false otherwise.
 */
int probe_array(char **set, int size, char *candidate)
{
    // read candidate
    mread(candidate);

    // read l
    for (int i = 0; i < size; ++i)
    {
        mread(set[i]);
    }

    // measure time to read candidate
    t = mread_timing(candidate);

    return (t > threshold );
}

/**
 * Build the conflict set, i.e., a union of eviction sets.
 * It is using the algorithm, without partitioning part, provided by:
 *     F. Liu, Y. Yarom, Q. Ge, G. Heiser and R. B. Lee,
 *     "Last-Level Cache Side-Channel Attacks are Practical,"
 *     2015 IEEE Symposium on Security and Privacy,
 *     San Jose, CA, USA, 2015,
 *     pp. 605-622, doi: 10.1109/SP.2015.43.
 * However, no pruning is performed on the eviction set.
 */
void build_conflict_set()
{
    for (int candidate = 0; candidate < NUM_CANDIDATES; ++candidate)
    {
        // For each candidate from `lines`, probe the candidate:
        // - conflict found if probing reveals a cache miss;
        // - otherwise, no conflict yet.
        // We only enter the if-statement if the cache has hit,
        // i.e. we need at least one more elements to have
        // conflicts.
        // It is necessary to not stop the loop in case
        // there are multiple slices.
        if(!(probe_array(conflict_set, conflict_bar, lines[candidate])))
        {
            // add evicting candidate to the conflict set
            conflict_set[conflict_bar++] = lines[candidate];
        }
    }
    //printf("conflict_bar=%d\n", conflict_bar);
    for (int i = 0; i < conflict_bar; ++i)
    {
        //printf("conflict_set[%d]=%p\n", i, conflict_set[i]);
    }
}


unsigned long test_delay(char* start[], int sec, int mea_type)
{
    unsigned long t=0;

    // initially load start[0] in local L1 for all hybrid cases
    if (sec == L1_REMOTE_L1_HIT_CLEAN
            || sec == L1_REMOTE_L2_HIT_CLEAN
            || sec == L1_REMOTE_L3_HIT_CLEAN
        || sec == L2_REMOTE_L1_HIT_CLEAN
            || sec == L2_REMOTE_L2_HIT_CLEAN
            || sec == L2_REMOTE_L3_HIT_CLEAN
        || sec == L3_REMOTE_L1_HIT_CLEAN
            || sec == L3_REMOTE_L2_HIT_CLEAN
            || sec == L3_REMOTE_L3_HIT_CLEAN)
    {
        mread(start[0]);
    }

    // ensure start[0] is in L2 but not in L1 (local)
    if (sec == L2_REMOTE_L1_HIT_CLEAN
            || sec == L2_REMOTE_L2_HIT_CLEAN
            || sec == L2_REMOTE_L3_HIT_CLEAN)
    {
        evict(start_L1, L1_C, L1_D, L1_L, L1_S);
    }


    // ensure start[0] is in L3 but not in L1 nor L2 (local)
    if (sec == L3_REMOTE_L1_HIT_CLEAN
            || sec == L3_REMOTE_L2_HIT_CLEAN
            || sec == L3_REMOTE_L3_HIT_CLEAN)
    {
        evict(start_L2, L2_C, L2_D, L2_L, L2_S);
        //evict_L2(start, conflict_set, conflict_bar);
    }



    /* if access from remote proc., change CPU to CPU 3 */
    if (sec == L1_REMOTE_L1_HIT_CLEAN
            || sec == L1_REMOTE_L2_HIT_CLEAN
            || sec == L1_REMOTE_L3_HIT_CLEAN
        || sec == L2_REMOTE_L1_HIT_CLEAN
            || sec == L2_REMOTE_L2_HIT_CLEAN
            || sec == L2_REMOTE_L3_HIT_CLEAN
        || sec == L3_REMOTE_L1_HIT_CLEAN
            || sec == L3_REMOTE_L2_HIT_CLEAN
            || sec == L3_REMOTE_L3_HIT_CLEAN)
    {
        CPU_ZERO(&mycpuset);
        CPU_SET(3, &mycpuset);
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
        {
            perror("sched_setaffinity");
        }

        // initially load start[0] in remote L1 for all hybrid cases
        mread(start[0]);
    }


    // ensure start[0] is in L2 but not in L1 (remote)
    if (sec == L1_REMOTE_L2_HIT_CLEAN
            || sec == L2_REMOTE_L2_HIT_CLEAN
            || sec == L3_REMOTE_L2_HIT_CLEAN)
    {
        evict(start_L1, L1_C, L1_D, L1_L, L1_S);
    }

    // ensure start[0] is in L3 but not in L1 nor L2 (remote)
    if (sec == L1_REMOTE_L3_HIT_CLEAN
            || sec == L2_REMOTE_L3_HIT_CLEAN
            || sec == L3_REMOTE_L3_HIT_CLEAN)
    {
        evict(start_L2, L2_C, L2_D, L2_L, L2_S);
        //evict_L2(start, conflict_set, conflict_bar);
    }

    /* if access was from remote proc., change CPU back to CPU 1 */
    if (sec == L1_REMOTE_L1_HIT_CLEAN
            || sec == L1_REMOTE_L2_HIT_CLEAN
            || sec == L1_REMOTE_L3_HIT_CLEAN
        || sec == L2_REMOTE_L1_HIT_CLEAN
            || sec == L2_REMOTE_L2_HIT_CLEAN
            || sec == L2_REMOTE_L3_HIT_CLEAN
        || sec == L3_REMOTE_L1_HIT_CLEAN
            || sec == L3_REMOTE_L2_HIT_CLEAN
            || sec == L3_REMOTE_L3_HIT_CLEAN)
    {
        CPU_ZERO(&mycpuset);
        CPU_SET(1, &mycpuset);
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
        {
              perror("sched_setaffinity");
        }
    }



    /* if access from remote proc., change CPU to CPU 3 */
    if (sec == REMOTE_L1_HIT_CLEAN || sec == REMOTE_L1_HIT_DIRTY
            || sec == REMOTE_L2_HIT_CLEAN || sec == REMOTE_L2_HIT_DIRTY
            || sec == REMOTE_L3_HIT_CLEAN || sec == REMOTE_L3_HIT_DIRTY)
    {
        CPU_ZERO(&mycpuset);
        CPU_SET(3, &mycpuset);
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
        {
            perror("sched_setaffinity");
        }
    }

    /* prepare for cache hit by reading data */
    if (sec == L1_HIT_CLEAN
            || sec == L2_HIT_CLEAN
            || sec == L3_HIT_CLEAN
        || sec == REMOTE_L1_HIT_CLEAN
            || sec == REMOTE_L2_HIT_CLEAN
            || sec == REMOTE_L3_HIT_CLEAN
        || sec == DRAM_HIT)
    {
        mread(start[0]);
    }
    /* prepare for cache hit by writing dirty data */
    else if(sec == L1_HIT_DIRTY
                || sec == L2_HIT_DIRTY
                || sec == L3_HIT_DIRTY
            || sec == REMOTE_L1_HIT_DIRTY
                || sec == REMOTE_L2_HIT_DIRTY
                || sec == REMOTE_L3_HIT_DIRTY)
    {
        mwrite_dirty(start[0]);
    }

    /* if access was from remote proc., change CPU back to CPU 1 */
    if (sec == REMOTE_L1_HIT_CLEAN || sec == REMOTE_L1_HIT_DIRTY
            || sec == REMOTE_L2_HIT_CLEAN || sec == REMOTE_L2_HIT_DIRTY
            || sec == REMOTE_L3_HIT_CLEAN || sec == REMOTE_L3_HIT_DIRTY)
    {
        CPU_ZERO(&mycpuset);
        CPU_SET(1, &mycpuset);
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
        {
            perror("sched_setaffinity");
        }
    }


    // possible eviction to certain level of cache
    if (sec == L2_HIT_CLEAN || sec == L2_HIT_DIRTY
            || sec == REMOTE_L2_HIT_CLEAN || sec == REMOTE_L2_HIT_DIRTY)
    {
        /* if access from remote proc., change CPU to CPU 3 */
        if (sec == REMOTE_L2_HIT_CLEAN || sec == REMOTE_L2_HIT_DIRTY)
        {
            CPU_ZERO(&mycpuset); 
            CPU_SET(3, &mycpuset);
            if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
            {
                perror("sched_setaffinity");
            }
        }


        // ensure start[0] is in L2 but not in L1 (local or remote)
        evict(start_L1, L1_C, L1_D, L1_L, L1_S);

        /* if access was from remote proc., change CPU back to CPU 1 */
        if (sec == REMOTE_L2_HIT_CLEAN || sec == REMOTE_L2_HIT_DIRTY)
        {
            CPU_ZERO(&mycpuset);
            CPU_SET(1, &mycpuset);
            if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
            {
                perror("sched_setaffinity");
            }
        }

    }
    else if(sec == L3_HIT_CLEAN || sec == L3_HIT_DIRTY
                || sec == REMOTE_L3_HIT_CLEAN || sec == REMOTE_L3_HIT_DIRTY)
    {
        /* if access from remote proc., change CPU to CPU 3 */
        if (sec == REMOTE_L3_HIT_CLEAN || sec == REMOTE_L3_HIT_DIRTY)
        {
            CPU_ZERO(&mycpuset);
            CPU_SET(3, &mycpuset);
            if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
            {
                  perror("sched_setaffinity");
            }
        }

        // ensure start[0] is in L3 but not in L1 nor L2
        // (local or remote)
        evict(start_L2, L2_C, L2_D, L2_L, L2_S);
        //evict_L2(start, conflict_set, conflict_bar);

        /* if access was from remote proc., change CPU back to CPU 1 */
        if (sec == REMOTE_L3_HIT_CLEAN || sec == REMOTE_L3_HIT_DIRTY)
        {
            CPU_ZERO(&mycpuset);
            CPU_SET(1, &mycpuset);
            if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
            {
                  perror("sched_setaffinity");
            }
        }
    }
    else if (sec == DRAM_HIT)
    {
        // require the Zicbom extension or a processor specific instr.
#ifndef NO_FLUSH
        mflush(start[0]);
#else
        evict(start_LLC, LLC_C, LLC_D, LLC_L, LLC_S);
#endif /* ifndef NO_FLUSH */
    }


    //measure latency
    // - for read
    if (mea_type == READ)
    {
        t = mread_timing(start[0]);
    }
    // - for write
    else if (mea_type == WRITE)
    {
        t = mwrite_timing(start[0]);
    }
    // - for flush
    else if (mea_type == FLUSH)
    {
        t = mflush_timing(start[0]);
    }

    return t;
}


/**
 * Calibrate the test for a given memory operation and all possible
 * scenarii, and add results to the different histograms.
 *
 * @param counter_hist current index in histogram arrays;
 * @param operation type of memory operation.
 *
 * @return new histogram index.
 */
int calibrate(int counter_hist, int operation)
{
    unsigned int t;

    for (int ord_calibre = 0; ord_calibre < NUM_CALIBRE_SINGLE; ++ord_calibre)
    {
        printf("Generating histogram for timing type %d ...\n", counter_hist+1);

        // Ignore some scenarii if no L3 cache exist.
#ifdef NO_L3_CACHE
        if (ord_calibre == L3_HIT_CLEAN
                || ord_calibre == REMOTE_L3_HIT_CLEAN
                || ord_calibre == L3_HIT_DIRTY
                || ord_calibre == REMOTE_L3_HIT_DIRTY
                || ord_calibre == L1_REMOTE_L3_HIT_CLEAN
                || ord_calibre == L2_REMOTE_L3_HIT_CLEAN
                || ord_calibre == L3_REMOTE_L1_HIT_CLEAN
                || ord_calibre == L3_REMOTE_L2_HIT_CLEAN
                || ord_calibre == L3_REMOTE_L3_HIT_CLEAN)
        {
            ave_time_cycle_arr[counter_hist] = 0;
            counter_hist++;
            continue;
        }
#endif /* ifdef NO_L3_CACHE */

        // Add dummy computation to make sure the data load
        // into L1/L2 from memory
        for(int j=0;j <100;j++)
        {
            t=test_delay(start, ord_calibre, operation);
            for (int i = 0; i < 1000000; ++i)
            {
                t+=i;
            }
        }
        for(int i=0;i<test_cnt;i++)
        {
            t=test_delay(start, ord_calibre, operation);
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
    return counter_hist;
}

void print_to_files(FILE *fp, FILE* fp_coarse)
{
    // header
    // - CSV delimiter
    //fprintf(fp, "sep=,\n");
    //fprintf(fp_coarse, "sep=,\n");
    // - operation
    int timing_type;
    fprintf(fp, ",");
    fprintf(fp_coarse, ",");
    //fprintf(fp, "Timing type,");
    //fprintf(fp_coarse, "Timing type,");
    //fprintf(fp, "Operation,");
    //fprintf(fp_coarse, "Operation,");
    for (int j = 0; j < NUM_CALIBRE; ++j)
    {
        timing_type = j / NUM_CALIBRE_SINGLE;
        fprintf(fp, "%s,", timing_mapping[timing_type]);
        fprintf(fp_coarse, "%s,", timing_mapping[timing_type]);
    }
    fprintf(fp, "\n");
    fprintf(fp_coarse, "\n");
    // - calibre
    int calibre_type;
    fprintf(fp, "%s,", cycles_str);
    fprintf(fp_coarse, "%s,", cycles_str);
    for (int j = 0; j < NUM_CALIBRE; ++j)
    {
        calibre_type = j % NUM_CALIBRE_SINGLE;
        fprintf(fp, "%s,", calibre_mapping[calibre_type]);
        fprintf(fp_coarse, "%s,", calibre_mapping[calibre_type]);
    }
    fprintf(fp, "\n");
    fprintf(fp_coarse, "\n");

    // histogram
    for(int i=0;i<MAX_CYCLE;i++)
    {
        fprintf(fp, "%d,", i);
        for (int j = 0; j < NUM_CALIBRE; ++j)
        {
            fprintf(fp, "%d,", histogram[j][i]);
        }
        fprintf(fp, "\n");

        for (int j = 0; j < NUM_CALIBRE; ++j)
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
        for (int j = 0; j < NUM_CALIBRE; ++j)
        {
            fprintf(fp_coarse, "%d,", coarse_histogram[j][i]);
        }
        fprintf(fp_coarse, "\n");
    }
    for (int j = 0; j < NUM_CALIBRE; ++j)
    {
        ave_time_cycle_arr[j] = ave_time_cycle_arr[j]/test_cnt;
    }

    fclose(fp);
    fclose(fp_coarse);
}

void print_cli_summary(char* file_info)
{
    printf("The maximum frequency cycle number of %d types of timings are given below \n", NUM_CALIBRE);
    for (int j = 0; j < NUM_CALIBRE; ++j)
    {
        printf("%d,", min_time_cycle_arr[j]);
    }
    printf("\n");

    printf("The average cycle number of %d types of timings are given below \n", NUM_CALIBRE);
    for (int j = 0; j < NUM_CALIBRE; ++j)
    {
        printf("%d,", ave_time_cycle_arr[j]);
    }
    printf("\n");

    printf("Histogram numbers are output to histogram_output/histogram_%s.csv and histogram_output/coarse_histogram_%s.csv\n", file_info, file_info);
}


int main(int argc, char *argv[])
{
#ifdef MMAP_ARRAY
    probe_arr = mmap(0, SIZE_GT_LLC*sizeof(char), PROT_READ|PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_2MB, -1, 0);
    if (probe_arr == MAP_FAILED)
    {
        perror("mmap failed for probe_arr\n");
        exit(EXIT_FAILURE);
    }
#endif /* ifdef MMAP_ARRAY */

    // get our CPU: set current CPU to CPU G
    CPU_ZERO(&mycpuset);
    CPU_SET(1, &mycpuset);
    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
    {
        perror("sched_setaffinity");
    }

    char* file_long[50];
    sprintf(file_long, "histogram_output/histogram_%s.csv", argv[1]);

    char* file_name[50];
    sprintf(file_name, "histogram_output/coarse_histogram_%s.csv", argv[1]);
    char* file_info = argv[1];


    FILE *fp = fopen(file_long, "w");
    FILE *fp_coarse = fopen(file_name, "w");


    initialize_probe_array();
    initialize_pointer_arrays();
    create_shuffled_eviction_set();


    /*
     * NOTE: used only when relying on conflict sets for eviction
     * -> should be adapted if needed
     */
    //build_conflict_set();


    //--- CALIBRATION ---
    // calibration is performed for each type of operation: READ, WRITE, FLUSH
    int counter_hist=0;
    // READ
    counter_hist = calibrate(counter_hist, READ);
    // WRITE
    counter_hist = calibrate(counter_hist, WRITE);
#ifndef NO_FLUSH
    // FLUSH
    counter_hist = calibrate(counter_hist, FLUSH);
#endif /* ifndef NO_FLUSH */


    //--- PRINT RESULTS ---
    // to files
    print_to_files(fp, fp_coarse);
    // to cli
    print_cli_summary(file_info);

#ifdef MMAP_ARRAY
    if (munmap(probe_arr, SIZE_GT_LLC*sizeof(char)) == -1) {
        perror("munmap failed for probe_arr\n");
        exit(EXIT_FAILURE);
    }
#endif /* ifdef MMAP_ARRAY */

    return 0;
}
