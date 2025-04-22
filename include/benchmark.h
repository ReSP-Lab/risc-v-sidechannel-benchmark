/*
* This file is one of the headers file for generating benchmark binaries
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

#ifndef __BENCHMARK_H
#define __BENCHMARK_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>

#include "board.h"
#include "asm_functions.h"
#include "evict.h"
#include "stats.h"
#include "vulnerabilities.h"

#define MAX_ARRAY_SIZE  L1_CACHE_SETS*L1_ASSOC*CACHE_LINE*64

//#ifndef PROBE_SIZE
//    #error "PROBE_SIZE undefined. Use '-DPROBE_SIZE=...'."
//#endif /* ifndef PROBE_SIZE */
#define PROBE_SIZE  3 // 3 different candidates of "u": "a", "a_alias", "NIB"
#ifndef EACH_RUN
    #error "EACH_RUN undefined. Use '-DEACH_RUN=...'."
#endif /* ifndef EACH_RUN */
#ifndef MAX_CYCLE
    #error "MAX_CYCLE undefined. Use '-DMAX_CYCLE=...'."
#endif /* ifndef MAX_CYCLE */
#ifndef NUM_TEST
    #error "NUM_TEST undefined. Use '-DNUM_TEST=...'."
#endif /* ifndef NUM_TEST */
#define NUM_TEST_HALF NUM_TEST/2


/* Ensure an attacker or victim was defined for each step. */
#if !defined(ATTACKER_L_1) && !defined(VICTIM_L_1) \
    && !defined(ATTACKER_R_1) && !defined(VICTIM_R_1)
    #error "Attacker/victim from local/remote undefined for step 1."
#endif
#if !defined(ATTACKER_L_2) && !defined(VICTIM_L_2) \
    && !defined(ATTACKER_R_2) && !defined(VICTIM_R_2)
    #error "Attacker/victim from local/remote undefined for step 2."
#endif
#if !defined(ATTACKER_L_3) && !defined(VICTIM_L_3) \
    && !defined(ATTACKER_R_3) && !defined(VICTIM_R_3)
    #error "Attacker/victim from local/remote undefined for step 3."
#endif

#if defined(ATTACKER_L_1) || defined(ATTACKER_L_2) || defined(ATTACKER_L_3)
#define LOCAL_ATT   1
#endif
#if defined(VICTIM_L_1) || defined(VICTIM_L_2) || defined(VICTIM_L_3)
#define LOCAL_VIC   1
#endif
#if defined(ATTACKER_R_1) || defined(ATTACKER_R_2) || defined(ATTACKER_R_3)
#define REMOTE_ATT  1
#endif
#if defined(VICTIM_R_1) || defined(VICTIM_R_2) || defined(VICTIM_R_3)
#define REMOTE_VIC  1
#endif

#ifndef VUL1
    #error "VUL1 undefined. Use '-DVUL1=...'."
#endif // !VUL1
#ifndef VUL2
    #error "VUL2 undefined. Use '-DVUL2=...'."
#endif // !VUL2
#ifndef VUL3
    #error "VUL3 undefined. Use '-DVUL3=...'."
#endif // !VUL3

#ifndef IF_PLAIN
    #error "IF_PLAIN undefined. Use '-DIF_PLAIN=...'"
#endif // !IF_PLAIN

#ifndef IF_WRITE_1
    #error "IF_WRITE_1 undefined. Use '-DIF_WRITE_1=...'"
#endif // !IF_WRITE_1
#ifndef IF_WRITE_2
    #error "IF_WRITE_2 undefined. Use '-DIF_WRITE_2=...'"
#endif // !IF_WRITE_2
#ifndef IF_WRITE_3
    #error "IF_WRITE_3 undefined. Use '-DIF_WRITE_3=...'"
#endif // !IF_WRITE_3

#ifndef AFFINITY_REL
    #error "AFFINITY_REL undefined. Use '-DAFFINITY_REL=...'."
#endif /* ifndef NUM_TEST */

