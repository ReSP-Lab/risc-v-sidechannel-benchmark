/*
* This file is the header file for eviction functions
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

#ifndef __EVICT_H
#define __EVICT_H

#include "stdint.h"
#include "asm_functions.h"

void evict(char** array, int c, int d, int l, int s);

void evict_offset(
        char** array, uint64_t inner_offset, uint64_t outer_offset,
        int c, int d, int l, int s, void (*operation)(char*)
);

#endif // !__EVICT_H
