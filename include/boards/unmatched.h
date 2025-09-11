/*
* This file is the header file to define the HiFive Unmatched board macros
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

#ifndef __UNMATCHED_BOARD_H
#define __UNMATCHED_BOARD_H

/* CONSTANTS */

/* Board-specific constants */
#define CACHE_LINE      64 // 64B cache block
#define L1_CACHE_SIZE   32*1024
#define L1_ASSOC        8
#define L2_CACHE_SIZE   2*1024*1024
#define L2_ASSOC        16
#define NO_FLUSH
#define NO_L3_CACHE

/* Eviction strategy parameters */
#define L1_S L1_ASSOC+2
#define L1_D L1_S-1
#define L1_C 4
#define L1_L 1
#define L2_C 4
#define L2_S L2_ASSOC+4
#define L2_D L2_S-1
#define L2_L 1

/* CPU definitions */
#define LOCAL_CPU1  1
#define LOCAL_CPU2  1
#define REMOTE_CPU1 2
#define REMOTE_CPU2 2
#define REST_CPU    3

#define MULTICORE

/* Board-specific inline instructions */
__attribute__((always_inline))
static inline void flush(size_t char_ptr)
{
    // not needed
}

#endif // !__UNMATCHED_BOARD_H