#define NOONE_RUN 0
#define STEP0_RUN 1
#define STEP1_RUN 2
#define STEP2_RUN 3
#define STEP3_RUN 4

#define ATTACKER 0
#define VICTIM 1
#define A 2
#define A_ALIAS 3
#define D 4
#define U 5
#define INV 6
#define A_INV 7
#define D_INV 8
#define U_INV 9
#define A_ALIAS_INV 10
#define MISS 11
#define HIT 12
#define STAR 13
#define PRE 14

#define PROBE_A         0
#define PROBE_A_ALIAS   1
#define PROBE_NIB       2

#define L_1     0
#define L_2     1
#define R_1     2
#define R_2     3
#define REST    4

#define NIB_OFFSET 512


#define UNROLL_MOP(moperation, array, offset, block_offset, byte_offset) \
    if (&moperation##_mfenced != &mflush_mfenced) \
    { \
        for (int k = 0; k < L1_ASSOC; k++) \
        { \
            evict_offset(array, offset, block_offset+byte_offset, \
                    L1_C, L1_D, L1_L, L1_S, &moperation##_mfenced); \
        } \
    } \
    else \
    { \
        for (int k = 0; k < L1_ASSOC; k++) \
        { \
            mflush_mfenced(array[offset+k]+block_offset+byte_offset); \
        } \
    }

#define UNROLL_TIMING_MOP(moperation, array, offset, block_offset, byte_offset, t_array, t_offset) \
    for (int k = 0; k < L1_ASSOC; k++) \
    { \
        t_array[t_offset+k] = moperation##_timing(array[offset+k]+block_offset+byte_offset); \
    }


/*
 * ACCESS macro used to adapt functions at compilation time.
 *
 * @param write  is this a write access?
 * @param unknown  is the target address unknown by the attacker?
 * @param last  is this access from the last step?
 * @param u  is the last step related to the unknown address?
 * @param vul_type  vulnerability type
 */
#define  __ACCESS(write, unknown, last, u, vul_type) \
    ACCESS_UNKNOWN##unknown##_WRITE##write##_LAST##last##_U##u(vul_type)

#define ACCESS(write, unknown, last, u, vul_type) \
    __ACCESS(write, unknown, last, u, vul_type)


#define ACCESS_UNKNOWN1_LAST0_U0(moperation) \
    int var_num = 0; \
    if (j==PROBE_A) \
    { /* a */ \
        var_num=0; \
        UNROLL_MOP(moperation, start, var_num, tar_block, 0) \
    } \
    else if (j==PROBE_A_ALIAS) \
    { /* a_alias */ \
        var_num=L1_S; \
        UNROLL_MOP(moperation, start, var_num, tar_block, 0) \
    } \
    else if (j==PROBE_NIB) \
    { /* NIB */ \
        var_num=0; \
        UNROLL_MOP(moperation, start, var_num, untar_block, NIB_OFFSET) \
    } \
    else if (j==3) \
    { \
        int dummy_print=1; \
    }

#define ACCESS_UNKNOWN1_WRITE0_LAST0_U0(vul_type) \
    ACCESS_UNKNOWN1_LAST0_U0(mread)

#define ACCESS_UNKNOWN1_WRITE1_LAST0_U0(vul_type) \
    ACCESS_UNKNOWN1_LAST0_U0(mwrite)

#define ACCESS_UNKNOWN1_WRITE0_LAST0_U1(vul_type) \
    ACCESS_UNKNOWN1_LAST0_U0(mread)

#define ACCESS_UNKNOWN1_WRITE1_LAST0_U1(vul_type) \
    ACCESS_UNKNOWN1_LAST0_U0(mwrite)


#define ACCESS_UNKNOWN1_LAST1_U0(moperation) \
    int var_num = 0; \
    if (j==PROBE_A) \
    { /* a */ \
        var_num=0; \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
    } \
    else if (j==PROBE_A_ALIAS) \
    { /* a_alias */ \
        var_num=L1_S; \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
    } \
    else if (j==PROBE_NIB) \
    { /* NIB */ \
        var_num=0; \
        UNROLL_TIMING_MOP(moperation, start, var_num, untar_block, NIB_OFFSET, t, 0) \
    } \
    else if (j==3) \
    { \
        int dummy_print=1; \
    }

#define ACCESS_UNKNOWN1_WRITE0_LAST1_U0(vul_type) \
    ACCESS_UNKNOWN1_LAST1_U0(mread)

#define ACCESS_UNKNOWN1_WRITE1_LAST1_U0(vul_type) \
    ACCESS_UNKNOWN1_LAST1_U0(mwrite)


#define ACCESS_UNKNOWN1_LAST1_U1(moperation) \
    int var_num = 0; \
    if (j==PROBE_A) \
    { /* a */ \
        var_num=0; \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t_r, 0) \
    } \
    else if (j==PROBE_A_ALIAS) \
    { /* a_alias */ \
        var_num=L1_S; \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t_r, 0) \
    } \
    else if (j==PROBE_NIB) \
    { /* NIB */ \
        var_num=0; \
        UNROLL_TIMING_MOP(moperation, start, var_num, untar_block, NIB_OFFSET, t, 0) \
        UNROLL_TIMING_MOP(moperation, start, var_num, untar_block, NIB_OFFSET, t_r, 0) \
    } \
    else if (j==3) \
    { \
        int dummy_print=1; \
    }

