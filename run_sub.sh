#!/bin/bash

#
# This file run each benchmark code and output results
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

i=0
for file in `find . -name '*.c' | sed 's_./__'| sort -n`
do
  echo "i is $i"
  let i=$i+1
  echo "Compiling and executing: file $file"
  gcc $file -std=gnu99 -lm -o ${file:0:-2} -O0 
  ./${file:0:-2} ../benchmark_output/$output_name &

  ps
  PID2=$(ps -ef | grep ${file:0:-2} | awk '{print $2}')
  j=0 
  pid=0
  ori=1
  for word in $PID2
  do
    let j=$j+1
      echo $word
      if [ "$j" == "$ori" ]
    then
        pid=$word
    fi
  done
  echo $pid
  echo "Before finishing ${file:0:-2}"
  while ps -ef | grep ${file:0:-2} | grep -v grep > /dev/null; do sleep 0.5; done
  echo "Finished ${file:0:-2}"

done
echo "Finished"