#
# This file implements cache three-step simulator for deriving vulnerability types
#
# Copyright (C) 2020 Shuwen Deng <shuwen.deng@yale.edu>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
#
#

import sys
import collections

if ((len(sys.argv))<=1):
    print ("Please add number after the python script to indicate which type of reduced three-step patterns you want:")
    print ("0 for Strong type, 2 for Weak type, 1 for ineffective type")
    exit(1)
TYPE=int(str(sys.argv[1]))
# 0 for Strong type, 2 for Weak type, 1 for ineffective type

out_filename = "eff_attacks_cache_simu.txt"
F_out1= open(out_filename, "wb")

states = ["Vuu", "Aaa", "Vaa", "Abb", "Vbb", "Add", "Vdd", "Ainv", "Vinv", "Akainv", "Vkainv", "Apbinv", "Vpbinv",  "Aqdinv", "Vqdinv", "Vuinv", "***", "NOP"]

# memory addresses:
# - "x": set of sensitive memory locations
# - "u": a memory address within the set "x" and unknown to the attacker
# - "a": a memory address within the set "x" and known to the attacker
# - "a^alias": a memory address within the set "x", not the same as "a" but
#              mapping to the same cache block as "a", and known to the
#              attacker
# - "d": a memory address outside the range "x" and known to the attacker
options ={"Vuu": 0, # the victim sets the cache block to "u"
        "Aaa": 1, # the attacker sets the cache block to "a"
        "Vaa": 2, # the victim sets the cache block to "a"
        "Abb": 3, # the attacker sets the cache block to "a^alias"
        "Vbb": 4, # the victim sets the cache block to "a^alias"
        "Add": 5, # the attacker sets the cache block to "d"
        "Vdd": 6, # the victim sets the cache block to "d"
        "Ainv": 7, # the attacker removes the data and its address from the
                   # cache block, as a result of cache block invalidation
        "Vinv": 8, # the victim removes the data and its address from the
                   # cache block, as a result of cache block invalidation
        "Akainv": 9, # the attacker removes the data and its address "a" from
                     # the cache block, i.e. it can be anything except "a"
        "Vkainv": 10, # the victim removes the data and its address "a" from
                      # the cache block, i.e. it can be anything except "a"
        "Apbinv": 11, # the attacker removes the data and its address "a^alias"
                      # from the cache block; it can be anything but "a^alias"
        "Vpbinv": 12, # the victim removes the data and its address "a^alias"
                      # from the cache block; it can be anything but "a^alias"
        "Aqdinv": 13, # the attacker removes the data and its address "d" from
                      # the cache block, i.e. it can be anything except "d"
        "Vqdinv": 14, # the victim removes the data and its address "d" from
                      # the cache block, i.e. it can be anything except "d"
        "Vuinv": 15, # the victim removes the data and its address "u" from
                     # the cache block, i.e. it can be anything except "u"
        "***": 16, # the cache block state may contain any data or no data,
                   # and content is unknown by attacker
        "NOP": 17
        }
 
# Correspondence to paper's states in Table 1 (in Latex)
# Vuu -> $V_u$
# Aaa -> $A_a$
# Vaa -> $V_a$
# Abb -> $A_{a^{alias}}$
# Vbb -> $V_{a^{alias}}$
# Add -> $A_d$
# Vdd -> $V_d$
# Ainv -> $A^{inv}$
# Vinv -> $V^{inv}$
# Akainv -> $A_a^{inv}$
# Vkainv -> $V_a^{inv}$
# Apbinv -> $A_{a^{alias}}^{inv}$
# Vpbinv -> $V_{a^{alias}}^{inv}$
# Aqdinv -> $A_d^{inv}$
# Vqdinv -> $V_d^{inv}$
# Vuinv -> $V_u^{inv}$
# *** -> $\star$
# NOP -> $NIB$

# Three different types of three-step patterns
STRONG=0
NOATT=1
WEAK=2



# Timing list, can be reconfigured to other timing lists suited for different coherence models.
# DONT_CARE represents the case of measuring timing of *** or Ainv/Vinv, which has no value for measurement.
# FAST_SLOW represents the case that some indeterministic state exist for that cache block that makes the timing measurement uncertain with given states.
# for rest of the timings, it corresponds to the 66 types of timings in Figure 1 of the paper.
# l -> read (load), s -> write (store), and f -> flush 
# L1, L2, L3 -> L1 data cache, L2 cache, last-level cache
# c -> clean, d -> dirty
# r -> remote

DONT_CARE=0
FAST_SLOW=1

