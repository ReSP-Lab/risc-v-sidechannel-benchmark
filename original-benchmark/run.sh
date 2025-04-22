#!/bin/bash

#
# This file is the top script file to generate and evaluate benchmark code
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

machine=$1
max_cycle=$2
cache_size=$3
cache_asso=$4
cache_line=$5
num_test=$6
vic_num=$7
probe_size=$8
each_run=$9
output_name=${10}

rm -rf gen_output
mkdir gen_output

cp run_sub.sh gen_output/

gcc -std=gnu99 -o gen_vulnerabilities_clean gen_vulnerabilities_clean.c
./gen_vulnerabilities_clean $machine $max_cycle $cache_size $cache_asso $cache_line $num_test $vic_num $probe_size $each_run

cd gen_output
./run_sub.sh $output_name

