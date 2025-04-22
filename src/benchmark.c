/*
* This file generates benchmark binary based on provided compilation flags
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

#include "benchmark.h"

#define TOTAL_ADDRESS_VALUES 7
#define MUTEX_BOUND 10000
#define RANDOM_NUM 1000
#define WT_PROB 0.0005

#define LAST_STEP       0
#if VUL3 == U_INV || VUL3 == U
#define U_LAST_STEP 1
#else
#define U_LAST_STEP 0
#endif

#define T_SIZE      4*L1_ASSOC
#define T_R_SIZE    L1_ASSOC

typedef int bool;
enum { false, true };

const int line_size=CACHE_LINE;
const int stride_size=L1_STRIDE;

char* file_name;
FILE *fp;
FILE *fp_res;

size_t t[T_SIZE];
size_t t_r[T_R_SIZE];

int histogram[PROBE_SIZE][MAX_CYCLE]={0};

uint64_t sum[PROBE_SIZE] = {0};
uint64_t counter[PROBE_SIZE] = {0};

double arr1[EACH_RUN*NUM_TEST_HALF]={0};
double arr2[EACH_RUN*NUM_TEST_HALF]={0};
double arr3[EACH_RUN*NUM_TEST_HALF]={0};
double arr4[EACH_RUN*NUM_TEST_HALF]={0};
double arr5[EACH_RUN*NUM_TEST_HALF]={0};
double arr6[EACH_RUN*NUM_TEST_HALF]={0};

int rand_chosen=RANDOM_NUM;

char* chain_arr;
//char* start[L1_ASSOC*CACHE_LINE*L1_CACHE_SETS];
char* start[10*L1_S];

int* maintain_arr;
int status[4];

int tar_block;
int untar_block;


void initialize_arrays()
{
    // Map space for shared array
    chain_arr = mmap(0, MAX_ARRAY_SIZE*sizeof(char), PROT_READ|PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (chain_arr == MAP_FAILED)
    {
        perror("mmap failed for chain_arr\n");
        exit(EXIT_FAILURE);
    }
    memset(chain_arr, 0, MAX_ARRAY_SIZE*sizeof(char));

    //  initiate maintain_arr
    maintain_arr = mmap(0, 1*sizeof(int), PROT_READ|PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (maintain_arr == MAP_FAILED)
    {
        perror("mmap failed for maintain_arr\n");
        exit(EXIT_FAILURE);
    }

    for(int i =0 ; i< MAX_ARRAY_SIZE; i++) /*probe_arr*/
        chain_arr[i]=/*(char)*/ MAX_ARRAY_SIZE-i; //copy on write
}

/**
 * Unmap the memory mappings.
 */
void free_mappings()
{
    if (munmap(chain_arr, MAX_ARRAY_SIZE*sizeof(char)) == -1) {
        perror("munmap failed for chain_arr\n");
        exit(EXIT_FAILURE);
    }

    if (munmap(maintain_arr, 1*sizeof(int)) == -1) {
        perror("munmap failed for maintain_arr\n");
        exit(EXIT_FAILURE);
    }
}

// NOTE: adapt if necessary
void build_eviction_set()
{
    // initialize eviction set
    for(int i=0;i< L1_TOTAL_WAYS/4;i++)
    {
        start[i]=&chain_arr[ i*stride_size ];
    }

    // load into L1 L2
    for(int i=0;i<L1_ASSOC*8;i++)
    {
        mread_mfenced(start[i]);
    }

    for(int i=0;i<L1_ASSOC*8;i++)
    {
        mread_mfenced(start[i]+untar_block+NIB_OFFSET);
    }

    // leave time to load data
    int tmp;
    for (int i = 0; i < 1000000; ++i)
    {
        tmp+=i; // no utility, except leaving time for loading
    }
}