l_L1c=2
l_L1d=3
l_L2c=4
l_L2d=5
l_L3c=6
l_L3d=7
l_rL1c=8
l_rL1d=9
l_rL2c=10
l_rL2d=11
l_rL3c=12
l_rL3d=13
l_L1rL1c=14
l_L1rL2c=15
l_L1rL3c=16
l_L2rL1c=17
l_L2rL2c=18
l_L2rL3c=19
l_L3rL1c=20
l_L3rL2c=21
l_L3rL3c=22
l_DRAM=23


s_L1c=24
s_L1d=25
s_L2c=26
s_L2d=27
s_L3c=28
s_L3d=29
s_rL1c=30
s_rL1d=31
s_rL2c=32
s_rL2d=33
s_rL3c=34
s_rL3d=35
s_L1rL1c=36
s_L1rL2c=37
s_L1rL3c=38
s_L2rL1c=39
s_L2rL2c=40
s_L2rL3c=41
s_L3rL1c=42
s_L3rL2c=43
s_L3rL3c=44
s_DRAM=45


f_L1c=46
f_L1d=47
f_L2c=48
f_L2d=49
f_L3c=50
f_L3d=51
f_rL1c=52
f_rL1d=53
f_rL2c=54
f_rL2d=55
f_rL3c=56
f_rL3d=57
f_L1rL1c=58
f_L1rL2c=59
f_L1rL3c=60
f_L2rL1c=61
f_L2rL2c=62
f_L2rL3c=63
f_L3rL1c=64
f_L3rL2c=65
f_L3rL3c=66
f_DRAM=67

NO_1=68
NO_2=69


# Python program to illustrate the intersection of two lists
def intersection(lst1, lst2):
    lst3 = [value for value in lst1 if value in lst2]
    return lst3


def num(in_value):
    tmp = options[in_value]
    return tmp

def u_related(in_value):
    seri = options[in_value]
    relate = (seri==15) or (seri==0) # Vuinv and Vu
    return relate

def change_u(in_value, number):
    candidate_u = ["Vaa", "Vbb", "NOPu"]
    candidate_uinv = ["Vkainv", "Vpbinv", "NOPuinv"]
    seri = options[in_value]
    if seri==15:
        return candidate_uinv[number]
    elif seri==0:
        return candidate_u[number]

