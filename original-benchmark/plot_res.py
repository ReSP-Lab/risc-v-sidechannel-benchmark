#
# This file plot benchmark running results for 9 machine configurations
#
# Copyright (C) 2020
# Author: Shuwen Deng <shuwen.deng@yale.edu>
# Date:   02.04.2020
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


import matplotlib.pyplot as plt
import csv
import sys

params = {'legend.fontsize': 15,
          'legend.handlelength': 2,
          #'legend.frameon': False,
          'axes.titlesize'      : 18,
          'axes.labelsize'      : 15}
plt.rcParams.update(params)

if ((len(sys.argv))<=1):
	print ("Please add string after the python script to indicate machine and running information as the naming of the dot diagrams")
	exit(1)

vul_num=88
x = []

y_wkst10_dup = []
y_wkst5_dup = []
y_srv1_dup = []
y_srv2_onchip_dup = []
y_srv2_interchip_dup = []
y_amd_core4_dup = []
y_broadwell_dup = []
y_skylake_dup = []
y_amd_local_dup = []
y_and_dup = []
y_or_dup = []


y_wkst10 = []
y_wkst5 = []
y_srv1 = []
y_srv2_onchip = []
y_srv2_interchip = []
y_amd_core4 = []
y_broadwell = []
y_skylake = []
y_amd_local = []
y_and = []
y_or = []

y_wkst10_1 = []
y_wkst5_1 = []
y_srv1_1 = []
y_srv2_onchip_1 = []
y_srv2_interchip_1 = []
y_amd_core4_1 = []
y_broadwell_1 = []
y_skylake_1 = []
y_amd_local_1 = []
y_and_1 = []
y_or_1 = []

y_wkst10_2 = []
y_wkst5_2 = []
y_srv1_2 = []
y_srv2_onchip_2 = []
y_srv2_interchip_2 = []
y_amd_core4_2 = []
y_broadwell_2 = []
y_skylake_2 = []
y_amd_local_2 = []
y_and_2 = []
y_or_2 = []

x_1=[]
x_2=[]
x_single_1 = []
x_single_2 = []

outer_x=[]
outer_grids = []
grids = []
grids_2 = []

outer_counter=0
counter=0
step=0

x_single = []
y_wkst10_single = []
y_wkst5_single = []
y_srv1_single = []
y_srv2_onchip_single = []
y_srv2_interchip_single = []
y_amd_core4_single = []
y_broadwell_single = []
y_skylake_single = []
y_amd_local_single = []
y_and_single = []
y_or_single = []

y_read=[[], [], [], [], [], [], [], [], [], [], []] 
y_write=[[], [], [], [], [], [], [], [], [], [], []]
y_ccinv=[[], [], [], [], [], [], [], [], [], [], []]
y_clflush=[[], [], [], [], [], [], [], [], [], [], []]
y_hyper=[[], [], [], [], [], [], [], [], [], [], []]
y_time=[[], [], [], [], [], [], [], [], [], [], []]
y_inner=[[], [], [], [], [], [], [], [], [], [], []]
y_hasatt=[[], [], [], [], [], [], [], [], [], [], []]
x_read=[]
grids_read=[]
x_write=[]
grids_write=[]
x_ccinv=[]
grids_ccinv=[]
x_clflush=[]
grids_clflush=[]
x_hyper=[]
grids_hyper=[]
x_time=[]
grids_time=[]
x_inner=[]
grids_inner=[]
x_hasatt=[]
grids_hasatt=[]

