/*
* This file is the header file for generating benchmark code
*
* Copyright (C) 2020
* Author: Shuwen Deng <shuwen.deng@yale.edu>
* Date:   02.04.2020
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


#ifndef GEN_VULNERABILITIES_H 

#define GEN_VULNERABILITIES_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Attacker 0
#define Victim 1
#define a 2
#define a_alias 3
#define d 4
#define u 5
#define inv 6
#define a_inv 7
#define d_inv 8
#define u_inv 9
#define a_alias_inv 10
#define miss 11
#define hit 12
#define star 13

#define TOTAL_VUL 4913
int total_arr[4913][8];

int three_step_arr[88][8] = {
        // cache internal collision
        {0,         Attacker,        inv,        Victim,         u,             Victim,         a,                 hit},
        {1,         Victim,          inv,        Victim,         u,             Victim,         a,                 hit},
        {2,         Attacker,        a_inv,      Victim,         u,             Victim,         a,                 hit},
        {3,         Victim,          a_inv,      Victim,         u,             Victim,         a,                 hit},

        // flush + reload
        {4,         Attacker,        a_inv,      Victim,         u,             Attacker,       a,                 hit},
        {5,         Victim,          a_inv,      Victim,         u,             Attacker,       a,                 hit},
        {6,         Attacker,        inv,        Victim,         u,             Attacker,       a,                 hit},
        {7,         Victim,          inv,        Victim,         u,             Attacker,       a,                 hit},
        
	// reload + time
        {8,         Victim,          u_inv,      Attacker,       a,             Victim,         u,                 hit},
        {9,         Victim,          u_inv,      Victim,         a,             Victim,         u,                 hit},
        
	// flush + probe
        {10,         Attacker,       a,          Victim,         u_inv,         Attacker,       a,                 hit},
        {11,         Attacker,       a,          Victim,         u_inv,         Victim,         a,                 hit},
        {12,         Victim,         a,          Victim,         u_inv,         Attacker,       a,                 hit},
        {13,         Victim,         a,          Victim,         u_inv,         Victim,         a,                 hit},
        
	// flush + time
        {14,         Victim,         u,          Attacker,       a_inv,         Victim,         u,                 hit},
        {15,         Victim,         u,          Victim,         a_inv,         Victim,         u,                 hit},

	// cache coherence flush + reload
        {16,         Attacker,       inv,        Victim,         u_inv,         Attacker,       a,                 hit},        
        {17,         Attacker,       inv,        Victim,         u_inv,         Victim,         a,                 hit},         
        {18,         Victim,         inv,        Victim,         u_inv,         Attacker,       a,                 hit},         
        {19,         Victim,         inv,        Victim,         u_inv,         Victim,         a,                 hit},  
        // cache coherence prime + probe       
        {20,         Attacker,       a_inv,      Victim,         u_inv,         Attacker,       a,                 hit},         
        {21,         Attacker,       a_inv,      Victim,         u_inv,         Victim,         a,                 hit},         
        {22,         Victim,         a_inv,      Victim,         u_inv,         Attacker,       a,                 hit},         
        {23,         Victim,         a_inv,      Victim,         u_inv,         Victim,         a,                 hit},         
        {24,         Attacker,       d_inv,      Victim,         u_inv,         Attacker,       d,                 hit},         
        {25,         Attacker,       d_inv,      Victim,         u_inv,         Victim,         d,                 hit},         
        {26,         Victim,         d_inv,      Victim,         u_inv,         Attacker,       d,                 hit},         
        {27,         Victim,         d_inv,      Victim,         u_inv,         Victim,         d,                 hit},  
        // cache coherence evict + time       
        {28,         Victim,         u_inv,      Attacker,       a_inv,         Victim,         u,                 hit},         
        {29,         Victim,         u_inv,      Victim,         a_inv,         Victim,         u,                 hit},         
        {30,         Victim,         u_inv,      Attacker,       d_inv,         Victim,         u,                 hit},         
        {31,         Victim,         u_inv,      Victim,         d_inv,         Victim,         u,                 hit},         

        // Bernstein's attack
        {32,         Victim,         u,          Victim,         a,             Victim,         u,                 miss},
        {33,         Victim,         u,          Victim,         d,             Victim,         u,                 miss},
        {34,         Victim,         d,          Victim,         u,             Victim,         d,                 miss},
        {35,         Victim,         a,          Victim,         u,             Victim,         a,                 miss},

        // evict + probe
        {36,         Victim,         d,          Victim,         u,             Attacker,       d,                 miss},
        {37,         Victim,         a,          Victim,         u,             Attacker,       a,                 miss},

        // prime + time
        {38,         Attacker,       d,          Victim,         u,             Victim,         d,                 miss},
        {39,         Attacker,       a,          Victim,         u,             Victim,         a,                 miss},
        
        // evict + time
        {40,         Victim,         u,          Attacker,       d,             Victim,         u,                 miss},
        {41,         Victim,         u,          Attacker,       a,             Victim,         u,                 miss},

        // prime + probe
        {42,         Attacker,       d,          Victim,         u,             Attacker,       d,                 miss},
        {43,         Attacker,       a,          Victim,         u,             Attacker,       a,                 miss},
        
        // cache internal collision invalidation
        {44,         Attacker,       inv,        Victim,         u,             Victim,         a_inv,         hit},
        {45,         Victim,         inv,        Victim,         u,             Victim,         a_inv,         hit},
        
	// flush + flush
        {46,         Attacker,       a_inv,      Victim,         u,             Victim,         a_inv,         hit},
        {47,         Victim,         a_inv,      Victim,         u,             Victim,         a_inv,         hit},
        {48,         Attacker,       a_inv,      Victim,         u,             Attacker,       a_inv,         hit},
        {49,         Victim,         a_inv,      Victim,         u,             Attacker,       a_inv,         hit},
        
	// flush + reload invalidation
        {50,         Attacker,       inv,        Victim,         u,             Attacker,       a_inv,         hit},
        {51,         Victim,         inv,        Victim,         u,             Attacker,       a_inv,         hit},
        
	// reload + time invalidation
        {52,         Victim,         u_inv,      Attacker,       a,             Victim,         u_inv,         hit},
        {53,         Victim,         u_inv,      Victim,         a,             Victim,         u_inv,         hit},
        
	// flush + probe invalidation
        {54,         Attacker,       a,          Victim,         u_inv,         Attacker,       a_inv,         hit},
        {55,         Attacker,       a,          Victim,         u_inv,         Victim,         a_inv,         hit},
        {56,         Victim,         a,          Victim,         u_inv,         Attacker,       a_inv,         hit},
        {57,         Victim,         a,          Victim,         u_inv,         Victim,         a_inv,         hit},
        
	// flush + time invalidation
        {58,         Victim,         u,          Attacker,       a_inv,         Victim,         u_inv,         hit},
        {59,         Victim,         u,          Victim,         a_inv,         Victim,         u_inv,         hit},

        // cache coherence flush + reload invalidation
        {60,         Attacker,       inv,        Victim,         u_inv,         Attacker,       a_inv,         hit},         
        {61,         Attacker,       inv,        Victim,         u_inv,         Victim,         a_inv,         hit},         
        {62,         Victim,         inv,        Victim,         u_inv,         Attacker,       a_inv,         hit},         
        {63,         Victim,         inv,        Victim,         u_inv,         Victim,         a_inv,         hit},   
        // cache coherence prime + probe invalidation     
        {64,         Attacker,       a_inv,      Victim,         u_inv,         Attacker,       a_inv,         hit},         
        {65,         Attacker,       a_inv,      Victim,         u_inv,         Victim,         a_inv,         hit},         
        {66,         Victim,         a_inv,      Victim,         u_inv,         Attacker,       a_inv,         hit},         
        {67,         Victim,         a_inv,      Victim,         u_inv,         Victim,         a_inv,         hit},         
        {68,         Attacker,       d_inv,      Victim,         u_inv,         Attacker,       d_inv,         hit},         
        {69,         Attacker,       d_inv,      Victim,         u_inv,         Victim,         d_inv,         hit},         
        {70,         Victim,         d_inv,      Victim,         u_inv,         Attacker,       d_inv,         hit},         
        {71,         Victim,         d_inv,      Victim,         u_inv,         Victim,         d_inv,         hit}, 
        // cache coherence evict + time invalidation        
        {72,         Victim,         u_inv,      Attacker,       a_inv,         Victim,         u_inv,         hit},         
        {73,         Victim,         u_inv,      Victim,         a_inv,         Victim,         u_inv,         hit},         
        {74,         Victim,         u_inv,      Attacker,       d_inv,         Victim,         u_inv,         hit},         
        {75,         Victim,         u_inv,      Victim,         d_inv,         Victim,         u_inv,         hit}, 

        // Bernstein's attack invalidation
        {76,         Victim,         u,          Victim,         a,             Victim,         u_inv,         miss},
        {77,         Victim,         u,          Victim,         d,             Victim,         u_inv,         miss},
        {78,         Victim,         d,          Victim,         u,             Victim,         d_inv,         miss},
        {79,         Victim,         a,          Victim,         u,             Victim,         a_inv,         miss},

        // evict + probe invalidation
        {80,         Victim,         d,          Victim,         u,             Attacker,       d_inv,         miss},
        {81,         Victim,         a,          Victim,         u,             Attacker,       a_inv,         miss},

        // prime + time invalidation
        {82,         Attacker,       d,          Victim,         u,             Victim,         d_inv,         miss},
        {83,         Attacker,       a,          Victim,         u,             Victim,         a_inv,         miss},

        // evict + time invalidation
        {84,         Victim,         u,          Attacker,       d,             Victim,         u_inv,         miss},
        {85,         Victim,         u,          Attacker,       a,             Victim,         u_inv,         miss},

        // prime + probe invalidation
        {86,         Attacker,       d,          Victim,         u,             Attacker,       d_inv,         miss},
        {87,         Attacker,       a,          Victim,         u,             Attacker,       a_inv,         miss}};


int possi_access[17][2] = {
        {Victim,        u},
        {Attacker,      a},
        {Victim,        a},
        {Attacker,      a_alias},
        {Victim,        a_alias},
        {Attacker,      d},
        {Victim,        d},
        {Attacker,      inv},
        {Victim,        inv},
        {Attacker,      a_inv},
        {Victim,        a_inv},
        {Attacker,      a_alias_inv},
        {Victim,        a_alias_inv},
        {Attacker,      d_inv},
        {Victim,        d_inv},
        {Victim,        u_inv},
        {Victim,        star}
        };

#endif
