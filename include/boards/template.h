/*
* This file is a template header file to define target board macros
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

// TODO: adapt the macro
#ifndef __TEMPLATE_BOARD_H
#define __TEMPLATE_BOARD_H

/* CONSTANTS */

/* Board-specific constants */
// TODO: set the values
#define CACHE_LINE      64 // 64B cache block - change if needed
#define L1_CACHE_SIZE   ...
#define L1_ASSOC        ...
#define L2_CACHE_SIZE   ...
#define L2_ASSOC        ...
//#define NO_FLUSH // TODO: uncomment if it has custom instruction
//#define NO_L3_CACHE // TODO: uncomment if no L3 cache is present

/* Eviction strategy parameters */
// TODO: adapt to the strategy which works on the processor
// TODO: comment levels which do not exist
#define L1_C 1
#define L1_D 1
#define L1_L 1
#define L1_S L1_ASSOC
#define L2_C 1
#define L2_D 1
#define L2_L 1
#define L2_S L2_ASSOC
#define L3_C 1
#define L3_D 1
#define L3_L 1
#define L3_S L3_ASSOC

/* CPU definitions */
// TODO: CPU2 should be identical to CPU1 if no L3 exists or L2 is not shared between at least two cores
// TODO: REMOTE_CPU1 has to be set to a core independent from LOCAL_CPU1
#define LOCAL_CPU1  0
#define LOCAL_CPU2  0
#define REMOTE_CPU1 2
#define REMOTE_CPU2 2
#define REST_CPU    1

// TODO: comment if single core
#define MULTICORE


/* Board-specific inline instructions */
__attribute__((always_inline))
static inline void flush(size_t char_ptr)
{
    // TODO: define it the processor has a custom flush instruction
}

#endif