void step0(int i, int j, int m)
{
    if(maintain_arr[0]==NOONE_RUN)
    {
        maintain_arr[0]=STEP0_RUN;
    }
    while(maintain_arr[0]!=STEP0_RUN)
        sched_yield();

    tar_block = (rand()%4)*8;
    untar_block = (rand()%4)*8;

    //if (j==1 && i==299 && m==30) printf("sched_getcpu_in_step_1 = %d\n", sched_getcpu());

    maintain_arr[0]=STEP1_RUN;
}


/**
 * First step from the three-step model.
 * The vulnerability type is declared before the execution of a
 * generic code part.
 */
void step1(int i, int j, int m)
{
    while(maintain_arr[0]!=STEP1_RUN)
        sched_yield();

#define VUL_TYPE    VUL1

    /* GENERIC CODE - START */
#define IF_UNKNOWN 0

#if VUL_TYPE == U || VUL_TYPE == U_INV
#undef IF_UNKNOWN
#define IF_UNKNOWN 1
#endif /* if VUL_TYPE */

#if VUL_TYPE == A || VUL_TYPE == A_ALIAS || VUL_TYPE == D || VUL_TYPE == U
    ACCESS(IF_WRITE_1, IF_UNKNOWN, LAST_STEP, U_LAST_STEP, VUL_TYPE)

#elif VUL_TYPE==INV || VUL_TYPE==A_INV || VUL_TYPE==D_INV || VUL_TYPE==A_ALIAS_INV || VUL_TYPE==U_INV
    FLUSH(IF_WRITE_1, IF_UNKNOWN, LAST_STEP, U_LAST_STEP, VUL_TYPE)

#elif VUL_TYPE==PRE
    PRE_STEP(LAST_STEP)

#elif VUL_TYPE==STAR
    STAR_STEP(LAST_STEP)

#else
    #error "Error: Wrong input in step 1\n"
#endif /* if VUL_TYPE */
#undef IF_UNKNOWN

    /* GENERIC CODE - END */
#undef VUL_TYPE

    //if (j==1 && i==299 && m==30) printf("sched_getcpu_in_step_1 = %d\n", sched_getcpu());

    maintain_arr[0]=STEP2_RUN;
}


/**
 * Second step from the three-step model.
 * The vulnerability type is declared before the execution of a
 * generic code part.
 */
void step2(int i, int j, int m)
{
    while(maintain_arr[0]!=STEP2_RUN)
        sched_yield();

#if IF_PLAIN
        //
#else
#define VUL_TYPE    VUL2

        /* GENERIC CODE - START */
#define IF_UNKNOWN 0

#if VUL_TYPE == U || VUL_TYPE == U_INV
#undef IF_UNKNOWN
#define IF_UNKNOWN 1
#endif /* if VUL_TYPE */

#if VUL_TYPE == A || VUL_TYPE == A_ALIAS || VUL_TYPE == D || VUL_TYPE == U
        ACCESS(IF_WRITE_2, IF_UNKNOWN, LAST_STEP, U_LAST_STEP, VUL_TYPE)

#elif VUL_TYPE==INV || VUL_TYPE==A_INV || VUL_TYPE==D_INV || VUL_TYPE==A_ALIAS_INV || VUL_TYPE==U_INV
        FLUSH(IF_WRITE_2, IF_UNKNOWN, LAST_STEP, U_LAST_STEP, VUL_TYPE)

#elif VUL_TYPE==PRE
        PRE_STEP(LAST_STEP)

#elif VUL_TYPE==STAR
        STAR_STEP(LAST_STEP)

#else
        #error "Error: Wrong input in step 2\n"
#endif /* if VUL_TYPE */
#undef IF_UNKNOWN

        /* GENERIC CODE - END */
#undef VUL_TYPE

#endif /* if IF_PLAIN */

    //if (j==1 && i==299 && m==30) printf("sched_getcpu_in_step_2 = %d\n", sched_getcpu());

    maintain_arr[0] = STEP3_RUN;
}


/**
 * Third and last step from the three-step model.
 * The vulnerability type is declared before the execution of a
 * generic code part. Then, measured timings are processed.
 */
