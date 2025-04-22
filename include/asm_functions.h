/*
* This file defines assembly functions for memory operations
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

#ifndef __ASM_FUNCTIONS_H
#define __ASM_FUNCTIONS_H

#include <stdint.h>

#ifndef ON_MULTIPLE_LINES
#define ON_MULTIPLE_LINES 0
#endif // !ON_MULTIPLE_LINES


__attribute__((always_inline))
static inline void fence_rwrw()
{
    asm __volatile__(
        "fence rw, rw \n"// mfence
    );
}

__attribute__((always_inline))
static inline void fence_rrw()
{
    asm __volatile__(
        "fence r, rw \n"// lfence
    );
}

__attribute__((always_inline))
static inline void fence_ww()
{
    asm __volatile__(
        "fence w, w \n"// sfence
    );
}

__attribute__((always_inline))
static inline uint64_t rdcycle()
{
    uint64_t time;
    asm __volatile__ (
        "rdcycle %[t]       \n" //   rdtsc -> store inside eax
        : [t] "=r" (time)       //: "=a" (t)
    );
    return time;
}


__attribute__((always_inline))
static inline uint64_t mread_timing(char* char_ptr)
{
    uint64_t time = 0;
    asm __volatile__ (          // previously:
        "fence r, rw         \n"//   lfence
        "rdcycle t1          \n"//   rdtsc -> store inside eax
                                //   movl %%eax, %%esi
        "ld t0,   0(%[c])    \n"//   movq (%%rcx),   %%rax
        "fence r, rw         \n"//   lfence
#if ON_MULTIPLE_LINES
        "ld t0,  64(%[c])    \n"//   movq 64(%%rcx), %%rax
        "fence r, rw         \n"//   lfence
        "ld t0, 448(%[c])    \n"//   movq 448(%%rcx),%%rax
        "fence r, rw         \n"//   lfence
        "ld t0, 256(%[c])    \n"//   movq 256(%%rcx),%%rax
        "fence r, rw         \n"//   lfence
        "ld t0, 384(%[c])    \n"//   movq 384(%%rcx),%%rax
        "fence r, rw         \n"//   lfence
        "ld t0, 320(%[c])    \n"//   movq 320(%%rcx),%%rax
        "fence r, rw         \n"//   lfence
        "ld t0, 192(%[c])    \n"//   movq 192(%%rcx),%%rax
        "fence r, rw         \n"//   lfence
        "ld t0, 128(%[c])    \n"//   movq 128(%%rcx),%%rax
        "fence r, rw         \n"//   lfence
#endif // ON_MULTIPLE_LINES
        "rdcycle t2          \n"//   rdtsc -> store inside eax
        "sub %[t], t2, t1    \n"//   subl %%esi, %%eax
        : [t] "=r" (time)       //: "=a" (t)
        : [c] "r" (char_ptr)    //: [c] "r" (candidate)
        : "t0", "t1", "t2", "memory"    //: "%esi", "%edx"
    );
    return time;
}

__attribute__((always_inline))
static inline void mread_mfenced(char* char_ptr)
{
    asm __volatile__ (          // previously:
        "fence rw, rw         \n"//   mfence
        "ld t0,   0(%[c])    \n"//   movq (%%rcx),   %%rax
        "fence rw, rw         \n"//   mfence
#if ON_MULTIPLE_LINES
        "ld t0,  64(%[c])    \n"//   movq 64(%%rcx), %%rax
        "fence rw, rw         \n"//   mfence
        "ld t0, 448(%[c])    \n"//   movq 448(%%rcx),%%rax
        "fence rw, rw         \n"//   mfence
        "ld t0, 256(%[c])    \n"//   movq 256(%%rcx),%%rax
        "fence rw, rw         \n"//   mfence
        "ld t0, 384(%[c])    \n"//   movq 384(%%rcx),%%rax
        "fence rw, rw         \n"//   mfence
        "ld t0, 320(%[c])    \n"//   movq 320(%%rcx),%%rax
        "fence rw, rw         \n"//   mfence
        "ld t0, 192(%[c])    \n"//   movq 192(%%rcx),%%rax
        "fence rw, rw         \n"//   mfence
        "ld t0, 128(%[c])    \n"//   movq 128(%%rcx),%%rax
        "fence rw, rw         \n"//   mfence
#endif // ON_MULTIPLE_LINES
        :
        : [c] "r" (char_ptr)    //: "c" (char_ptr)
        : "t0", "memory"        //: "%esi", "%edx"
    );
}

__attribute__((always_inline))
static inline void mread(char* char_ptr)
{
    asm __volatile__ (          // previously:
        "fence r, rw         \n"//   lfence
        "ld t0,   0(%[c])    \n"//   movq (%%rcx),   %%rax
#if ON_MULTIPLE_LINES
        "ld t0,  64(%[c])    \n"//   movq 64(%%rcx), %%rax
        "ld t0, 448(%[c])    \n"//   movq 448(%%rcx),%%rax
        "ld t0, 256(%[c])    \n"//   movq 256(%%rcx),%%rax
        "ld t0, 384(%[c])    \n"//   movq 384(%%rcx),%%rax
        "ld t0, 320(%[c])    \n"//   movq 320(%%rcx),%%rax
        "ld t0, 192(%[c])    \n"//   movq 192(%%rcx),%%rax
        "ld t0, 128(%[c])    \n"//   movq 128(%%rcx),%%rax
#endif // ON_MULTIPLE_LINES
        "fence r, rw         \n"//   lfence
        :
        : [c] "r" (char_ptr)    //: "c" (char_ptr)
        : "t0", "memory"        //: "%esi", "%edx"
    );
}


__attribute__((always_inline))
static inline uint64_t mwrite_timing(char* char_ptr)
{
    uint64_t time = 0;
    asm __volatile__ (      // previously using mfence
        "fence rw, rw           \n"
        "rdcycle t1         \n" //   rdtsc -> store inside eax
                                //   movl %%eax, %%esi
        "sd %[c],    0(%[c])    \n"
        "fence rw, rw           \n"
#if ON_MULTIPLE_LINES
        "sd %[c],   64(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  448(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  256(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  384(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  320(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  192(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  128(%[c])    \n"
        "fence rw, rw           \n"
#endif // ON_MULTIPLE_LINES
        "rdcycle t2         \n" //   rdtsc -> store inside eax
        "sub %[t], t2, t1   \n" //   subl %%esi, %%eax
        : [t] "=r" (time)       //: "=a" (t)
        : [c] "r" (char_ptr)    //: "c" (start[0])
        : "t1", "t2", "memory"
    );
    return time;
}

__attribute__((always_inline))
static inline uint64_t mwrite_mfenced(char* char_ptr)
{
    asm __volatile__ (      // previously using mfence
        "fence rw, rw           \n"
        "sd %[c],    0(%[c])    \n"
        "fence rw, rw           \n"
#if ON_MULTIPLE_LINES
        "sd %[c],   64(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  448(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  256(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  384(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  320(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  192(%[c])    \n"
        "fence rw, rw           \n"
        "sd %[c],  128(%[c])    \n"
        "fence rw, rw           \n"
#endif // ON_MULTIPLE_LINES
        :
        : [c] "r" (char_ptr)    //: "c" (start[0])
        : "memory"
    );
}

__attribute__((always_inline))
static inline void mwrite_dirty(char* char_ptr)
{
    asm __volatile__ (          // previously using sfence
        "fence w, w             \n"
        "sd %[c],     (%[c])    \n"
#if ON_MULTIPLE_LINES
        "fence w, w             \n"
        "sd %[c],   64(%[c])    \n"
        "fence w, w             \n"
        "sd %[c],  448(%[c])    \n"
        "fence w, w             \n"
        "sd %[c],  256(%[c])    \n"
        "fence w, w             \n"
        "sd %[c],  384(%[c])    \n"
        "fence w, w             \n"
        "sd %[c],  320(%[c])    \n"
        "fence w, w             \n"
        "sd %[c],  192(%[c])    \n"
        "fence w, w             \n"
        "sd %[c],  128(%[c])    \n"
#endif // ON_MULTIPLE_LINES
        "fence w, w             \n"
        :
        : [c] "r" (char_ptr)
        : "memory"
    );
}



__attribute__((always_inline))
static inline uint64_t mflush_timing(char* char_ptr)
{
    uint64_t t1, t2;
    uint64_t time;
#ifndef NO_FLUSH
    t1 = rdcycle();
    fence_rwrw();
    flush(char_ptr+0);               //   clflush (%%rcx)
    fence_rwrw();
#if ON_MULTIPLE_LINES
    flush(char_ptr+64);
    fence_rwrw();
    flush(char_ptr+448);
    fence_rwrw();
    flush(char_ptr+256);
    fence_rwrw();
    flush(char_ptr+384);
    fence_rwrw();
    flush(char_ptr+320);
    fence_rwrw();
    flush(char_ptr+192);
    fence_rwrw();
    flush(char_ptr+128);
    fence_rwrw();
#endif // ON_MULTIPLE_LINES
    t2 = rdcycle();
    time = t2-t1;
#endif // !NO_FLUSH
    return time;
}

__attribute__((always_inline))
static inline void mflush_mfenced(char* char_ptr)
{
#ifndef NO_FLUSH
    fence_rwrw();
    flush(char_ptr+0);               //   clflush (%%rcx)
    fence_rwrw();
#if ON_MULTIPLE_LINES
    flush(char_ptr+64);
    fence_rwrw();
    flush(char_ptr+448);
    fence_rwrw();
    flush(char_ptr+256);
    fence_rwrw();
    flush(char_ptr+384);
    fence_rwrw();
    flush(char_ptr+320);
    fence_rwrw();
    flush(char_ptr+192);
    fence_rwrw();
    flush(char_ptr+128);
    fence_rwrw();
#endif // ON_MULTIPLE_LINES
#endif // !NO_FLUSH
}


__attribute__((always_inline))
static inline void mflush(char* char_ptr)
{
#ifndef NO_FLUSH
    fence_rwrw();
    flush(char_ptr+0);
#if ON_MULTIPLE_LINES
    flush(char_ptr+64);
    flush(char_ptr+448);
    flush(char_ptr+256);
    flush(char_ptr+384);
    flush(char_ptr+320);
    flush(char_ptr+192);
    flush(char_ptr+128);
#endif // ON_MULTIPLE_LINES
    fence_rwrw();
#endif // !NO_FLUSH
}

#endif // !__ASM_FUNCTIONS_H
