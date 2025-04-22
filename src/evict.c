/*
* This file defines eviction functions used for the benchmark suite
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

#include "evict.h"

/**
 * Evict cache using the method from:
 *     Gruss, D., Maurice, C., Mangard, S. (2016).
 *     Rowhammer.js: A Remote Software-Induced Fault Attack in JavaScript.
 *     In: Caballero, J., Zurutuza, U., Rodríguez, R. (eds)
 *     Detection of Intrusions and Malware, and Vulnerability Assessment.
 *     DIMVA 2016. Lecture Notes in Computer Science(), vol 9721.
 *     Springer, Cham. https://doi.org/10.1007/978-3-319-40667-1_15
 * Parameters of this algorithm have to be used.
 *
 * @param array  pointer to the address of an array element;
 * @param c,d,l,s  parameters from Gruss' paper.
 */
void evict(char** array, int c, int d, int l, int s)
{
    for (int _s = 1; _s <= s-d+1; _s+=l)
    {
        for (int _c = 0; _c < c; _c++)
        {
            for (int _d = 0; _d < d; _d++)
            {
                mread(array[_s+_d]);
            }
        }
    }
}

/**
 * Evict cache in a similar way to the `evict` function but allowing offsets
 * on a given address.
 * Parameters of the Gruss' algorithm have to be used.
 *
 * @param array  pointer to the address of an array element;
 * @param inner_offset  offset on the element index;
 * @param outer_offset  offset on the element address;
 * @param c,d,l,s  parameters from Gruss' paper;
 * @param operation  pointer to a memory operation function to use for the
 *          eviction.
 */
void evict_offset(
        char** array, uint64_t inner_offset, uint64_t outer_offset,
        int c, int d, int l, int s, void (*operation)(char*)
)
{
    for (int _s = 0; _s <= s-d; _s+=l)
    {
        for (int _c = 0; _c < c; _c++)
        {
            for (int _d = 0; _d < d; _d++)
            {
                operation(array[inner_offset+_s+_d]+outer_offset);
            }
        }
    }
}
