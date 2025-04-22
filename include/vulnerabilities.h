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


#ifndef __VULNERABILITIES_H
#define __VULNERABILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Operation source */
#define ATTACKER 0
#define VICTIM 1
/* Operation type and resulting state */
#define A 2
#define A_ALIAS 3
#define D 4
#define U 5
#define INV 6
#define A_INV 7
#define D_INV 8
#define U_INV 9
#define A_ALIAS_INV 10
/* Hit or miss-based */
#define MISS 11
#define HIT 12
/* Unknown state */
#define STAR 13
#define PRE 14

#define VUL_NUM 88
#define ALL_NUM 4913
#define TOTAL_STATES 17

#define TOTAL_VUL ALL_NUM
int total_arr[ALL_NUM][8];

int three_step_arr[VUL_NUM][8] = {
        // cache internal collision
        {0,         ATTACKER,        INV,        VICTIM,         U,             VICTIM,         A,                 HIT},
        {1,         VICTIM,          INV,        VICTIM,         U,             VICTIM,         A,                 HIT},
        {2,         ATTACKER,        A_INV,      VICTIM,         U,             VICTIM,         A,                 HIT},
        {3,         VICTIM,          A_INV,      VICTIM,         U,             VICTIM,         A,                 HIT},

        // flush + reload
        {4,         ATTACKER,        A_INV,      VICTIM,         U,             ATTACKER,       A,                 HIT},
        {5,         VICTIM,          A_INV,      VICTIM,         U,             ATTACKER,       A,                 HIT},
        {6,         ATTACKER,        INV,        VICTIM,         U,             ATTACKER,       A,                 HIT},
        {7,         VICTIM,          INV,        VICTIM,         U,             ATTACKER,       A,                 HIT},

        // reload + time
        {8,         VICTIM,          U_INV,      ATTACKER,       A,             VICTIM,         U,                 HIT},
        {9,         VICTIM,          U_INV,      VICTIM,         A,             VICTIM,         U,                 HIT},

        // flush + probe
        {10,         ATTACKER,       A,          VICTIM,         U_INV,         ATTACKER,       A,                 HIT},
        {11,         ATTACKER,       A,          VICTIM,         U_INV,         VICTIM,         A,                 HIT},
        {12,         VICTIM,         A,          VICTIM,         U_INV,         ATTACKER,       A,                 HIT},
        {13,         VICTIM,         A,          VICTIM,         U_INV,         VICTIM,         A,                 HIT},

        // flush + time
        {14,         VICTIM,         U,          ATTACKER,       A_INV,         VICTIM,         U,                 HIT},
        {15,         VICTIM,         U,          VICTIM,         A_INV,         VICTIM,         U,                 HIT},

        // cache coherence flush + reload
        {16,         ATTACKER,       INV,        VICTIM,         U_INV,         ATTACKER,       A,                 HIT},        
        {17,         ATTACKER,       INV,        VICTIM,         U_INV,         VICTIM,         A,                 HIT},         
        {18,         VICTIM,         INV,        VICTIM,         U_INV,         ATTACKER,       A,                 HIT},         
        {19,         VICTIM,         INV,        VICTIM,         U_INV,         VICTIM,         A,                 HIT},  

        // cache coherence prime + probe
        {20,         ATTACKER,       A_INV,      VICTIM,         U_INV,         ATTACKER,       A,                 HIT},         
        {21,         ATTACKER,       A_INV,      VICTIM,         U_INV,         VICTIM,         A,                 HIT},         
        {22,         VICTIM,         A_INV,      VICTIM,         U_INV,         ATTACKER,       A,                 HIT},         
        {23,         VICTIM,         A_INV,      VICTIM,         U_INV,         VICTIM,         A,                 HIT},         
        {24,         ATTACKER,       D_INV,      VICTIM,         U_INV,         ATTACKER,       D,                 HIT},         
        {25,         ATTACKER,       D_INV,      VICTIM,         U_INV,         VICTIM,         D,                 HIT},         
        {26,         VICTIM,         D_INV,      VICTIM,         U_INV,         ATTACKER,       D,                 HIT},         
        {27,         VICTIM,         D_INV,      VICTIM,         U_INV,         VICTIM,         D,                 HIT},  

        // cache coherence evict + time
        {28,         VICTIM,         U_INV,      ATTACKER,       A_INV,         VICTIM,         U,                 HIT},         
        {29,         VICTIM,         U_INV,      VICTIM,         A_INV,         VICTIM,         U,                 HIT},         
        {30,         VICTIM,         U_INV,      ATTACKER,       D_INV,         VICTIM,         U,                 HIT},         
        {31,         VICTIM,         U_INV,      VICTIM,         D_INV,         VICTIM,         U,                 HIT},         

        // Bernstein's attack
        {32,         VICTIM,         U,          VICTIM,         A,             VICTIM,         U,                 MISS},
        {33,         VICTIM,         U,          VICTIM,         D,             VICTIM,         U,                 MISS},
        {34,         VICTIM,         D,          VICTIM,         U,             VICTIM,         D,                 MISS},
        {35,         VICTIM,         A,          VICTIM,         U,             VICTIM,         A,                 MISS},

        // evict + probe
        {36,         VICTIM,         D,          VICTIM,         U,             ATTACKER,       D,                 MISS},
        {37,         VICTIM,         A,          VICTIM,         U,             ATTACKER,       A,                 MISS},

        // prime + time
        {38,         ATTACKER,       D,          VICTIM,         U,             VICTIM,         D,                 MISS},
        {39,         ATTACKER,       A,          VICTIM,         U,             VICTIM,         A,                 MISS},

        // evict + time
        {40,         VICTIM,         U,          ATTACKER,       D,             VICTIM,         U,                 MISS},
        {41,         VICTIM,         U,          ATTACKER,       A,             VICTIM,         U,                 MISS},

        // prime + probe
        {42,         ATTACKER,       D,          VICTIM,         U,             ATTACKER,       D,                 MISS},
        {43,         ATTACKER,       A,          VICTIM,         U,             ATTACKER,       A,                 MISS},

        // cache internal collision INValidation
        {44,         ATTACKER,       INV,        VICTIM,         U,             VICTIM,         A_INV,         HIT},
        {45,         VICTIM,         INV,        VICTIM,         U,             VICTIM,         A_INV,         HIT},

        // flush + flush
        {46,         ATTACKER,       A_INV,      VICTIM,         U,             VICTIM,         A_INV,         HIT},
        {47,         VICTIM,         A_INV,      VICTIM,         U,             VICTIM,         A_INV,         HIT},
        {48,         ATTACKER,       A_INV,      VICTIM,         U,             ATTACKER,       A_INV,         HIT},
        {49,         VICTIM,         A_INV,      VICTIM,         U,             ATTACKER,       A_INV,         HIT},

        // flush + reload INValidation
        {50,         ATTACKER,       INV,        VICTIM,         U,             ATTACKER,       A_INV,         HIT},
        {51,         VICTIM,         INV,        VICTIM,         U,             ATTACKER,       A_INV,         HIT},

        // reload + time INValidation
        {52,         VICTIM,         U_INV,      ATTACKER,       A,             VICTIM,         U_INV,         HIT},
        {53,         VICTIM,         U_INV,      VICTIM,         A,             VICTIM,         U_INV,         HIT},

        // flush + probe INValidation
        {54,         ATTACKER,       A,          VICTIM,         U_INV,         ATTACKER,       A_INV,         HIT},
        {55,         ATTACKER,       A,          VICTIM,         U_INV,         VICTIM,         A_INV,         HIT},
        {56,         VICTIM,         A,          VICTIM,         U_INV,         ATTACKER,       A_INV,         HIT},
        {57,         VICTIM,         A,          VICTIM,         U_INV,         VICTIM,         A_INV,         HIT},

        // flush + time INValidation
        {58,         VICTIM,         U,          ATTACKER,       A_INV,         VICTIM,         U_INV,         HIT},
        {59,         VICTIM,         U,          VICTIM,         A_INV,         VICTIM,         U_INV,         HIT},

        // cache coherence flush + reload INValidation
        {60,         ATTACKER,       INV,        VICTIM,         U_INV,         ATTACKER,       A_INV,         HIT},         
        {61,         ATTACKER,       INV,        VICTIM,         U_INV,         VICTIM,         A_INV,         HIT},         
        {62,         VICTIM,         INV,        VICTIM,         U_INV,         ATTACKER,       A_INV,         HIT},         
        {63,         VICTIM,         INV,        VICTIM,         U_INV,         VICTIM,         A_INV,         HIT},   
        // cache coherence prime + probe INValidation     
        {64,         ATTACKER,       A_INV,      VICTIM,         U_INV,         ATTACKER,       A_INV,         HIT},         
        {65,         ATTACKER,       A_INV,      VICTIM,         U_INV,         VICTIM,         A_INV,         HIT},         
        {66,         VICTIM,         A_INV,      VICTIM,         U_INV,         ATTACKER,       A_INV,         HIT},         
        {67,         VICTIM,         A_INV,      VICTIM,         U_INV,         VICTIM,         A_INV,         HIT},         
        {68,         ATTACKER,       D_INV,      VICTIM,         U_INV,         ATTACKER,       D_INV,         HIT},         
        {69,         ATTACKER,       D_INV,      VICTIM,         U_INV,         VICTIM,         D_INV,         HIT},         
        {70,         VICTIM,         D_INV,      VICTIM,         U_INV,         ATTACKER,       D_INV,         HIT},         
        {71,         VICTIM,         D_INV,      VICTIM,         U_INV,         VICTIM,         D_INV,         HIT}, 
        // cache coherence evict + time INValidation        
        {72,         VICTIM,         U_INV,      ATTACKER,       A_INV,         VICTIM,         U_INV,         HIT},         
        {73,         VICTIM,         U_INV,      VICTIM,         A_INV,         VICTIM,         U_INV,         HIT},         
        {74,         VICTIM,         U_INV,      ATTACKER,       D_INV,         VICTIM,         U_INV,         HIT},         
        {75,         VICTIM,         U_INV,      VICTIM,         D_INV,         VICTIM,         U_INV,         HIT}, 

        // Bernstein's attack INValidation
        {76,         VICTIM,         U,          VICTIM,         A,             VICTIM,         U_INV,         MISS},
        {77,         VICTIM,         U,          VICTIM,         D,             VICTIM,         U_INV,         MISS},
        {78,         VICTIM,         D,          VICTIM,         U,             VICTIM,         D_INV,         MISS},
        {79,         VICTIM,         A,          VICTIM,         U,             VICTIM,         A_INV,         MISS},

        // evict + probe INValidation
        {80,         VICTIM,         D,          VICTIM,         U,             ATTACKER,       D_INV,         MISS},
        {81,         VICTIM,         A,          VICTIM,         U,             ATTACKER,       A_INV,         MISS},

        // prime + time INValidation
        {82,         ATTACKER,       D,          VICTIM,         U,             VICTIM,         D_INV,         MISS},
        {83,         ATTACKER,       A,          VICTIM,         U,             VICTIM,         A_INV,         MISS},

        // evict + time INValidation
        {84,         VICTIM,         U,          ATTACKER,       D,             VICTIM,         U_INV,         MISS},
        {85,         VICTIM,         U,          ATTACKER,       A,             VICTIM,         U_INV,         MISS},

        // prime + probe INValidation
        {86,         ATTACKER,       D,          VICTIM,         U,             ATTACKER,       D_INV,         MISS},
        {87,         ATTACKER,       A,          VICTIM,         U,             ATTACKER,       A_INV,         MISS}
};


int possi_access[17][2] = {
        {VICTIM,        U},
        {ATTACKER,      A},
        {VICTIM,        A},
        {ATTACKER,      A_ALIAS},
        {VICTIM,        A_ALIAS},
        {ATTACKER,      D},
        {VICTIM,        D},
        {ATTACKER,      INV},
        {VICTIM,        INV},
        {ATTACKER,      A_INV},
        {VICTIM,        A_INV},
        {ATTACKER,      A_ALIAS_INV},
        {VICTIM,        A_ALIAS_INV},
        {ATTACKER,      D_INV},
        {VICTIM,        D_INV},
        {VICTIM,        U_INV},
        {VICTIM,        STAR}
};

const  char* type_match[14] = {
    "ATT","VIC",
    "A","A_ALIAS","D","U",
    "INV","A_INV","D_INV","U_INV","A_ALIAS_INV",
    "ERROR1", "ERROR2", "STAR"
};

#endif /* ifndef __VULNERABILITIES_H*/
