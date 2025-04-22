/*
* This file is the header file for statistical functions
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

#ifndef __STAT_H
#define __STAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// t test
// calculate a p-value based on an array
double Pvalue (const double *restrict ARRAY1, const size_t ARRAY1_SIZE, const double *restrict ARRAY2, const size_t ARRAY2_SIZE);

#endif // !__STAT_H