void step3(int i, int j, int m, int round_rand)
{
    while(maintain_arr[0]!=STEP3_RUN)
        sched_yield();

#define VUL_TYPE    VUL3
#undef  LAST_STEP
#define LAST_STEP   1

    /* GENERIC CODE - START */
#define IF_UNKNOWN 0

#if VUL_TYPE == U || VUL_TYPE == U_INV
#undef IF_UNKNOWN
#define IF_UNKNOWN 1
#endif /* if VUL_TYPE */

#if VUL_TYPE == A || VUL_TYPE == A_ALIAS || VUL_TYPE == D || VUL_TYPE == U
    ACCESS(IF_WRITE_3, IF_UNKNOWN, LAST_STEP, U_LAST_STEP, VUL_TYPE)

#elif VUL_TYPE==INV || VUL_TYPE==A_INV || VUL_TYPE==D_INV || VUL_TYPE==A_ALIAS_INV || VUL_TYPE==U_INV
    FLUSH(IF_WRITE_3, IF_UNKNOWN, LAST_STEP, U_LAST_STEP, VUL_TYPE)

#elif VUL_TYPE==PRE
    PRE_STEP(LAST_STEP)

#elif VUL_TYPE==STAR
    STAR_STEP(LAST_STEP)

#else
    #error "Error: Wrong input in step 3\n"
#endif /* if VUL_TYPE */
#undef IF_UNKNOWN

    /* GENERIC CODE - END */
#undef LAST_STEP
#undef VUL_TYPE

    // process timings
    size_t t_sum = 0;
    for (int t_cnt = 0; t_cnt<T_SIZE; t_cnt++)
        t_sum += t[t_cnt];

    if (t_sum < 30000 && i>NUM_TEST_HALF*0.3)
    {
        sum[j]+=t_sum;//delta;
        counter[j]+=1;
    }

    if(t_sum<MAX_CYCLE)
        histogram[j][t_sum]++;
    else
        histogram[j][MAX_CYCLE-1]++;

    if(j==0)
        arr1[m*NUM_TEST_HALF+i]+=(double)(t_sum);
    else if(j==1)
        arr2[m*NUM_TEST_HALF+i]+=(double)(t_sum);
    else if(j==2)
        arr3[m*NUM_TEST_HALF+i]+=(double)(t_sum);

    size_t t_r_sum = 0;
    for (int t_r_cnt = 0; t_r_cnt<T_R_SIZE; t_r_cnt++)
        t_r_sum += t_r[t_r_cnt];

    if(round_rand==0)
    {
        if(j==0)
            arr4[m*NUM_TEST_HALF+i]+=(double)(t_r_sum);
        else if(j==1)
            arr5[m*NUM_TEST_HALF+i]+=(double)(t_r_sum);
        else if(j==2)
            arr6[m*NUM_TEST_HALF+i]+=(double)(t_r_sum);
    }
    else
    {
        if(j==0)
            arr4[m*NUM_TEST_HALF+i]+=(double)(t_r_sum-arr1[m*NUM_TEST_HALF+i]);
        else if(j==1)
            arr5[m*NUM_TEST_HALF+i]+=(double)(t_r_sum-arr2[m*NUM_TEST_HALF+i]);
        else if(j==2)
            arr6[m*NUM_TEST_HALF+i]+=(double)(t_r_sum-arr3[m*NUM_TEST_HALF+i]);
    }

    //if (j==1 && i==299 && m==30) printf("sched_getcpu_in_step_3 = %d\n", sched_getcpu());

    maintain_arr[0]=STEP0_RUN;
}

void print_m10(int m)
{
#if IF_PLAIN == 0
    if (m==10)
    {
        for(int i=0; i< PROBE_SIZE; i++)
        {
            fprintf(fp, "%lf\t", (double)(sum[i])/counter[i]);
        }
    }
#endif /* if IF_PLAIN == 0 */
}