#define ACCESS_UNKNOWN1_WRITE0_LAST1_U1(vul_type) \
    ACCESS_UNKNOWN1_LAST1_U1(mread)

#define ACCESS_UNKNOWN1_WRITE1_LAST1_U1(vul_type) \
    ACCESS_UNKNOWN1_LAST1_U1(mwrite)


#define ACCESS_UNKNOWN0_LAST0_U0(moperation, vul_type) \
    int var_num=RANDOM_NUM; \
    int vul = vul_type; \
    switch (vul) \
    { \
        case A: \
            var_num = 0; \
            break; \
        case D: \
            var_num = 2*L1_S; \
            break; \
        case A_ALIAS: \
            var_num = L1_S; \
            break; \
        default: \
            printf("Error! VUL_TYPE is not correct for access;1\n"); \
            exit(1); \
    } \
    UNROLL_MOP(moperation, start, var_num, tar_block, 0)

#define ACCESS_UNKNOWN0_WRITE0_LAST0_U0(vul_type) \
    ACCESS_UNKNOWN0_LAST0_U0(mread, vul_type)

#define ACCESS_UNKNOWN0_WRITE0_LAST0_U1(vul_type) \
    ACCESS_UNKNOWN0_WRITE0_LAST0_U0(vul_type)

#define ACCESS_UNKNOWN0_WRITE1_LAST0_U0(vul_type) \
    ACCESS_UNKNOWN0_LAST0_U0(mwrite, vul_type)

#define ACCESS_UNKNOWN0_WRITE1_LAST0_U1(vul_type) \
    ACCESS_UNKNOWN0_WRITE1_LAST0_U0(vul_type)


#define ACCESS_UNKNOWN0_LAST1_U0(moperation, vul_type) \
    int var_num=RANDOM_NUM; \
    int vul = vul_type; \
    switch (vul) \
    { \
        case A: \
            var_num = 0; \
            break; \
        case D: \
            var_num = 2*L1_S; \
            break; \
        case A_ALIAS: \
            var_num = L1_S; \
            break; \
        default: \
            printf("Error! VUL_TYPE is not correct for access;1\n"); \
            exit(1); \
    } \
    UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0)

#define ACCESS_UNKNOWN0_WRITE0_LAST1_U0(vul_type) \
    ACCESS_UNKNOWN0_LAST1_U0(mread, vul_type)

#define ACCESS_UNKNOWN0_WRITE0_LAST1_U1(vul_type) \
    ACCESS_UNKNOWN0_WRITE0_LAST1_U0(vul_type)

