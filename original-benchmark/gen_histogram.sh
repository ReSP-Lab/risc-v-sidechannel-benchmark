#!/bin/bash


#
# This file is the script file for generating histogram
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


output_name=$1


if [ -z  "$output_name" ]; then
   echo "  Please set the output machine and corresponding information as the histogram name."
   exit 1
fi

gcc timing_histogram.c -std=gnu99 -o timing_histogram
./timing_histogram $output_name