void timing()
{
    printf("cycles\ta\ta_alias\tNIB\n");
    for(int i=0;i<MAX_CYCLE;i++)
    {
        printf("%d\t%d\t%d\t%d\n", i,
                histogram[0][i], histogram[1][i], histogram[2][i]);
    }

    // start calculate the p value of mapped and not mapped data
    double pvalue_a_a_alias = Pvalue(
            arr1, NUM_TEST_HALF*EACH_RUN,
            arr2, NUM_TEST_HALF*EACH_RUN
    );
    double pvalue_a_NIB = Pvalue(
            arr1, NUM_TEST_HALF*EACH_RUN,
            arr3, NUM_TEST_HALF*EACH_RUN
    );
    double pvalue_a_alias_NIB = Pvalue(
            arr2, NUM_TEST_HALF*EACH_RUN,
            arr3, NUM_TEST_HALF*EACH_RUN
    );

    double pvalue_a_a_alias_2 = Pvalue(
            arr4, NUM_TEST_HALF*EACH_RUN,
            arr5, NUM_TEST_HALF*EACH_RUN
    );
    double pvalue_a_NIB_2 = Pvalue(
            arr4, NUM_TEST_HALF*EACH_RUN,
            arr6, NUM_TEST_HALF*EACH_RUN
    );
    double pvalue_a_alias_NIB_2 = Pvalue(
            arr5, NUM_TEST_HALF*EACH_RUN,
            arr6, NUM_TEST_HALF*EACH_RUN
    );

    fprintf(fp, "\n%d\t%s_%s\t%s_%s\t%s_%s\n", PATTERN,
            type_match[ENTITY1], type_match[VUL1],
            type_match[ENTITY2], type_match[VUL2],
            type_match[ENTITY3], type_match[VUL3]
    );
    fprintf(fp, "\n%f\t%f\t%f\t%f\t%f\t%f\n",
            pvalue_a_a_alias, pvalue_a_NIB, pvalue_a_alias_NIB,
            pvalue_a_a_alias_2, pvalue_a_NIB_2, pvalue_a_alias_NIB_2
    );

    // first, determine whether an attacker can observe different
    // timings depending on the address pointed by Vu
    uint8_t welch_cond1;
    welch_cond1 =
        (pvalue_a_a_alias<WT_PROB && pvalue_a_NIB<WT_PROB)
        ||(pvalue_a_a_alias<WT_PROB && pvalue_a_alias_NIB<WT_PROB)
        ||(pvalue_a_alias_NIB<WT_PROB && pvalue_a_NIB<WT_PROB);
    // then, determine if there already is a timing difference
    // between candidates for cache hits (second access)
    uint8_t welch_cond2;
    welch_cond2 =
        (pvalue_a_a_alias_2<WT_PROB && pvalue_a_NIB_2<WT_PROB)
        ||(pvalue_a_a_alias_2<WT_PROB && pvalue_a_alias_NIB_2<WT_PROB)
        ||(pvalue_a_alias_NIB_2<WT_PROB && pvalue_a_NIB_2<WT_PROB);

    /*
     * If the first condition is satisfied AND
     * (either the last step is not a U step
     *  OR the second condition is satisfied)
     * -> success
     */
    if (welch_cond1 && ((!U_LAST_STEP) || welch_cond2))
    {
#if IF_PLAIN == 0
        fprintf(fp, "\t\t\t\t\t\t\t\t\t\t\t\t\t\t%s Succeed \n", file_name);
        fprintf(fp_res, "1");
#else /* IF_PLAIN == 1 */
        fprintf(fp, "%s Succeeds \n\n", file_name);
        fprintf(fp_res, "1\n");
#endif /* if IF_PLAIN == 0 */
    }
    /*
     * Otherwise -> fail
     */
    else
    {
#if IF_PLAIN == 0
        fprintf(fp, "\t\t\t\t\t\t\t\t\t\t\t\t\t\t%s Fails \n", file_name);
        fprintf(fp_res, "0");
#else /* IF_PLAIN == 1 */
        fprintf(fp, "%s Fails \n\n", file_name);
        fprintf(fp_res, "0\n");
#endif /* if IF_PLAIN == 0 */
    }

    // NOTE: i.e. when plain and the main 4 "for" loops reach
    // their boundary
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    file_name = argv[0];
    fp = fopen(strcat(argv[1],".output"), "a");
    fp_res = fopen(strcat(argv[1],".res"), "a");

#if SKIP_CASE == 1
#if IF_PLAIN == 0
    fprintf(fp, "\t\t\t\t\t\t\t\t\t\t\t\t\t\t%s Unsupported \n", file_name);
    // case that should be skipped (e.g.: no flush support)
    // -> will be considered in IF_PLAIN case
#else /* IF_PLAIN == 1 */
    fprintf(fp, "%s Unsupported \n", file_name);
    // case that should be skipped (e.g.: no flush support)
    fprintf(fp_res, "222\n");
#endif /* if IF_PLAIN == 0 */
#elif AFFINITY_REL==1 && (LOCAL_CPU1 == LOCAL_CPU2 \
                      || REMOTE_CPU1 == REMOTE_CPU2)