#define ACCESS_UNKNOWN0_WRITE1_LAST1_U0(vul_type) \
    ACCESS_UNKNOWN0_LAST1_U0(mwrite, vul_type)

#define ACCESS_UNKNOWN0_WRITE1_LAST1_U1(vul_type) \
    ACCESS_UNKNOWN0_WRITE1_LAST1_U0(vul_type)



/*
 * FLUSH macro used to adapt functions at compilation time.
 *
 * @param write  is this a write invalidation?
 * @param unknown  is the target address unknown by the attacker?
 * @param last  is this invalidation from the last step?
 * @param u  is the last step related to the unknown address?
 * @param vul_type  vulnerability type
 */
#define  __FLUSH(write, unknown, last, u, vul_type) \
    FLUSH_UNKNOWN##unknown##_WRITE##write##_LAST##last##_U##u(vul_type)

#define FLUSH(write, unknown, last, u, vul_type) \
    __FLUSH(write, unknown, last, u, vul_type)


#define FLUSH_UNKNOWN1_LAST0_U0(moperation) \
    int var_num = 0; \
    if (j==PROBE_A) \
    { /* a */ \
        var_num=0; \
        UNROLL_MOP(moperation, start, var_num, tar_block, 0) \
    } \
    else if (j==PROBE_A_ALIAS) \
    { /* a_alias */ \
        var_num=L1_S; \
        UNROLL_MOP(moperation, start, var_num, tar_block, 0) \
    } \
    else if (j==PROBE_NIB) \
    { /* NIB */ \
        /* it will start 512 bytes higher and use untar_block */ \
        var_num=0; \
        UNROLL_MOP(moperation, start, var_num, untar_block, NIB_OFFSET) \
    } \
    else if (j==3) \
    { \
        int dummy_print=1; \
    }

#define FLUSH_UNKNOWN1_WRITE0_LAST0_U0(vul_type) \
    FLUSH_UNKNOWN1_LAST0_U0(mflush)

#define FLUSH_UNKNOWN1_WRITE0_LAST0_U1(vul_type) \
    FLUSH_UNKNOWN1_LAST0_U0(mflush)

#define FLUSH_UNKNOWN1_WRITE1_LAST0_U0(vul_type) \
    FLUSH_UNKNOWN1_LAST0_U0(mwrite)

#define FLUSH_UNKNOWN1_WRITE1_LAST0_U1(vul_type) \
    FLUSH_UNKNOWN1_LAST0_U0(mwrite)


#define FLUSH_UNKNOWN1_LAST1_U0(moperation) \
    int var_num = 0; \
    if (j==PROBE_A) \
    { /* a */ \
        var_num=0; \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
    } \
    else if (j==PROBE_A_ALIAS) \
    { /* a_alias */ \
        var_num=L1_S; \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
    } \
    else if (j==PROBE_NIB) \
    { /* NIB */ \
        /* it will start 512 bytes higher and use untar_block */ \
        var_num=0; \
        UNROLL_TIMING_MOP(moperation, start, var_num, untar_block, NIB_OFFSET, t, 0) \
    } \
    else if (j==3) \
    { \
        int dummy_print=1; \
    }

#define FLUSH_UNKNOWN1_WRITE0_LAST1_U0(vul_type) \
    FLUSH_UNKNOWN1_LAST1_U0(mflush)

#define FLUSH_UNKNOWN1_WRITE1_LAST1_U0(vul_type) \
    FLUSH_UNKNOWN1_LAST1_U0(mwrite)


#define FLUSH_UNKNOWN1_LAST1_U1(moperation) \
    int var_num = 0; \
    if (j==PROBE_A) \
    { /* a */ \
        var_num=0; \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t_r, 0) \
    } \
    else if (j==PROBE_A_ALIAS) \
    { /* a_alias */ \
        var_num=L1_S; \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
        UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t_r, 0) \
    } \
    else if (j==PROBE_NIB) \
    { /* NIB */ \
        /* it will start 512 bytes higher and use untar_block */ \
        var_num=0; \
        UNROLL_TIMING_MOP(moperation, start, var_num, untar_block, NIB_OFFSET, t, 0) \
        UNROLL_TIMING_MOP(moperation, start, var_num, untar_block, NIB_OFFSET, t_r, 0) \
    } \
    else if (j==3) \
    { \
        int dummy_print=1; \
    }