with open('benchmark_output/wkst10_gen_v16_recon_fix.output.res.tran','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
        if row!=[]:
        	if(row==['222']):
        		y_wkst10_dup.append(-2)
        	elif(int(row[0], 2)==2):
        		y_wkst10.append(1+step)
        		y_wkst10_dup.append(1+step)
        	else:
        		y_wkst10.append(0+step)
        		y_wkst10_dup.append(0+step)
step = step +2

with open('benchmark_output/srv2_onchip_gen_v17_recon_fix.output.res.tran','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
        if row!=[]:
        	if(row==['222']):
        		y_srv2_onchip_dup.append(-2)
        	elif(int(row[0], 2)==2):
        		y_srv2_onchip.append(1+step)
        		y_srv2_onchip_dup.append(1+step)
        	else:
        		y_srv2_onchip.append(0+step)
        		y_srv2_onchip_dup.append(0+step)
step = step +2

with open('benchmark_output/srv2_interchip_gen_v17_recon_fix.output.res.tran','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
        if row!=[]:
        	if(row==['222']):
        		y_srv2_interchip_dup.append(-2)
        	elif(int(row[0], 2)==2):
        		y_srv2_interchip.append(1+step)
        		y_srv2_interchip_dup.append(1+step)
        	else:
        		y_srv2_interchip.append(0+step)
        		y_srv2_interchip_dup.append(0+step)
step = step +2

with open('benchmark_output/srv1_gen_v16_recon_fix.output.res.tran','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
    	if row==[]:
        	grids.append(counter)
        	outer_grids.append(outer_counter)
        else:
        	if(row==['222']):
        		y_srv1_dup.append(-2)
        	elif(int(row[0], 2)==2):
        		y_srv1.append(1+step)
        		y_srv1_dup.append(1+step)
        		x.append(counter)
        		counter = counter + 1
        	else:
        		y_srv1.append(0+step)
        		y_srv1_dup.append(0+step)
        		x.append(counter)
        		counter = counter + 1
        	outer_x.append(outer_counter)
        	outer_counter = outer_counter + 1
step = step +2


with open('benchmark_output/wkst5_gen_v16_recon_fix.output.res.tran','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
        if row==['111']:
        	y_wkst5.append(0+step)#-1)
        	y_wkst5_dup.append(0+step)#-1)
        elif row!=[]:
        	if(row==['222']):
        		y_wkst5_dup.append(-2)
        	elif(int(row[0], 2)==2):
        		y_wkst5.append(1+step)
        		y_wkst5_dup.append(1+step)
        	else:
        		y_wkst5.append(0+step)
        		y_wkst5_dup.append(0+step)
step = step +2


with open('benchmark_output/broadwell_gen_v16_recon_fix.output.res.tran','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
        if row==['111']:
        	y_broadwell.append(0+step)#-1)
        	y_broadwell_dup.append(0+step)#-1)
        elif row!=[]:
        	if(row==['222']):
        		y_broadwell_dup.append(-2)
        	elif(int(row[0], 2)==2):
        		y_broadwell.append(1+step)
        		y_broadwell_dup.append(1+step)
        	else:
        		y_broadwell.append(0+step)
        		y_broadwell_dup.append(0+step)
step = step +2



with open('benchmark_output/skylake_gen_v16_recon_fix.output.res.tran','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
    	if row!=[]:
        	if(row==['222']):
        		y_skylake_dup.append(-2)
        	elif(int(row[0], 2)==2):
        		y_skylake.append(1+step)
        		y_skylake_dup.append(1+step)
        	else:
        		y_skylake.append(0+step)
        		y_skylake_dup.append(0+step)
step = step +2

with open('benchmark_output/amd_local_gen_v16_recon_fix.output.res.tran','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
        if row==['111']:
        	y_amd_local.append(0+step)#-1)
        	y_amd_local_dup.append(0+step)#-1)
        elif row!=[]:
        	if(row==['222']):
        		y_amd_local_dup.append(-2)
        	elif(int(row[0], 2)==2):
        		y_amd_local.append(1+step)
        		y_amd_local_dup.append(1+step)
        	else:
        		y_amd_local.append(0+step)
        		y_amd_local_dup.append(0+step)
step = step +2


with open('benchmark_output/amd_amazon_gen_v16_recon_fix.output.res.tran','r') as csvfile:
    plots = csv.reader(csvfile, delimiter=' ')
    for row in plots:
    	if row!=[]:
        	if(row==['222']):
        		y_amd_core4_dup.append(-2)
        	elif(int(row[0], 2)==2):
        		y_amd_core4.append(1+step)
        		y_amd_core4_dup.append(1+step)
        	else:
        		y_amd_core4.append(0+step)
        		y_amd_core4_dup.append(0+step)
step = step +2






#print outer_counter, counter, len(y_wkst10), len(y_wkst5), len(y_srv1), len(y_srv2_onchip), len(y_srv2_interchip)
for i in range(counter):
	if (y_wkst5[i]==-1):
		y_and.append(((y_srv1[i]-6) and (y_srv2_onchip[i]-2) and (y_srv2_interchip[i]-4) and (y_wkst10[i]-0) and (y_amd_core4[i]-16) and (y_skylake[i]-12) ) + step) # and (y_wkst5[i]-2)
	else:
		y_and.append(((y_wkst5[i]-8) and (y_srv1[i]-6) and (y_srv2_onchip[i]-2) and (y_srv2_interchip[i]-4) and (y_wkst10[i]-0) and (y_amd_core4[i]-16) and (y_broadwell[i]-10) and (y_skylake[i]-12) and (y_amd_local[i]-14)) + step)
for i in range(outer_counter):
	if (y_wkst5_dup[i]==-2):
		y_and_dup.append(-5)
	else:
		y_and_dup.append(((y_wkst5_dup[i]-8) and (y_srv1_dup[i]-6) and (y_srv2_onchip_dup[i]-2) and (y_srv2_interchip_dup[i]-4) and (y_wkst10_dup[i]-0) and (y_amd_core4_dup[i]-16) and (y_broadwell_dup[i]-10) and (y_skylake_dup[i]-12) and (y_amd_local_dup[i]-14)) + step)
step = step +2

	
for i in range(counter):
	if (y_wkst5[i]==-1):
		y_or.append(((y_srv1[i]-6) or (y_srv2_onchip[i]-2) or (y_srv2_interchip[i]-4) or (y_wkst10[i]-0) or (y_amd_core4[i]-16) or (y_skylake[i]-12) ) + step) # or (y_wkst5[i]-2)
	else:
		y_or.append(((y_wkst5[i]-8) or (y_srv1[i]-6) or (y_srv2_onchip[i]-2) or (y_srv2_interchip[i]-4) or (y_wkst10[i]-0) or (y_amd_core4[i]-16) or (y_broadwell[i]-10) or (y_skylake[i]-12) or (y_amd_local[i]-14)) + step) # or (y_wkst5[i]-2)
for i in range(outer_counter):
	if (y_wkst5_dup[i]==-2):
		y_or_dup.append(-5)
	else:
		y_or_dup.append(((y_wkst5_dup[i]-8) or (y_srv1_dup[i]-6) or (y_srv2_onchip_dup[i]-2) or (y_srv2_interchip_dup[i]-4) or (y_wkst10_dup[i]-0) or (y_amd_core4_dup[i]-16) or (y_broadwell_dup[i]-10) or (y_skylake_dup[i]-12) or (y_amd_local_dup[i]-14)) + step)
step = step +2

#print grids


for i in range(554):
	y_wkst10_1.append(y_wkst10[i])
	y_wkst5_1.append(y_wkst5[i])
	y_srv1_1.append(y_srv1[i])
	y_srv2_onchip_1.append(y_srv2_onchip[i])
	y_srv2_interchip_1.append(y_srv2_interchip[i])
	y_amd_core4_1.append(y_amd_core4[i])
	y_broadwell_1.append(y_broadwell[i])
	y_skylake_1.append(y_skylake[i])
	y_amd_local_1.append(y_amd_local[i])
	y_and_1.append(y_and[i])
	y_or_1.append(y_or[i])
	x_1.append(x[i])

for i in range(554, 1094):
	y_wkst10_2.append(y_wkst10[i])
	y_wkst5_2.append(y_wkst5[i])
	y_srv1_2.append(y_srv1[i])
	y_srv2_onchip_2.append(y_srv2_onchip[i])
	y_srv2_interchip_2.append(y_srv2_interchip[i])
	y_amd_core4_2.append(y_amd_core4[i])
	y_broadwell_2.append(y_broadwell[i])
	y_skylake_2.append(y_skylake[i])
	y_amd_local_2.append(y_amd_local[i])
	y_and_2.append(y_and[i])
	y_or_2.append(y_or[i])
	x_2.append(x[i]-554)

for i in range(vul_num/2):
	x_single_1.append(i+1)
	x_single_2.append(i+vul_num/2+1)
	grids_2.append(grids[i+vul_num/2]-554)

inner_counter=0
inner_bound = grids[0]
for i in range(vul_num):
	x_single.append(i)
	y_wkst5_single.append(y_wkst5[inner_counter]-8)
	y_srv1_single.append(y_srv1[inner_counter]-6)
	y_srv2_onchip_single.append(y_srv2_onchip[inner_counter]-2)
	y_srv2_interchip_single.append(y_srv2_interchip[inner_counter]-4)
	y_wkst10_single.append(y_wkst10[inner_counter]-0)
	y_amd_core4_single.append(y_amd_core4[inner_counter]-16)
	y_broadwell_single.append(y_broadwell[inner_counter]-10)
	y_skylake_single.append(y_skylake[inner_counter]-12)
	y_amd_local_single.append(y_amd_local[inner_counter]-14)
	inner_counter = inner_counter + 1
	if i!=0:
		inner_bound = grids[i] - grids[i-1]
	for j in range(inner_bound-1):
		if y_wkst5[inner_counter]!=-1:
			y_wkst5_single[i] = y_wkst5_single[i] or (y_wkst5[inner_counter]-8)
			y_broadwell_single[i] = y_broadwell_single[i] or (y_broadwell[inner_counter]-10)
			y_amd_local_single[i] = y_amd_local_single[i] or (y_amd_local[inner_counter]-14)
		y_srv1_single[i] = y_srv1_single[i] or (y_srv1[inner_counter]-6)
		y_srv2_onchip_single[i] = y_srv2_onchip_single[i] or (y_srv2_onchip[inner_counter]-2)
		y_srv2_interchip_single[i] = y_srv2_interchip_single[i] or (y_srv2_interchip[inner_counter]-4)
		y_wkst10_single[i] = y_wkst10_single[i] or (y_wkst10[inner_counter]-0)
		y_amd_core4_single[i] = y_amd_core4_single[i] or (y_amd_core4[inner_counter]-16)
		y_skylake_single[i] = y_skylake_single[i] or (y_skylake[inner_counter]-12)
		inner_counter = inner_counter + 1
	y_and_single.append(((y_wkst5_single[i]) and (y_srv1_single[i]) and (y_srv2_onchip_single[i]) and (y_srv2_interchip_single[i]) and y_wkst10_single[i] and y_amd_core4_single[i] and y_broadwell_single[i] and y_skylake_single[i] and y_amd_local_single[i]) + 18)
	y_or_single.append(((y_wkst5_single[i]) or (y_srv1_single[i]) or (y_srv2_onchip_single[i]) or (y_srv2_interchip_single[i]) or y_wkst10_single[i] or y_amd_core4_single[i] or y_broadwell_single[i] or y_skylake_single[i] or y_amd_local_single[i]) + 20)
	y_wkst5_single[i] = y_wkst5_single[i] + 8
	y_srv1_single[i] = y_srv1_single[i] + 6
	y_srv2_onchip_single[i] = y_srv2_onchip_single[i] + 2
	y_srv2_interchip_single[i] = y_srv2_interchip_single[i] + 4
	y_wkst10_single[i] = y_wkst10_single[i] + 0
	y_amd_core4_single[i] = y_amd_core4_single[i] + 16
	y_broadwell_single[i] = y_broadwell_single[i] + 10
	y_skylake_single[i] = y_skylake_single[i] + 12
	y_amd_local_single[i] = y_amd_local_single[i] + 14

#print outer_grids
#print vul_num

# read
inner_counter=0
inner_bound = outer_grids[0]
for i in range(88/2):
	grids_read.append(outer_grids[i]/2)
	if i!=0:
		inner_bound = outer_grids[i] - outer_grids[i-1]
	if (inner_bound==8):
		for j in range(inner_bound/2):
			x_read.append(inner_counter/2)
			y_read[4].append(y_wkst5_dup[inner_counter])
			y_read[3].append(y_srv1_dup[inner_counter])
			y_read[1].append(y_srv2_onchip_dup[inner_counter])
			y_read[2].append(y_srv2_interchip_dup[inner_counter])
			y_read[0].append(y_wkst10_dup[inner_counter])
			y_read[8].append(y_amd_core4_dup[inner_counter])
			y_read[5].append(y_broadwell_dup[inner_counter])
			y_read[6].append(y_skylake_dup[inner_counter])
			y_read[7].append(y_amd_local_dup[inner_counter])
			y_read[9].append(y_and_dup[inner_counter])
			y_read[10].append(y_or_dup[inner_counter])
			inner_counter = inner_counter + 1+1
	elif (inner_bound==16):
		for j in range(inner_bound/4):
			x_read.append(inner_counter/2)
			x_read.append(inner_counter/2+1)
			y_read[4].extend((y_wkst5_dup[inner_counter], y_wkst5_dup[inner_counter+1]))
			y_read[3].extend((y_srv1_dup[inner_counter], y_srv1_dup[inner_counter+1]))
			y_read[1].extend((y_srv2_onchip_dup[inner_counter], y_srv2_onchip_dup[inner_counter+1]))
			y_read[2].extend((y_srv2_interchip_dup[inner_counter], y_srv2_interchip_dup[inner_counter+1]))
			y_read[0].extend((y_wkst10_dup[inner_counter], y_wkst10_dup[inner_counter+1]))
			y_read[8].extend((y_amd_core4_dup[inner_counter], y_amd_core4_dup[inner_counter+1]))
			y_read[5].extend((y_broadwell_dup[inner_counter], y_broadwell_dup[inner_counter+1]))
			y_read[6].extend((y_skylake_dup[inner_counter], y_skylake_dup[inner_counter+1]))
			y_read[7].extend((y_amd_local_dup[inner_counter], y_amd_local_dup[inner_counter+1]))
			y_read[9].extend((y_and_dup[inner_counter], y_and_dup[inner_counter+1]))
			y_read[10].extend((y_or_dup[inner_counter], y_or_dup[inner_counter+1]))
			inner_counter = inner_counter + 2+2


# write
inner_counter=0
inner_bound = outer_grids[0]
for i in range(88/2):
	grids_write.append(outer_grids[i]/2)
	if i!=0:
		inner_bound = outer_grids[i] - outer_grids[i-1]
	if (inner_bound==8):
		for j in range(inner_bound/2):
			x_write.append(inner_counter/2)
			inner_counter = inner_counter + 1
			
			y_write[4].append(y_wkst5_dup[inner_counter])
			y_write[3].append(y_srv1_dup[inner_counter])
			y_write[1].append(y_srv2_onchip_dup[inner_counter])
			y_write[2].append(y_srv2_interchip_dup[inner_counter])
			y_write[0].append(y_wkst10_dup[inner_counter])
			y_write[8].append(y_amd_core4_dup[inner_counter])
			y_write[5].append(y_broadwell_dup[inner_counter])
			y_write[6].append(y_skylake_dup[inner_counter])
			y_write[7].append(y_amd_local_dup[inner_counter])
			y_write[9].append(y_and_dup[inner_counter])
			y_write[10].append(y_or_dup[inner_counter])
			inner_counter = inner_counter + 1
	elif (inner_bound==16):
		for j in range(inner_bound/4):
			x_write.append(inner_counter/2)
			x_write.append(inner_counter/2+1)
			inner_counter = inner_counter + 2
			
			y_write[4].extend((y_wkst5_dup[inner_counter], y_wkst5_dup[inner_counter+1]))
			y_write[3].extend((y_srv1_dup[inner_counter], y_srv1_dup[inner_counter+1]))
			y_write[1].extend((y_srv2_onchip_dup[inner_counter], y_srv2_onchip_dup[inner_counter+1]))
			y_write[2].extend((y_srv2_interchip_dup[inner_counter], y_srv2_interchip_dup[inner_counter+1]))
			y_write[0].extend((y_wkst10_dup[inner_counter], y_wkst10_dup[inner_counter+1]))
			y_write[8].extend((y_amd_core4_dup[inner_counter], y_amd_core4_dup[inner_counter+1]))
			y_write[5].extend((y_broadwell_dup[inner_counter], y_broadwell_dup[inner_counter+1]))
			y_write[6].extend((y_skylake_dup[inner_counter], y_skylake_dup[inner_counter+1]))
			y_write[7].extend((y_amd_local_dup[inner_counter], y_amd_local_dup[inner_counter+1]))
			y_write[9].extend((y_and_dup[inner_counter], y_and_dup[inner_counter+1]))
			y_write[10].extend((y_or_dup[inner_counter], y_or_dup[inner_counter+1]))
			inner_counter = inner_counter + 2



# ccinv

inner_counter=outer_grids[vul_num/2-1]
for i in range(88/2, 88):
	grids_ccinv.append(outer_grids[i]/2)
	inner_bound = outer_grids[i] - outer_grids[i-1]
	if (inner_bound==8):
		for j in range(inner_bound/2):
			x_ccinv.append(inner_counter/2)
			inner_counter = inner_counter + 1
			
			y_ccinv[4].append(y_wkst5_dup[inner_counter])
			y_ccinv[3].append(y_srv1_dup[inner_counter])
			y_ccinv[1].append(y_srv2_onchip_dup[inner_counter])
			y_ccinv[2].append(y_srv2_interchip_dup[inner_counter])
			y_ccinv[0].append(y_wkst10_dup[inner_counter])
			y_ccinv[8].append(y_amd_core4_dup[inner_counter])
			y_ccinv[5].append(y_broadwell_dup[inner_counter])
			y_ccinv[6].append(y_skylake_dup[inner_counter])
			y_ccinv[7].append(y_amd_local_dup[inner_counter])
			y_ccinv[9].append(y_and_dup[inner_counter])
			y_ccinv[10].append(y_or_dup[inner_counter])
			inner_counter = inner_counter + 1
	elif (inner_bound==16):
		for j in range(inner_bound/4):
			x_ccinv.append(inner_counter/2)
			x_ccinv.append(inner_counter/2+1)
			
			y_ccinv[4].extend((y_wkst5_dup[inner_counter], y_wkst5_dup[inner_counter+1]))
			y_ccinv[3].extend((y_srv1_dup[inner_counter], y_srv1_dup[inner_counter+1]))
			y_ccinv[1].extend((y_srv2_onchip_dup[inner_counter], y_srv2_onchip_dup[inner_counter+1]))
			y_ccinv[2].extend((y_srv2_interchip_dup[inner_counter], y_srv2_interchip_dup[inner_counter+1]))
			y_ccinv[0].extend((y_wkst10_dup[inner_counter], y_wkst10_dup[inner_counter+1]))
			y_ccinv[8].extend((y_amd_core4_dup[inner_counter], y_amd_core4_dup[inner_counter+1]))
			y_ccinv[5].extend((y_broadwell_dup[inner_counter], y_broadwell_dup[inner_counter+1]))
			y_ccinv[6].extend((y_skylake_dup[inner_counter], y_skylake_dup[inner_counter+1]))
			y_ccinv[7].extend((y_amd_local_dup[inner_counter], y_amd_local_dup[inner_counter+1]))
			y_ccinv[9].extend((y_and_dup[inner_counter], y_and_dup[inner_counter+1]))
			y_ccinv[10].extend((y_or_dup[inner_counter], y_or_dup[inner_counter+1]))
			inner_counter = inner_counter + 2 +2

# clflush
inner_counter=outer_grids[vul_num/2-1]
for i in range(88/2, 88):
	grids_clflush.append(outer_grids[i]/2)
	inner_bound = outer_grids[i] - outer_grids[i-1]
	if (inner_bound==8):
		for j in range(inner_bound/2):
			x_clflush.append(inner_counter/2)
			y_clflush[4].append(y_wkst5_dup[inner_counter])
			y_clflush[3].append(y_srv1_dup[inner_counter])
			y_clflush[1].append(y_srv2_onchip_dup[inner_counter])
			y_clflush[2].append(y_srv2_interchip_dup[inner_counter])
			y_clflush[0].append(y_wkst10_dup[inner_counter])
			y_clflush[8].append(y_amd_core4_dup[inner_counter])
			y_clflush[5].append(y_broadwell_dup[inner_counter])
			y_clflush[6].append(y_skylake_dup[inner_counter])
			y_clflush[7].append(y_amd_local_dup[inner_counter])
			y_clflush[9].append(y_and_dup[inner_counter])
			y_clflush[10].append(y_or_dup[inner_counter])
			inner_counter = inner_counter + 1+1
	elif (inner_bound==16):
		for j in range(inner_bound/4):
			x_clflush.append(inner_counter/2)
			x_clflush.append(inner_counter/2+1)
			inner_counter = inner_counter + 2
			y_clflush[4].extend((y_wkst5_dup[inner_counter], y_wkst5_dup[inner_counter+1]))
			y_clflush[3].extend((y_srv1_dup[inner_counter], y_srv1_dup[inner_counter+1]))
			y_clflush[1].extend((y_srv2_onchip_dup[inner_counter], y_srv2_onchip_dup[inner_counter+1]))
			y_clflush[2].extend((y_srv2_interchip_dup[inner_counter], y_srv2_interchip_dup[inner_counter+1]))
			y_clflush[0].extend((y_wkst10_dup[inner_counter], y_wkst10_dup[inner_counter+1]))
			y_clflush[8].extend((y_amd_core4_dup[inner_counter], y_amd_core4_dup[inner_counter+1]))
			y_clflush[5].extend((y_broadwell_dup[inner_counter], y_broadwell_dup[inner_counter+1]))
			y_clflush[6].extend((y_skylake_dup[inner_counter], y_skylake_dup[inner_counter+1]))
			y_clflush[7].extend((y_amd_local_dup[inner_counter], y_amd_local_dup[inner_counter+1]))
			y_clflush[9].extend((y_and_dup[inner_counter], y_and_dup[inner_counter+1]))
			y_clflush[10].extend((y_or_dup[inner_counter], y_or_dup[inner_counter+1]))
			inner_counter = inner_counter + 2


# derived for differentiating victim, attacker running on the same (hyper-threading, time-slicing)/diff core, victim running alone cases
case_arr=[4,3,4,3,4,3,4,3,0,2,0,2,0,2,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,4,3,4,3,0,2,0,2,0,2,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,0,2,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,0,2,4,3,4,3,0,2,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,2,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,2,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,2,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,2,4,3,0,2,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,4,3,4,3,0,2,4,3,0,2,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,4,3,4,3,4,3,0,2,4,3,0,2,4,3,0,2,4,3,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,2,4,3,0,2,4,3,0,2,4,3,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,0,2,4,3,0,2,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,4,3,4,3,4,3,0,2,4,3,4,3,4,3,4,3,0,2,4,3,4,3,4,3,0,2,4,3,0,2,4,3,4,3,4,3,0,2,4,3,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,4,3,4,3,4,3,0,2,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,4,3,4,3,0,2,4,3,4,3,4,3,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,0,2,4,3,4,3,4,3,4,3,0,2,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,4,3,4,3,0,2,4,3,4,3,4,3,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,0,2,4,3,4,3,4,3,4,3,0,2,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,4,3,4,3,0,2,4,3,4,3,4,3,4,3,4,3,0,2,0,2,4,3,4,3,4,3,4,3,0,2,4,3,4,3,4,3,4,3,0,2,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,4,3,4,3,0,2,4,3,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,4,3,0,2,4,3,4,3,0,2,4,3,4,3,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,4,3,0,2,4,3,0,2,4,3,0,2,4,3,0,2,4,3,0,2,4,3,0,2,4,3,0,2,4,3,0,2,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3,4,3]

         
# time slicing
inner_counter_time=0
inner_counter_hyper=0
inner_counter_inner=0
inner_counter_hasatt=0
inner_bound = outer_grids[0]
for i in range(88*16):
	if(case_arr[i]==3):
		x_time.append(inner_counter_time)
		y_time[4].append(y_wkst5_dup[i])
		y_time[3].append(y_srv1_dup[i])
		y_time[1].append(y_srv2_onchip_dup[i])
		y_time[2].append(y_srv2_interchip_dup[i])
		y_time[0].append(y_wkst10_dup[i])
		y_time[8].append(y_amd_core4_dup[i])
		y_time[5].append(y_broadwell_dup[i])
		y_time[6].append(y_skylake_dup[i])
		y_time[7].append(y_amd_local_dup[i])
		y_time[9].append(y_and_dup[i])
		y_time[10].append(y_or_dup[i])
		inner_counter_time = inner_counter_time + 1
	elif(case_arr[i]==4):
		x_hyper.append(inner_counter_hyper)
		y_hyper[4].append(y_wkst5_dup[i])
		y_hyper[3].append(y_srv1_dup[i])
		y_hyper[1].append(y_srv2_onchip_dup[i])
		y_hyper[2].append(y_srv2_interchip_dup[i])
		y_hyper[0].append(y_wkst10_dup[i])
		y_hyper[8].append(y_amd_core4_dup[i])
		y_hyper[5].append(y_broadwell_dup[i])
		y_hyper[6].append(y_skylake_dup[i])
		y_hyper[7].append(y_amd_local_dup[i])
		y_hyper[9].append(y_and_dup[i])
		y_hyper[10].append(y_or_dup[i])
		inner_counter_hyper = inner_counter_hyper + 1
	elif(case_arr[i]==1):
		x_inner.append(inner_counter_inner)
		y_inner[4].append(y_wkst5_dup[i])
		y_inner[3].append(y_srv1_dup[i])
		y_inner[1].append(y_srv2_onchip_dup[i])
		y_inner[2].append(y_srv2_interchip_dup[i])
		y_inner[0].append(y_wkst10_dup[i])
		y_inner[8].append(y_amd_core4_dup[i])
		y_inner[5].append(y_broadwell_dup[i])
		y_inner[6].append(y_skylake_dup[i])
		y_inner[7].append(y_amd_local_dup[i])
		y_inner[9].append(y_and_dup[i])
		y_inner[10].append(y_or_dup[i])
		inner_counter_inner = inner_counter_inner + 1
	elif(case_arr[i]==2):
		x_hasatt.append(inner_counter_hasatt)
		y_hasatt[4].append(y_wkst5_dup[i])
		y_hasatt[3].append(y_srv1_dup[i])
		y_hasatt[1].append(y_srv2_onchip_dup[i])
		y_hasatt[2].append(y_srv2_interchip_dup[i])
		y_hasatt[0].append(y_wkst10_dup[i])
		y_hasatt[8].append(y_amd_core4_dup[i])
		y_hasatt[5].append(y_broadwell_dup[i])
		y_hasatt[6].append(y_skylake_dup[i])
		y_hasatt[7].append(y_amd_local_dup[i])
		y_hasatt[9].append(y_and_dup[i])
		y_hasatt[10].append(y_or_dup[i])
		inner_counter_hasatt = inner_counter_hasatt + 1


ia_base=[1,2,3,4,10,12,14,16,18,20,45,46,47,48,54,56,58,60,62,64]
is_base=[26,28,32,34,35,39,70,72,76,78,79,83]
isa_base=[22,24,30,33,36,40,66,68,74,77,80,84]
ea_base=[5,6,7,8,9,11,13,15,17,19,49,50,51,52,53,55,57,59,61,63,]
es_base=[25,27,31,37,41,43,69,71,75,81,85,87]
esa_base=[21,23,29,38,42,44,65,67,73,82,86,88]


for i in range(len(ia_base)):
	ia_base[i] = ia_base[i] -1
for i in range(len(isa_base)):
	is_base[i] = is_base[i] -1
for i in range(len(isa_base)):
	isa_base[i] = isa_base[i] -1
for i in range(len(ea_base)):
	ea_base[i] = ea_base[i] -1
for i in range(len(esa_base)):
	es_base[i] = es_base[i] -1
for i in range(len(isa_base)):
	esa_base[i] = esa_base[i] -1


#print len(ia_base), len(is_base), len(isa_base), len(ea_base), len(es_base), len(esa_base)


all_suc=[0,0,0,0,0,0,0,0,0,0,0]
ia_suc=[0,0,0,0,0,0,0,0,0,0,0]
is_suc=[0,0,0,0,0,0,0,0,0,0,0]
isa_suc=[0,0,0,0,0,0,0,0,0,0,0]
ea_suc=[0,0,0,0,0,0,0,0,0,0,0]
es_suc=[0,0,0,0,0,0,0,0,0,0,0]
esa_suc=[0,0,0,0,0,0,0,0,0,0,0]


for i in x_single:
	if i in x_single:
		if y_wkst5_single[i]==9:
			all_suc[4] = all_suc[4] + 1
		if y_srv1_single[i]==7:
			all_suc[3]=all_suc[3]+1
		if y_srv2_onchip_single[i]==3:
			all_suc[1]=all_suc[1]+1
		if y_srv2_interchip_single[i]==5:
			all_suc[2]=all_suc[2]+1
		if y_wkst10_single[i]==1:
			all_suc[0]=all_suc[0]+1
		if y_amd_core4_single[i]==17:
			all_suc[8]=all_suc[8]+1
		if y_broadwell_single[i]==11:
			all_suc[5]=all_suc[5]+1
		if y_skylake_single[i]==13:
			all_suc[6]=all_suc[6]+1
		if y_amd_local_single[i]==15:
			all_suc[7]=all_suc[7]+1
		if y_and_single[i]==19:
			all_suc[9]=all_suc[9]+1
		if y_or_single[i]==21:
			all_suc[10]=all_suc[10]+1
	if i in ia_base:
		if y_wkst5_single[i]==9:
			ia_suc[4] = ia_suc[4] + 1
		if y_srv1_single[i]==7:
			ia_suc[3]=ia_suc[3]+1
		if y_srv2_onchip_single[i]==3:
			ia_suc[1]=ia_suc[1]+1
		if y_srv2_interchip_single[i]==5:
			ia_suc[2]=ia_suc[2]+1
		if y_wkst10_single[i]==1:
			ia_suc[0]=ia_suc[0]+1
		if y_amd_core4_single[i]==17:
			ia_suc[8]=ia_suc[8]+1
		if y_broadwell_single[i]==11:
			ia_suc[5]=ia_suc[5]+1
		if y_skylake_single[i]==13:
			ia_suc[6]=ia_suc[6]+1
		if y_amd_local_single[i]==15:
			ia_suc[7]=ia_suc[7]+1
		if y_and_single[i]==19:
			ia_suc[9]=ia_suc[9]+1
		if y_or_single[i]==21:
			ia_suc[10]=ia_suc[10]+1
	if i in is_base:
		if y_wkst5_single[i]==9:
			is_suc[4] = is_suc[4] + 1
		if y_srv1_single[i]==7:
			is_suc[3]=is_suc[3]+1
		if y_srv2_onchip_single[i]==3:
			is_suc[1]=is_suc[1]+1
		if y_srv2_interchip_single[i]==5:
			is_suc[2]=is_suc[2]+1
		if y_wkst10_single[i]==1:
			is_suc[0]=is_suc[0]+1
		if y_amd_core4_single[i]==17:
			is_suc[8]=is_suc[8]+1
		if y_broadwell_single[i]==11:
			is_suc[5]=is_suc[5]+1
		if y_skylake_single[i]==13:
			is_suc[6]=is_suc[6]+1
		if y_amd_local_single[i]==15:
			is_suc[7]=is_suc[7]+1
		if y_and_single[i]==19:
			is_suc[9]=is_suc[9]+1
		if y_or_single[i]==21:
			is_suc[10]=is_suc[10]+1
	if i in isa_base:
		if y_wkst5_single[i]==9:
			isa_suc[4]=isa_suc[4]+1
		if y_srv1_single[i]==7:
			isa_suc[3]=isa_suc[3]+1
		if y_srv2_onchip_single[i]==3:
			isa_suc[1]=isa_suc[1]+1
		if y_srv2_interchip_single[i]==5:
			isa_suc[2]=isa_suc[2]+1
		if y_wkst10_single[i]==1:
			isa_suc[0]=isa_suc[0]+1
		if y_amd_core4_single[i]==17:
			isa_suc[8]=isa_suc[8]+1
		if y_broadwell_single[i]==11:
			isa_suc[5]=isa_suc[5]+1
		if y_skylake_single[i]==13:
			isa_suc[6]=isa_suc[6]+1
		if y_amd_local_single[i]==15:
			isa_suc[7]=isa_suc[7]+1
		if y_and_single[i]==19:
			isa_suc[9]=isa_suc[9]+1
		if y_or_single[i]==21:
			isa_suc[10]=isa_suc[10]+1
	if i in ea_base:
		if y_wkst5_single[i]==9:
			ea_suc[4]=ea_suc[4]+1
		if y_srv1_single[i]==7:
			ea_suc[3]=ea_suc[3]+1
		if y_srv2_onchip_single[i]==3:
			ea_suc[1]=ea_suc[1]+1
		if y_srv2_interchip_single[i]==5:
			ea_suc[2]=ea_suc[2]+1
		if y_wkst10_single[i]==1:
			ea_suc[0]=ea_suc[0]+1
		if y_amd_core4_single[i]==17:
			ea_suc[8]=ea_suc[8]+1
		if y_broadwell_single[i]==11:
			ea_suc[5]=ea_suc[5]+1
		if y_skylake_single[i]==13:
			ea_suc[6]=ea_suc[6]+1
		if y_amd_local_single[i]==15:
			ea_suc[7]=ea_suc[7]+1
		if y_and_single[i]==19:
			ea_suc[9]=ea_suc[9]+1
		if y_or_single[i]==21:
			ea_suc[10]=ea_suc[10]+1
	if i in es_base:
		if y_wkst5_single[i]==9:
			es_suc[4]=es_suc[4]+1
		if y_srv1_single[i]==7:
			es_suc[3]=es_suc[3]+1
		if y_srv2_onchip_single[i]==3:
			es_suc[1]=es_suc[1]+1
		if y_srv2_interchip_single[i]==5:
			es_suc[2]=es_suc[2]+1
		if y_wkst10_single[i]==1:
			es_suc[0]=es_suc[0]+1
		if y_amd_core4_single[i]==17:
			es_suc[8]=es_suc[8]+1
		if y_broadwell_single[i]==11:
			es_suc[5]=es_suc[5]+1
		if y_skylake_single[i]==13:
			es_suc[6]=es_suc[6]+1
		if y_amd_local_single[i]==15:
			es_suc[7]=es_suc[7]+1
		if y_and_single[i]==19:
			es_suc[9]=es_suc[9]+1
		if y_or_single[i]==21:
			es_suc[10]=es_suc[10]+1
	if i in esa_base:
		if y_wkst5_single[i]==9:
			esa_suc[4]=esa_suc[4]+1
		if y_srv1_single[i]==7:
			esa_suc[3]=esa_suc[3]+1
		if y_srv2_onchip_single[i]==3:
			esa_suc[1]=esa_suc[1]+1
		if y_srv2_interchip_single[i]==5:
			esa_suc[2]=esa_suc[2]+1
		if y_wkst10_single[i]==1:
			esa_suc[0]=esa_suc[0]+1
		if y_amd_core4_single[i]==17:
			esa_suc[8]=esa_suc[8]+1
		if y_broadwell_single[i]==11:
			esa_suc[5]=esa_suc[5]+1
		if y_skylake_single[i]==13:
			esa_suc[6]=esa_suc[6]+1
		if y_amd_local_single[i]==15:
			esa_suc[7]=esa_suc[7]+1
		if y_and_single[i]==19:
			esa_suc[9]=esa_suc[9]+1
		if y_or_single[i]==21:
			esa_suc[10]=esa_suc[10]+1
print "All the vulnerability types found vulnerable for the machines in paper's order: "
print all_suc
print "I-A Vulnerability types found vulnerable for the machines in paper's order: "
print ia_suc
print "I-S Vulnerability types found vulnerable for the machines in paper's order: "
print is_suc
print "I-SA Vulnerability types found vulnerable for the machines in paper's order: "
print isa_suc
print "E-A Vulnerability types found vulnerable for the machines in paper's order: "
print ea_suc
print "E-S Vulnerability types found vulnerable for the machines in paper's order: "
print es_suc
print "E-SA Vulnerability types found vulnerable for the machines in paper's order: "
print esa_suc

x_single_plus=[]
for i in range(len(x_single)):
	x_single_plus.append(x_single[i] + 1)


read_suc=[0,0,0,0,0,0,0,0,0,0,0]
write_suc=[0,0,0,0,0,0,0,0,0,0,0]
ccinv_suc=[0,0,0,0,0,0,0,0,0,0,0]
clflush_suc=[0,0,0,0,0,0,0,0,0,0,0]
time_suc=[0,0,0,0,0,0,0,0,0,0,0]
hyper_suc=[0,0,0,0,0,0,0,0,0,0,0]
inner_suc=[0,0,0,0,0,0,0,0,0,0,0]
hasatt_suc=[0,0,0,0,0,0,0,0,0,0,0]

#print len(x_read), len(x_write), len(x_ccinv), len(x_clflush), len(x_time), len(x_hyper), len(x_inner), len(x_hasatt)

len_xread=len(x_read)
len_xwrite=len(x_write)
len_xccinv=len(x_ccinv)
len_xclfllush=len(x_clflush)

#print grids

for i in x_read:
	if y_read[4][i]==9:
		read_suc[4] = read_suc[4] + 1
	if y_read[3][i]==7:
		read_suc[3]=read_suc[3]+1
	if y_read[1][i]==3:
		read_suc[1]=read_suc[1]+1
	if y_read[2][i]==5:
		read_suc[2]=read_suc[2]+1
	if y_read[0][i]==1:
		read_suc[0]=read_suc[0]+1
	if y_read[8][i]==17:
		read_suc[8]=read_suc[8]+1
	if y_read[5][i]==11:
		read_suc[5]=read_suc[5]+1
	if y_read[6][i]==13:
		read_suc[6]=read_suc[6]+1
	if y_read[7][i]==15:
		read_suc[7]=read_suc[7]+1
	if y_read[9][i]==19:
		read_suc[9]=read_suc[9]+1
	if y_read[10][i]==21:
		read_suc[10]=read_suc[10]+1
	if y_read[3][i]==-2:
		len_xread = len_xread - 1
for i in x_write:
	if y_write[4][i]==9:
		write_suc[4] = write_suc[4] + 1
	if y_write[3][i]==7:
		write_suc[3]=write_suc[3]+1
	if y_write[1][i]==3:
		write_suc[1]=write_suc[1]+1
	if y_write[2][i]==5:
		write_suc[2]=write_suc[2]+1
	if y_write[0][i]==1:
		write_suc[0]=write_suc[0]+1
	if y_write[8][i]==17:
		write_suc[8]=write_suc[8]+1
	if y_write[5][i]==11:
		write_suc[5]=write_suc[5]+1
	if y_write[6][i]==13:
		write_suc[6]=write_suc[6]+1
	if y_write[7][i]==15:
		write_suc[7]=write_suc[7]+1
	if y_write[9][i]==19:
		write_suc[9]=write_suc[9]+1
	if y_write[10][i]==21:
		write_suc[10]=write_suc[10]+1
	if y_write[3][i]==-2:
		len_xwrite = len_xwrite - 1


for i in range(len(x_ccinv)):
	if y_ccinv[4][i]==9:
		ccinv_suc[4] = ccinv_suc[4] + 1
	if y_ccinv[3][i]==7:
		ccinv_suc[3]=ccinv_suc[3]+1
	if y_ccinv[1][i]==3:
		ccinv_suc[1]=ccinv_suc[1]+1
	if y_ccinv[2][i]==5:
		ccinv_suc[2]=ccinv_suc[2]+1
	if y_ccinv[0][i]==1:
		ccinv_suc[0]=ccinv_suc[0]+1
	if y_ccinv[8][i]==17:
		ccinv_suc[8]=ccinv_suc[8]+1
	if y_ccinv[5][i]==11:
		ccinv_suc[5]=ccinv_suc[5]+1
	if y_ccinv[6][i]==13:
		ccinv_suc[6]=ccinv_suc[6]+1
	if y_ccinv[7][i]==15:
		ccinv_suc[7]=ccinv_suc[7]+1
	if y_ccinv[9][i]==19:
		ccinv_suc[9]=ccinv_suc[9]+1
	if y_ccinv[10][i]==21:
		ccinv_suc[10]=ccinv_suc[10]+1
	if y_ccinv[3][i]==-2:
		len_xccinv = len_xccinv - 1
for i in range(len(x_clflush)):
	if y_clflush[4][i]==9:
		clflush_suc[4] = clflush_suc[4] + 1
	if y_clflush[3][i]==7:
		clflush_suc[3]=clflush_suc[3]+1
	if y_clflush[1][i]==3:
		clflush_suc[1]=clflush_suc[1]+1
	if y_clflush[2][i]==5:
		clflush_suc[2]=clflush_suc[2]+1
	if y_clflush[0][i]==1:
		clflush_suc[0]=clflush_suc[0]+1
	if y_clflush[8][i]==17:
		clflush_suc[8]=clflush_suc[8]+1
	if y_clflush[5][i]==11:
		clflush_suc[5]=clflush_suc[5]+1
	if y_clflush[6][i]==13:
		clflush_suc[6]=clflush_suc[6]+1
	if y_clflush[7][i]==15:
		clflush_suc[7]=clflush_suc[7]+1
	if y_clflush[9][i]==19:
		clflush_suc[9]=clflush_suc[9]+1
	if y_clflush[10][i]==21:
		clflush_suc[10]=clflush_suc[10]+1
	if y_clflush[3][i]==-2:
		len_xclfllush = len_xclfllush - 1
for i in range(len(x_time)):
	if y_time[4][i]==9:
		time_suc[4] = time_suc[4] + 1
	if y_time[3][i]==7:
		time_suc[3]=time_suc[3]+1
	if y_time[1][i]==3:
		time_suc[1]=time_suc[1]+1
	if y_time[2][i]==5:
		time_suc[2]=time_suc[2]+1
	if y_time[0][i]==1:
		time_suc[0]=time_suc[0]+1
	if y_time[8][i]==17:
		time_suc[8]=time_suc[8]+1
	if y_time[5][i]==11:
		time_suc[5]=time_suc[5]+1
	if y_time[6][i]==13:
		time_suc[6]=time_suc[6]+1
	if y_time[7][i]==15:
		time_suc[7]=time_suc[7]+1
	if y_time[9][i]==19:
		time_suc[9]=time_suc[9]+1
	if y_time[10][i]==21:
		time_suc[10]=time_suc[10]+1
for i in range(len(x_hyper)):
	if y_hyper[4][i]==9:
		hyper_suc[4] = hyper_suc[4] + 1
	if y_hyper[3][i]==7:
		hyper_suc[3]=hyper_suc[3]+1
	if y_hyper[1][i]==3:
		hyper_suc[1]=hyper_suc[1]+1
	if y_hyper[2][i]==5:
		hyper_suc[2]=hyper_suc[2]+1
	if y_hyper[0][i]==1:
		hyper_suc[0]=hyper_suc[0]+1
	if y_hyper[8][i]==17:
		hyper_suc[8]=hyper_suc[8]+1
	if y_hyper[5][i]==11:
		hyper_suc[5]=hyper_suc[5]+1
	if y_hyper[6][i]==13:
		hyper_suc[6]=hyper_suc[6]+1
	if y_hyper[7][i]==15:
		hyper_suc[7]=hyper_suc[7]+1
	if y_hyper[9][i]==19:
		hyper_suc[9]=hyper_suc[9]+1
	if y_hyper[10][i]==21:
		hyper_suc[10]=hyper_suc[10]+1
for i in range(len(x_inner)):
	if y_inner[4][i]==9:
		inner_suc[4] = inner_suc[4] + 1
	if y_inner[3][i]==7:
		inner_suc[3]=inner_suc[3]+1
	if y_inner[1][i]==3:
		inner_suc[1]=inner_suc[1]+1
	if y_inner[2][i]==5:
		inner_suc[2]=inner_suc[2]+1
	if y_inner[0][i]==1:
		inner_suc[0]=inner_suc[0]+1
	if y_inner[8][i]==17:
		inner_suc[8]=inner_suc[8]+1
	if y_inner[5][i]==11:
		inner_suc[5]=inner_suc[5]+1
	if y_inner[6][i]==13:
		inner_suc[6]=inner_suc[6]+1
	if y_inner[7][i]==15:
		inner_suc[7]=inner_suc[7]+1
	if y_inner[9][i]==19:
		inner_suc[9]=inner_suc[9]+1
	if y_inner[10][i]==21:
		inner_suc[10]=inner_suc[10]+1
for i in range(len(x_hasatt)):
	if y_hasatt[4][i]==9:
		hasatt_suc[4] = hasatt_suc[4] + 1
	if y_hasatt[3][i]==7:
		hasatt_suc[3]=hasatt_suc[3]+1
	if y_hasatt[1][i]==3:
		hasatt_suc[1]=hasatt_suc[1]+1
	if y_hasatt[2][i]==5:
		hasatt_suc[2]=hasatt_suc[2]+1
	if y_hasatt[0][i]==1:
		hasatt_suc[0]=hasatt_suc[0]+1
	if y_hasatt[8][i]==17:
		hasatt_suc[8]=hasatt_suc[8]+1
	if y_hasatt[5][i]==11:
		hasatt_suc[5]=hasatt_suc[5]+1
	if y_hasatt[6][i]==13:
		hasatt_suc[6]=hasatt_suc[6]+1
	if y_hasatt[7][i]==15:
		hasatt_suc[7]=hasatt_suc[7]+1
	if y_hasatt[9][i]==19:
		hasatt_suc[9]=hasatt_suc[9]+1
	if y_hasatt[10][i]==21:
		hasatt_suc[10]=hasatt_suc[10]+1

#print len_xread, len_xwrite, len_xccinv, len_xclfllush

print "Vulnerability cases using local read as observation steps found vulnerable for the machines in paper's order: "
print read_suc
print "Vulnerability cases using local write as observation steps found vulnerable for the machines in paper's order: "
print write_suc
print "Vulnerability cases using remote write to invalidate as observation steps found vulnerable for the machines in paper's order: "
print ccinv_suc
print "Vulnerability cases using flush as observation steps found vulnerable for the machines in paper's order: "
print clflush_suc
print "Vulnerability cases with victim and attacker running time-slicing found vulnerable for the machines in paper's order: "
print time_suc
print "Vulnerability cases with victim and attacker running hyper-threading found vulnerable for the machines in paper's order: "
print hyper_suc
print "Vulnerability cases with victim and attacker running on different cores found vulnerable for the machines in paper's order: "
print inner_suc
print "Vulnerability cases running with victim found vulnerable for the machines in paper's order: "
print hasatt_suc



plt.rc('xtick', labelsize=13)
plt.rc('ytick', labelsize=16)


for i in range(len(x_single)):
	if y_wkst5_single[i]==8:
		y_wkst5_single[i]=-5
	if y_srv1_single[i]==6:
		y_srv1_single[i]=-5
	if y_srv2_onchip_single[i]==2:
		y_srv2_onchip_single[i]=-5
	if y_srv2_interchip_single[i]==4:
		y_srv2_interchip_single[i]=-5
	if y_wkst10_single[i]==0:
		y_wkst10_single[i]=-5
	if y_amd_core4_single[i]==16:
		y_amd_core4_single[i]=-5
	if y_broadwell_single[i]==10:
		y_broadwell_single[i]=-5
	if y_skylake_single[i]==12:
		y_skylake_single[i]=-5
	if y_amd_local_single[i]==14:
		y_amd_local_single[i]=-5
	if y_and_single[i]==18:
		y_and_single[i]=-5
	if y_or_single[i]==20:
		y_or_single[i]=-5


fig, ax=plt.subplots()
f1 = plt.figure(1)
f1.set_figheight(5.4)
f1.set_figwidth(28.3)
plt.plot(x_single,y_wkst5_single, label='Intel Core i5-4570', linestyle="",color='#A1887F',marker="o", markersize=8)
plt.plot(x_single,y_srv1_single, label='Intel Xeon E5-2690', linestyle="",color='c',marker='v', markersize=8)
plt.plot(x_single,y_srv2_onchip_single, label='Intel Xeon E5-2667 on-chip', linestyle="",color='g',marker="X", markersize=8)
plt.plot(x_single,y_srv2_interchip_single, label='Intel Xeon E5-2667 inter-chip', linestyle="",color='m',marker='*', markersize=10)
plt.plot(x_single,y_wkst10_single, label='Intel Xeon E5-1620', linestyle="",color='b',marker='<', markersize=8)
plt.plot(x_single,y_amd_core4_single, label='AMD EPYC 7571', linestyle="",color='k',marker='H', markersize=8)
plt.plot(x_single,y_broadwell_single, label='Intel Broadwell E5-2686', linestyle="",color='#99004C',marker='P', markersize=8)
plt.plot(x_single,y_skylake_single, label='Intel Skylake P-8175', linestyle="",color='#FF9933',marker='>', markersize=8)
plt.plot(x_single,y_amd_local_single, label='AMD FX-8150', linestyle="",color='#FF80AB',marker='p', markersize=8)
plt.plot(x_single,y_and_single, label='Found in all tested CPUs', linestyle="",color='#00FF00',marker='^', markersize=8)
plt.plot(x_single,y_or_single, label='Found in at least one tested CPU', linestyle="",color='r',marker='d', markersize=8)
plt.xlabel('vulnerabilities')
plt.ylabel('CPU Type')
ax.grid(True, which='major')
ax.set_xticks(x_single)
ax.set_ylim([-0.5,22])
ax.set_xlim([-1,88])
ax.set_xticklabels(x_single_plus)
ax.set_yticks([1,3,5,7,9,11,13,15,17,19,21])#1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22])
ax.set_yticklabels(["Intel Xeon E5-1620", "Intel Xeon E5-2667 on-chip", "Intel Xeon E5-2667 inter-chip", "Intel Xeon E5-2690", "Intel Core i5-4570",  "Intel Xeon E5-2686", "Intel Xeon P-8175", "AMD FX-8150", "AMD EPYC 7571", "Found in all tested CPUs", "Found in at least one tested CPU" ])
box = ax.get_position()
ax.set_position([box.x0*1.45, box.y0 + box.height * 0.2,
                 box.width, box.height * 0.8])

plt.savefig("gen_figure/vul_by_type_"+sys.argv[1]+".pdf",bbox_inches='tight')
#f1.show()


for i in range(len(x_1)):
	if y_wkst5_1[i]==8:
		y_wkst5_1[i]=-5
	if y_srv1_1[i]==6:
		y_srv1_1[i]=-5
	if y_srv2_onchip_1[i]==2:
		y_srv2_onchip_1[i]=-5
	if y_srv2_interchip_1[i]==4:
		y_srv2_interchip_1[i]=-5
	if y_wkst10_1[i]==0:
		y_wkst10_1[i]=-5
	if y_amd_core4_1[i]==16:
		y_amd_core4_1[i]=-5
	if y_broadwell_1[i]==10:
		y_broadwell_1[i]=-5
	if y_skylake_1[i]==12:
		y_skylake_1[i]=-5
	if y_amd_local_1[i]==14:
		y_amd_local_1[i]=-5
	if y_and_1[i]==18:
		y_and_1[i]=-5
	if y_or_1[i]==20:
		y_or_1[i]=-5

for i in range(len(x_2)):
	if y_wkst5_2[i]==8:
		y_wkst5_2[i]=-5
	if y_srv1_2[i]==6:
		y_srv1_2[i]=-5
	if y_srv2_onchip_2[i]==2:
		y_srv2_onchip_2[i]=-5
	if y_srv2_interchip_2[i]==4:
		y_srv2_interchip_2[i]=-5
	if y_wkst10_2[i]==0:
		y_wkst10_2[i]=-5
	if y_amd_core4_2[i]==16:
		y_amd_core4_2[i]=-5
	if y_broadwell_2[i]==10:
		y_broadwell_2[i]=-5
	if y_skylake_2[i]==12:
		y_skylake_2[i]=-5
	if y_amd_local_2[i]==14:
		y_amd_local_2[i]=-5
	if y_and_2[i]==18:
		y_and_2[i]=-5
	if y_or_2[i]==20:
		y_or_2[i]=-5


fig, ax=plt.subplots()
f2 = plt.figure(2)
f2.set_figheight(5.4)
f2.set_figwidth(28.3)
plt.plot(x_1,y_wkst5_1, label='Intel Core i5-4570 (wkst5)', linestyle="",color='#A1887F',marker="o", markersize=2)
plt.plot(x_1,y_srv1_1, label='Intel Xeon E5-2690 (srv1)', linestyle="",color='c',marker="o", markersize=2)
plt.plot(x_1,y_srv2_onchip_1, label='Intel Xeon E5-2667 (srv2_on_chip)', linestyle="",color='g',marker="o", markersize=2)
plt.plot(x_1,y_srv2_interchip_1, label='Intel Xeon E5-2667 (srv2_inter_chip)', linestyle="",color='m',marker="o", markersize=2)
plt.plot(x_1,y_wkst10_1, label='Intel Xeon E5-1620 (wkst10)', linestyle="",color='b',marker="o", markersize=2)
plt.plot(x_1,y_amd_core4_1, label='AMD EPYC Instance (amd Amazon EC2)', linestyle="",color='k',marker="o", markersize=2)
plt.plot(x_1,y_broadwell_1, label='Intel Broadwell Amazon EC2', linestyle="",color='#99004C',marker="o", markersize=2)
plt.plot(x_1,y_skylake_1, label='Intel Skylake Amazon EC2', linestyle="",color='#FF9933',marker="o", markersize=2)
plt.plot(x_1,y_amd_local_1, label='AMD FX-8150', linestyle="",color='#FF80AB',marker="o", markersize=2)
plt.plot(x_1,y_and_1, label='Vulnerabilities Found in all tested CPUs (and result)', linestyle="",color='#00FF00',marker="o", markersize=2)
plt.plot(x_1,y_or_1, label='Vulnerabilities Found in at least one tested CPU (or result)', linestyle="",color='r',marker="o", markersize=2)
plt.xlabel('vulnerabilities')
plt.ylabel('CPU Type')
ax.set_xticks(grids)
ax.grid(True, which='major', axis='x',linestyle="--")
ax.set_ylim([-0.2,22])
ax.set_xlim([-1,554])
ax.set_xticklabels(x_single_1)
ax.set_yticks([1,3,5,7,9,11,13,15,17,19,21])
ax.set_yticklabels(["Intel Xeon E5-1620", "Intel Xeon E5-2667 on-chip", "Intel Xeon E5-2667 inter-chip", "Intel Xeon E5-2690", "Intel Core i5-4570",  "Intel Xeon E5-2686", "Intel Xeon P-8175", "AMD FX-8150", "AMD EPYC 7571", "Found in all tested CPUs", "Found in at least one tested CPU" ])
box = ax.get_position()
ax.set_position([box.x0*1.5, box.y0 + box.height * 0.26,
                 box.width, box.height * 0.74])
plt.savefig("gen_figure/vul_by_case_"+sys.argv[1]+"_1.pdf",bbox_inches='tight')
#f2.show()




fig, ax=plt.subplots()
f3 = plt.figure(3)
f3.set_figheight(5.4)
f3.set_figwidth(28.3)
plt.plot(x_2,y_wkst5_2, label='Intel Core i5-4570', linestyle="",color='#A1887F',marker="o", markersize=2)
plt.plot(x_2,y_srv1_2, label='Intel Xeon E5-2690', linestyle="",color='c',marker="o", markersize=2)
plt.plot(x_2,y_srv2_onchip_2, label='Intel Xeon E5-2667 on-chip', linestyle="",color='g',marker="o", markersize=2)
plt.plot(x_2,y_srv2_interchip_2, label='Intel Xeon E5-2667 inter-chip', linestyle="",color='m',marker="o", markersize=2)
plt.plot(x_2,y_wkst10_2, label='Intel Xeon E5-1620', linestyle="",color='b',marker="o", markersize=2)
plt.plot(x_2,y_amd_core4_2, label='AMD EPYC 7571', linestyle="",color='k',marker="o", markersize=2)
plt.plot(x_2,y_broadwell_2, label='Intel Broadwell E5-2686', linestyle="",color='#99004C',marker="o", markersize=2)
plt.plot(x_2,y_skylake_2, label='Intel Skylake P-8175', linestyle="",color='#FF9933',marker="o", markersize=2)
plt.plot(x_2,y_amd_local_2, label='AMD FX-8150', linestyle="",color='#FF80AB',marker="o", markersize=2)
plt.plot(x_2,y_and_2, label='Found in all tested CPUs', linestyle="",color='#00FF00',marker="o", markersize=2)
plt.plot(x_2,y_or_2, label='Found in at least one tested CPU', linestyle="",color='r',marker="o", markersize=2)
plt.xlabel('vulnerabilities')
plt.ylabel('CPU Type')
ax.set_xticks(grids_2)
ax.grid(True, which='major', axis='x',linestyle="--")
ax.set_ylim([-0.2,22])
ax.set_xlim([-1,1094-554])
ax.set_xticklabels(x_single_2)
ax.set_yticks([1,3,5,7,9,11,13,15,17,19,21])#1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22])
ax.set_yticklabels(["Intel Xeon E5-1620", "Intel Xeon E5-2667 on-chip", "Intel Xeon E5-2667 inter-chip", "Intel Xeon E5-2690", "Intel Core i5-4570",  "Intel Xeon E5-2686", "Intel Xeon P-8175", "AMD FX-8150", "AMD EPYC 7571", "Found in all tested CPUs", "Found in at least one tested CPU" ])
box = ax.get_position()
ax.set_position([box.x0*1.5, box.y0 + box.height * 0.26,
                 box.width, box.height * 0.74])

plt.savefig("gen_figure/vul_by_case_"+sys.argv[1]+"_2.pdf",bbox_inches='tight')
#f3.show()

#plt.tight_layout()
#raw_input()

print "Dot figures are export to gen_figure folder."