def fast_or_slow(step0, step1, step2):
    if ("**" in step2):
        return [DONT_CARE]
    elif ("Ainv" in step2) or ("Vinv" in step2):
        return [DONT_CARE]
    elif ("NOPuinv" in step2):
        if ("**" in step1):
            return [FAST_SLOW]
        elif ("NOPuinv" in step1):
            return [s_L1d, s_DRAM, f_L1d, f_DRAM]
        elif ("Ainv" in step1) or ("Vinv" in step1):
            return [s_DRAM, f_DRAM, NO_2]
        elif ("NOPu" == step1):
            return [s_rL1d, s_rL1c, s_L1rL1c, s_L2rL1c, s_L3rL1c, f_L1d, f_L1c, f_L1rL1c, f_L1rL2c, f_L1rL3c]
        else:
            if ("NOPuinv" in step0):
                return [s_L1d, s_DRAM, f_L1d, f_DRAM]
            elif ("NOPu" == step0):
                return [s_rL1d, s_rL1c, s_L1rL1c, s_L2rL1c, s_L3rL1c, f_L1d, f_L1c, f_L1rL1c, f_L1rL2c, f_L1rL3c]
            elif ("Ainv" in step0) or ("Vinv" in step0):
                return [s_DRAM, f_DRAM, NO_2]
            else:
                return [FAST_SLOW]
    elif ("NOPu" == step2):
        if ("**" in step1):
            return [FAST_SLOW]
        elif ("NOPu" == step1):
            return [l_L1d, l_L1c, l_L1rL1c, l_L1rL2c, l_L1rL3c, s_L1d, s_L1c, s_L1rL1c, s_L1rL2c, s_L1rL3c]
        elif ("NOPuinv" in step1):
            return [l_rL1d, l_DRAM, s_rL1d, s_DRAM]
        elif ("Ainv" in step1) or ("Vinv" in step1):
            return [l_DRAM, s_DRAM, NO_2]
        else:
            if ("NOPu" == step0):
                return [l_L1d, l_L1c, l_L1rL1c, l_L1rL2c, l_L1rL3c, s_L1d, s_L1c, s_L1rL1c, s_L1rL2c, s_L1rL3c]
            elif ("NOPuinv" in step0):
                return [l_rL1d, l_DRAM, s_rL1d, s_DRAM]
            elif ("Ainv" in step0) or ("Vinv" in step0):
                return [l_DRAM, s_DRAM, NO_2]
            else:
                return [FAST_SLOW]
    elif ("dd" in step2) or ("aa" in step2) or ("bb" in step2):
        tmp = step2[1:3]
        tmp_inv = step2[1:2]+"inv"
        tmp1 = ""
        tmp2 = ""
        for x in ["aa", "bb", "dd"]:
            if (x != tmp and tmp1==""):
                tmp1 = x
            elif (x != tmp and tmp2==""):
                tmp2 = x
        tmp1_inv = tmp1[0]+"inv"
        tmp2_inv = tmp2[0]+"inv"
        if ("**" in step1):
            return [FAST_SLOW]
        elif (tmp in step1):
            return [l_L1d, l_L1c, l_L1rL1c, l_L1rL2c, l_L1rL3c, s_L1d, s_L1c, s_L1rL1c, s_L1rL2c, s_L1rL3c]
        elif (tmp_inv in step1):
            return [l_rL1d, l_DRAM, s_rL1d, s_DRAM]
        elif ("Ainv" in step1) or ("Vinv" in step1):
            return [l_DRAM, s_DRAM, NO_2]
        elif (tmp1 in step1) or (tmp2 in step1):
            if ("**" in step0) or ("NOPu" == step0) or ("NOPuinv" in step0) or (tmp1_inv in step0) or (tmp2_inv in step0):
                return [FAST_SLOW]
            elif (tmp_inv in step0):
                return [l_rL1d, l_DRAM, s_rL1d, s_DRAM]
            elif ("Ainv" in step0) or ("Vinv" in step0):
                return [l_DRAM, s_DRAM, NO_2]
            elif (tmp1 in step0) or (tmp2 in step0):
                return [FAST_SLOW] 
            elif (tmp in step0):
                return [l_L2c, l_L2rL1c, l_L2rL2c, l_L2rL3c, l_DRAM, s_L2c, s_L2rL1c, s_L2rL2c, s_L2rL3c, s_DRAM]
        else:
            if ("**" in step0) or ("NOPu" == step0) or ("NOPuinv" in step0) or (tmp1_inv in step0) or (tmp2_inv in step0):
                return [FAST_SLOW]
            elif (tmp_inv in step0):
                if (tmp1_inv in step1 or tmp2_inv in step1):
                    return [l_DRAM, s_DRAM, NO_1]
                else:
                    return [l_rL1d, l_DRAM, s_rL1d, s_DRAM]
            elif ("Ainv" in step0) or ("Vinv" in step0):
                return [l_DRAM, s_DRAM, NO_2]
            elif (tmp1 in step0) or (tmp2 in step0):
                return [FAST_SLOW] 
            elif (tmp in step0):
                return [l_L1d, l_L1c, l_L1rL1c, l_L1rL2c, l_L1rL3c, s_L1d, s_L1c, s_L1rL1c, s_L1rL2c, s_L1rL3c]
    elif ("dinv" in step2) or ("ainv" in step2) or ("binv" in step2):
        tmp = step2[-4]*2
        tmp_inv = step2[-4]+"inv"
        tmp1 = ""
        tmp2 = ""
        for x in ["aa", "bb", "dd"]:
            if (x != tmp and tmp1==""):
                tmp1 = x
            elif (x != tmp and tmp2==""):
                tmp2 = x
        tmp1_inv = tmp1[0]+"inv"
        tmp2_inv = tmp2[0]+"inv"
        if ("**" in step1):
            return [FAST_SLOW]
        elif (tmp in step1):
            return [s_rL1d, s_rL1c, s_L1rL1c, s_L2rL1c, s_L3rL1c, f_L1d, f_L1c, f_L1rL1c, f_L1rL2c, f_L1rL3c]
        elif (tmp_inv in step1):
            return [s_L1d, s_DRAM, f_L1d, f_DRAM]
        elif ("Ainv" in step1) or ("Vinv" in step1):
            return [s_DRAM, f_DRAM, NO_2]
        elif (tmp1 in step1) or (tmp2 in step1):
            if ("**" in step0):
                return [FAST_SLOW]
            elif ("NOPu" == step0) or ("NOPuinv" in step0) or (tmp1_inv in step0) or (tmp2_inv in step0) or (tmp1 in step0) or (tmp2 in step0):
                return [FAST_SLOW]
            elif (tmp_inv in step0):
                return [s_L1d, s_DRAM, f_L1d, f_DRAM]
            elif ("Ainv" in step0) or ("Vinv" in step0):
                return [s_DRAM, f_DRAM, NO_2]
            elif (tmp in step0): 
                return [s_rL2c, s_L1rL2c, s_L2rL2c, s_L3rL2c, s_DRAM, f_L2c, f_L2rL1c, f_L2rL2c, f_L2rL3c, f_DRAM]
        else:
            if ("**" in step0) or ("NOPu" == step0) or ("NOPuinv" in step0) or (tmp1_inv in step0) or (tmp2_inv in step0):
                return [FAST_SLOW]
            elif (tmp1 in step0) or (tmp2 in step0):
                return [FAST_SLOW]
            elif (tmp_inv in step0):
                if (tmp1_inv in step1 or tmp2_inv in step1):
                    return [s_DRAM, f_DRAM, NO_1]
                else:
                    return [s_L1d, s_DRAM, f_L1d, f_DRAM]
            elif ("Ainv" in step0) or ("Vinv" in step0):
                return [s_DRAM, f_DRAM, NO_2]
            elif (tmp in step0):
                return [s_rL1d, s_rL1c, s_L1rL1c, s_L2rL1c, s_L3rL1c, f_L1d, f_L1c, f_L1rL1c, f_L1rL2c, f_L1rL3c]