#define FLUSH_UNKNOWN1_WRITE0_LAST1_U1(vul_type) \
    FLUSH_UNKNOWN1_LAST1_U1(mflush)

#define FLUSH_UNKNOWN1_WRITE1_LAST1_U1(vul_type) \
    FLUSH_UNKNOWN1_LAST1_U1(mwrite)


#define FLUSH_UNKNOWN0_LAST0_U0(moperation, vul_type) \
    int var_num = RANDOM_NUM; \
    int vul = vul_type; \
    switch (vul) \
    { \
        case INV: \
            int var_num = 0; \
            UNROLL_MOP(moperation, start, 0, tar_block, 0) \
            UNROLL_MOP(moperation, start, L1_S, tar_block, 0) \
            UNROLL_MOP(moperation, start, 2*L1_S, tar_block, 0) \
            /* start 512 bytes higher and use untar_block */ \
            UNROLL_MOP(moperation, start, 0, untar_block, NIB_OFFSET) \
            break; \
        case A_INV: \
            var_num = 0; \
            UNROLL_MOP(moperation, start, var_num, tar_block, 0) \
            break; \
        case D_INV: \
            var_num = 2*L1_S; \
            UNROLL_MOP(moperation, start, var_num, tar_block, 0) \
            break; \
        case A_ALIAS_INV: \
            var_num = L1_S; \
            UNROLL_MOP(moperation, start, var_num, tar_block, 0) \
            break; \
        default: \
            printf("Error! VUL_TYPE is not correct for flush;1\n"); \
            exit(1); \
    }

#define FLUSH_UNKNOWN0_WRITE0_LAST0_U0(vul_type) \
    FLUSH_UNKNOWN0_LAST0_U0(mflush, vul_type)

#define FLUSH_UNKNOWN0_WRITE0_LAST0_U1(vul_type) \
    FLUSH_UNKNOWN0_WRITE0_LAST0_U0(vul_type)

#define FLUSH_UNKNOWN0_WRITE1_LAST0_U0(vul_type) \
    FLUSH_UNKNOWN0_LAST0_U0(mwrite, vul_type)

#define FLUSH_UNKNOWN0_WRITE1_LAST0_U1(vul_type) \
    FLUSH_UNKNOWN0_WRITE1_LAST0_U0(vul_type)


#define FLUSH_UNKNOWN0_LAST1_U0(moperation, vul_type) \
    int var_num = RANDOM_NUM; \
    int vul = vul_type; \
    switch (vul) \
    { \
        case INV: \
            int var_num = 0; \
            UNROLL_TIMING_MOP(moperation, start, 0, tar_block, 0, t, 0) \
            UNROLL_TIMING_MOP(moperation, start, L1_S, tar_block, 0, t, L1_ASSOC) \
            UNROLL_TIMING_MOP(moperation, start, 2*L1_S, tar_block, 0, t, 2*L1_ASSOC) \
            /* start 512 bytes higher and use untar_block */ \
            UNROLL_TIMING_MOP(moperation, start, 0, untar_block, NIB_OFFSET, t, 3*L1_ASSOC) \
            break; \
        case A_INV: \
            var_num = 0; \
            UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
            break; \
        case D_INV: \
            var_num = 2*L1_S; \
            UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
            break; \
        case A_ALIAS_INV: \
            var_num = L1_S; \
            UNROLL_TIMING_MOP(moperation, start, var_num, tar_block, 0, t, 0) \
            break; \
        default: \
            printf("Error! VUL_TYPE is not correct for flush;1\n"); \
            exit(1); \
    }