#if IF_PLAIN == 0
    fprintf(fp, "\t\t\t\t\t\t\t\t\t\t\t\t\t\t%s Ignored \n", file_name);
    // case is undesired: local or remote CPUs are identical
    // -> will be considered in IF_PLAIN case
#else /* IF_PLAIN == 1 */
    fprintf(fp, "%s Ignored \n", file_name);
    // case is undesired: local or remote CPUs are identical
    fprintf(fp_res, "111\n");
#endif /* if IF_PLAIN == 0 */
#else /* SKIP_CASE == 0 */
    /* go further in the code */

    /* get the number of cpu's */
    cpu_set_t mycpuset;
    int numcpu = sysconf( _SC_NPROCESSORS_ONLN );
    int cpu[5];
    cpu[L_1]=LOCAL_CPU1;
    cpu[L_2]=LOCAL_CPU2;
    cpu[R_1]=REMOTE_CPU1;
    cpu[R_2]=REMOTE_CPU2;
    cpu[REST]=REST_CPU;
    // get our CPU

    CPU_SET(cpu[REST], &mycpuset);
    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
    {
        perror("sched_setaffinity");
    }
    //print_affinity();
    //printf("sched_getcpu_in_main = %d\n", sched_getcpu());

    memset(t, 0, T_SIZE*sizeof(size_t));
    memset(t_r, 0, T_R_SIZE*sizeof(size_t));

    for (int i = 0; i < EACH_RUN*NUM_TEST_HALF; ++i)
    {
        arr1[i] = 0;
        arr2[i] = 0;
        arr3[i] = 0;
        arr4[i] = 0;
        arr5[i] = 0;
        arr6[i] = 0;
    }

    int c;
    pid_t pid_n, pid_l_1, pid_l_2, pid_r_1, pid_r_2;
    uint64_t a, b, d, e;

    initialize_arrays();
    build_eviction_set();

    maintain_arr[0] = NOONE_RUN;

    // local attacker
    if ((pid_l_1 = fork()) == -1)
    {
        printf("Failed to fork process 1\n");
        exit(EXIT_FAILURE);
    }
    else if (pid_l_1 == 0)
    {
        CPU_ZERO(&mycpuset);

#if LOCAL_ATT && LOCAL_VIC
        CPU_SET(cpu[L_1+AFFINITY_REL], &mycpuset);
#else
        CPU_SET(cpu[L_1], &mycpuset);
#endif

        //set processor affinity
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
        {
            perror("sched_setaffinity");
        }

        for (int m = 0; m < EACH_RUN; m++)
        {
#ifdef ATTACKER_L_3
            for (int e = 0; e < PROBE_SIZE; ++e)
            {
                sum[e] = 0;
                counter[e] = 0;
            }
#endif /* ifdef ATTACKER_L_3 */
            //probe = probe + 0x10*j;
            for (int i = 0; i < NUM_TEST_HALF; ++i)
            {
                for (int round_rand = 0; round_rand < 2; ++round_rand)
                {
                    /* try all "u" candidates: a, a_alias, NIB */
                    for (int j = 0; j < PROBE_SIZE; ++j)
                    {
#ifdef ATTACKER_L_3
                        memset(t, 0, T_SIZE*sizeof(size_t));
                        memset(t_r, 0, T_R_SIZE*sizeof(size_t));
#endif /* ifdef ATTACKER_L_3 */
                        step0(i, j, m);
#ifdef ATTACKER_L_1
                        step1(i, j, m);
#endif /* ifdef ATTACKER_L_1 */
#ifdef ATTACKER_L_2
                        step2(i, j, m);
#endif /* ifdef ATTACKER_L_2 */
#ifdef ATTACKER_L_3
                        step3(i, j, m, round_rand);
#endif /* ifdef ATTACKER_L_3 */
                    }
                }
            }
#ifdef ATTACKER_L_3
            print_m10(m);
#endif /* ifdef ATTACKER_L_3 */
        }
#ifdef ATTACKER_L_3
        timing();
#endif /* ifdef ATTACKER_L_3 */
        exit(EXIT_SUCCESS);
    }

    // local victim
    if ((pid_l_2 = fork()) == -1)
    {
        printf("Failed to fork process 2\n");
        exit(EXIT_FAILURE);
    }
    else if (pid_l_2 == 0)
    {
        CPU_ZERO(&mycpuset);
        CPU_SET(cpu[L_2], &mycpuset);
        //set processor affinity
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
        {
           perror("sched_setaffinity");
        }

        for (int m = 0; m < EACH_RUN; m++)
        {
#ifdef VICTIM_L_3
            for (int e = 0; e < PROBE_SIZE; ++e)
            {
                sum[e] = 0;
                counter[e] = 0;
            }
#endif /* ifdef VICTIM_L_3 */
            //probe = probe + 0x10*j;
            for (int i = 0; i < NUM_TEST_HALF; ++i)
            {
                for (int round_rand = 0; round_rand < 2; ++round_rand)
                {
                    /* try all "u" candidates: a, a_alias, NIB */
                    for (int j = 0; j < PROBE_SIZE; ++j)
                    {
#ifdef VICTIM_L_3
                        memset(t, 0, T_SIZE*sizeof(size_t));
                        memset(t_r, 0, T_R_SIZE*sizeof(size_t));
#endif /* ifdef VICTIM_L_3 */
#ifdef VICTIM_L_1
                        step1(i, j, m);
#endif /* ifdef VICTIM_L_1 */
#ifdef VICTIM_L_2
                        step2(i, j, m);
#endif /* ifdef VICTIM_L_2 */
#ifdef VICTIM_L_3
                        step3(i, j, m, round_rand);
#endif /* ifdef VICTIM_L_3 */
                    }
                }
            }
#ifdef VICTIM_L_3
            print_m10(m);
#endif /* ifdef VICTIM_L_3 */
        }
#ifdef VICTIM_L_3
        timing();
#endif /* ifdef VICTIM_L_3 */
        exit(EXIT_SUCCESS);
    }

    // remote attacker
    if ((pid_r_1 = fork()) == -1)
    {
        printf("Failed to fork process 3\n");
        exit(EXIT_FAILURE);
    }
    else if (pid_r_1 == 0)
    {
        CPU_ZERO(&mycpuset);

#if REMOTE_ATT && REMOTE_VIC
        CPU_SET(cpu[R_1+AFFINITY_REL], &mycpuset);
#else
        CPU_SET(cpu[R_1] , &mycpuset);
#endif
        //set processor affinity
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
        {
            perror("sched_setaffinity");
        }

        for (int m = 0; m < EACH_RUN; m++)
        {
#ifdef ATTACKER_R_3
            for (int e = 0; e < PROBE_SIZE; ++e)
            {
                sum[e] = 0;
                counter[e] = 0;
            }
#endif /* ifdef ATTACKER_R_3 */
            //probe = probe + 0x10*j;
            for (int i = 0; i < NUM_TEST_HALF; ++i)
            {
                for (int round_rand = 0; round_rand < 2; ++round_rand)
                {
                    /* try all "u" candidates: a, a_alias, NIB */
                    for (int j = 0; j < PROBE_SIZE; ++j)
                    {
#ifdef ATTACKER_R_3
                        memset(t, 0, T_SIZE*sizeof(size_t));
                        memset(t_r, 0, T_R_SIZE*sizeof(size_t));
#endif /* ifdef ATTACKER_R_3 */
#ifdef ATTACKER_R_1
                        step1(i, j, m);
#endif /* ifdef ATTACKER_R_1 */
#ifdef ATTACKER_R_2
                        step2(i, j, m);
#endif /* ifdef ATTACKER_R_2 */
#ifdef ATTACKER_R_3
                        step3(i, j, m, round_rand);
#endif /* ifdef ATTACKER_R_3 */
                    }
                }
            }
#ifdef ATTACKER_R_3
            print_m10(m);
#endif /* ifdef ATTACKER_R_3 */
        }
#ifdef ATTACKER_R_3
        timing();
#endif /* ifdef ATTACKER_R_3 */
        exit(EXIT_SUCCESS);
    }

    // remote victim
    if ((pid_r_2 = fork()) == -1)
    {
        printf("Failed to fork process 4\n");
        exit(EXIT_FAILURE);
    }
    else if (pid_r_2 == 0)
    {
        CPU_ZERO(&mycpuset);
        CPU_SET(cpu[R_2], &mycpuset);
        //set processor affinity
        if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1)
        {
            perror("sched_setaffinity");
        }

        for (int m = 0; m < EACH_RUN; m++)
        {
#ifdef VICTIM_R_3
            for (int e = 0; e < PROBE_SIZE; ++e)
            {
                sum[e] = 0;
                counter[e] = 0;
            }
#endif /* ifdef VICTIM_R_3 */
            //probe = probe + 0x10*j;
            for (int i = 0; i < NUM_TEST_HALF; ++i)
            {

                for (int round_rand = 0; round_rand < 2; ++round_rand)
                {
                    /* try all "u" candidates: a, a_alias, NIB */
                    for (int j = 0; j < PROBE_SIZE; ++j)
                    {
#ifdef VICTIM_R_3
                        memset(t, 0, T_SIZE*sizeof(size_t));
                        memset(t_r, 0, T_R_SIZE*sizeof(size_t));
#endif /* ifdef VICTIM_R_3 */
#ifdef VICTIM_R_1
                        step1(i, j, m);
#endif /* ifdef VICTIM_R_1 */
#ifdef VICTIM_R_2
                        step2(i, j, m);
#endif /* ifdef VICTIM_R_2 */
#ifdef VICTIM_R_3
                        step3(i, j, m, round_rand);
#endif /* ifdef VICTIM_R_3 */
                    }
                }
            }
#ifdef VICTIM_R_3
            print_m10(m);
#endif /* ifdef VICTIM_R_3 */
        }
#ifdef VICTIM_R_3
        timing();
#endif /* ifdef VICTIM_R_3 */
        exit(EXIT_SUCCESS);
    }

    while (wait(NULL)>0);
    free_mappings();

#endif /* SKIP_CASE == 1 */

    // NOTE: i.e. when plain and the main 4 "for" loops reach
    // their boundary
#if IF_PLAIN && IF_WRITE_1 && IF_WRITE_2 && IF_WRITE_3 && \
    AFFINITY_REL
    fprintf(fp_res, "\n");
#endif

    fclose(fp);
    fclose(fp_res);
    return EXIT_SUCCESS;
}
