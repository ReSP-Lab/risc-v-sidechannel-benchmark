/*
* This file is the header file to define the BeagleV Ahead board macros
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

#ifndef __BVAHEAD_BOARD_H
#define __BVAHEAD_BOARD_H

/* CONSTANTS */

/* Board-specific constants */
#define CACHE_LINE      64 // 64B cache block
#define L1_CACHE_SIZE   64*1024
#define L1_ASSOC        2
#define L2_CACHE_SIZE   1*1024*1024
#define L2_ASSOC        16
#define NO_L3_CACHE

/* Eviction strategy parameters */
#define L1_C 1
#define L1_D 1
#define L1_L 1
#define L1_S L1_ASSOC
#define L2_C 1
#define L2_D 1
#define L2_L 1
#define L2_S L2_ASSOC

/* CPU definitions */
#define LOCAL_CPU1  0
#define LOCAL_CPU2  0
#define REMOTE_CPU1 2
#define REMOTE_CPU2 2
#define REST_CPU    1

#define MULTICORE


/* Board-specific inline instructions */
__attribute__((always_inline))
static inline void flush(size_t char_ptr)
{
    asm __volatile__(
        "addi a7, %[c], 0\n"
        ".long 0x278800b \n"
        :
        : [c] "r" (char_ptr)
        : "a7", "memory"
    );
}

#endif // !__BVAHEAD_BOARD_H