#define FLUSH_UNKNOWN0_WRITE0_LAST1_U0(vul_type) \
    FLUSH_UNKNOWN0_LAST1_U0(mflush, vul_type)

#define FLUSH_UNKNOWN0_WRITE1_LAST1_U0(vul_type) \
    FLUSH_UNKNOWN0_LAST1_U0(mwrite, vul_type)

#define FLUSH_UNKNOWN0_WRITE0_LAST1_U1(vul_type) \
    FLUSH_UNKNOWN0_LAST1_U0(mflush, vul_type)

#define FLUSH_UNKNOWN0_WRITE1_LAST1_U1(vul_type) \
    FLUSH_UNKNOWN0_LAST1_U0(mwrite, vul_type)


#define PRE_STEP(last_step) \
    rand_chosen = rand()%(TOTAL_ADDRESS_VALUES+1); \
    if (m==10 && i==10) \
        printf("rand_chosen=%d\n", rand_chosen); \
    if (rand_chosen==0) \
    { \
        ACCESS(0, 0, last_step, A, 0) \
    } \
    else if (rand_chosen==1) \
    { \
        ACCESS(0, 0, last_step, A_ALIAS, 0) \
    } \
    else if (rand_chosen==2) \
    { \
        ACCESS(0, 0, last_step, D, 0) \
    } \
    else if (rand_chosen==3) \
    { \
        FLUSH(0, 0, last_step, INV, 0) \
    } \
    else if (rand_chosen==4) \
    { \
        FLUSH(0, 0, last_step, A_INV, 0) \
    } \
    else if (rand_chosen==5) \
    { \
        FLUSH(0, 0, last_step, D_INV, 0) \
    } else if (rand_chosen==6) \
    { \
        FLUSH(0, 0, last_step, A_ALIAS_INV, 0) \
    } \
    else \
    {} \
    /*  initiate maintain_arr */

#define STAR_STEP(last_step) \
    rand_chosen = rand()%(TOTAL_ADDRESS_VALUES*2+1); \
    if (m==10 && i==10)  \
        printf("rand_chosen=%d\n", rand_chosen); \
    if (rand_chosen==0) \
    { \
        ACCESS(0, 0, last_step, A, 0) \
    } \
    else if (rand_chosen==1) \
    { \
        ACCESS(1, 0, last_step, A, 0) \
    } \
    else if (rand_chosen==2) \
    { \
        ACCESS(0, 0, last_step, A_ALIAS, 0) \
    } \
    else if (rand_chosen==3) \
    { \
        ACCESS(1, 0, last_step, A_ALIAS, 0) \
    } \
    else if (rand_chosen==4) \
    { \
        ACCESS(0, 0, last_step, D, 0) \
    } \
    else if (rand_chosen==5) \
    { \
        ACCESS(1, 0, last_step, D, 0) \
    } \
    else if (rand_chosen==6) \
    { \
        FLUSH(0, 0, last_step, INV, 0) \
    } \
    else if (rand_chosen==7) \
    { \
        FLUSH(1, 0, last_step, INV, 0) \
    } \
    else if (rand_chosen==8) \
    { \
        FLUSH(0, 0, last_step, A_INV, 0) \
    } \
    else if (rand_chosen==9) \
    { \
        FLUSH(1, 0, last_step, A_INV, 0) \
    } \
    else if (rand_chosen==10) \
    { \
        FLUSH(0, 0, last_step, D_INV, 0) \
    } \
    else if (rand_chosen==11) \
    { \
        FLUSH(1, 0, last_step, D_INV, 0) \
    } \
    else if (rand_chosen==12) \
    { \
        FLUSH(0, 0, last_step, A_ALIAS_INV, 0) \
    } \
    else if (rand_chosen==13) \
    { \
        FLUSH(1, 0, last_step, A_ALIAS_INV, 0) \
    } \
    else \
    {} \
    /*  initiate maintain_arr */

#endif // !__BENCHMARK_H