candidate = []
total_length = len(states)

numbering = -1
count = 1
no_vu_count = 0
vu_count = 0
strong_array = []
weak_array = []
for i in range(total_length-1):
    for j in range (total_length-1):  
        for m in range (total_length-1): 
            numbering = numbering + 1
            steps = [states[i], states[j], states[m]]
            attack_level = NOATT
            possi = []
            res = []
            timing = []
            contain_dd = False
            contain_dinv = False
            for y in range(3):
                if "dd" in steps[y]:
                    contain_dd = True
                if "dinv" in steps[y]:
                    contain_dinv = True
            if (u_related(steps[0]) or u_related(steps[1]) or u_related(steps[2])):
                for q in range(3):
                    tmpsteps = [states[i], states[j], states[m]]
                    for p in range(3):
                        if u_related(steps[p]):
                            tmpsteps[p] = change_u(steps[p], q)
                    possi.append(tmpsteps)
            if possi==[]:
                no_vu_count = no_vu_count + 1
                continue
            else:
                vu_count = vu_count + 1
                for q in range(3):
                    res.append(fast_or_slow(possi[q][0], possi[q][1], possi[q][2]))
                if (res.count(res[0])==1 or res.count(res[1])==1 or res.count(res[2])==1) and ([FAST_SLOW] not in res):
                    attack_level = STRONG
                elif not(res.count(res[0])==3 or res.count(res[1])==3 or res.count(res[2])==3):
                    attack_level = WEAK
                else:
                    attack_level = NOATT #UNKNOWN

                if attack_level == TYPE:
                    cutList = []
                    cutList = cutList + [states[i]!=states[j]]
                    cutList = cutList + [states[j]!=states[m]]
                    ##### not two adjacent certain known accesses
                    cutList = cutList + [((not(((states[i][1]=="u") and (not(states[j][1]=="u")) and (not(states[m][1]=="u"))) or ((not(states[i][1]=="u")) and (not(states[j][1]=="u")) and (states[m][1]=="u"))))   or  ((states[i]=="***" and (states[j][1]=="u")) or (states[i]=="***" and (states[m][1]=="u"))))]
                    ##### before triggering Vu/Vuinv, the condition of this cache block must be known
                    ##### Vuinv is because when getting hit, mapping address must not be Vu, so information for Vu is got.
                    ##### limitation to same-within-x "b"
                    cutList = cutList + [not((states[i][2]!="a" and states[j][2]!="a" and states[m][2]!="a") and (states[i][2]=="b" or states[j][2]=="b" or states[m][2]=="b"))]
                    cutList = cutList + [not((states[i][2]=="a" and (states[j][2]=="b" or states[m][2]=="b")) or (states[j][2]=="a" and states[m][2]=="b"))]
                    ##### Vu and Vuinv shall not be adjacent, being canceled for each other
                    cutList = cutList + [(not((states[i]=="Vuu" and states[j]=="Vuinv") or (states[i]=="Vuinv" and states[j]=="Vuu") or (states[j]=="Vuu" and states[m]=="Vuinv") or (states[j]=="Vuinv" and states[m]=="Vuu")))]
                    cut = True
                    for n in range (len(cutList)):
                        cut = cut and cutList[n] 
                    if (cut and not(res[0]==DONT_CARE and res[1]==DONT_CARE and res[2]==DONT_CARE)):
                        print(["Three-step pattern: "] + steps)
                        print (["Different cases: "] + possi)
                        print (["Corresponding timings: "] + res)
                        print("Count:" + str(count) + "\t" + "Numbering:" + str(numbering))
                        strong_array.append(numbering)
                        line = str(count) + "\t\t\t" + states[i] + "\t\t" + states[j] + "\t\t" + states[m] + "\n"
                        count = count + 1

print ("Type numbers:")
print (strong_array)
print ("Total type counts:")
print (len(strong_array))


