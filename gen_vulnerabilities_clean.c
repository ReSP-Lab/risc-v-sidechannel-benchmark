/*
* This file generates benchmark code
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <stdint.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
#include <inttypes.h>
#include "gen_vulnerabilities.h"

#define ATTACKER 0
#define VICTIM 1
#define A 2
#define A_ALIAS 3
#define D 4
#define U 5
#define INV 6
#define A_INV 7
#define D_INV 8
#define U_INV 9
#define A_ALIAS_INV 10
#define MISS 11
#define HIT 12
#define STAR 13
#define PRE 14

#define VUL_NUM 88
#define ALL_NUM 4913
#define TOTAL_STATES 17 
#define TOTAL_ADDRESS_VALUES 7
#define MUTEX_BOUND 10000
#define RANDOM_NUM 1000

#define WKST10 0
#define WKST5 1
#define SRV1 2
#define SRV2ONCHIP 3
#define SRV2INTERCHIP 4
#define AMDCORE4 5
#define BROADWELL 6
#define SKYLAKE 7
#define AMDLOCAL 8

typedef int bool;
enum { false, true };


void print_access(int if_write, bool if_unknown, FILE *fout, int miss_hit, int l1_assoc, int if_last, int var_type, int step_num, bool local_u_last_step){

	if (if_unknown){
	  	fprintf(fout, "     if(j==0) {\n"); // a
	  	int var_num=0;
	  	if (if_write){
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
	           	fprintf(fout, "      \"mfence              \\n\" \n");
	           	fprintf(fout, "      \"movq %%%rcx, (%%%rcx)       \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	if (if_last){
	           		fprintf(fout, "      \"rdtsc               \\n\" \n");
	            	fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            	fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
	            	fprintf(fout, "      : \n");
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : );\n");
	       	   	} 
	       	}
	       	if (if_last||local_u_last_step){
	       		for(int pp=var_num;pp<var_num+8;pp++){
	       		fprintf(fout, "      asm __volatile__ (\n");
			   	fprintf(fout, "      \"mfence              \\n\" \n");
			    fprintf(fout, "      \"rdtsc               \\n\" \n");
			    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, (%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"rdtsc               \\n\" \n");
	            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
	            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n",pp);
	            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           }
	        }
		} 
		else {
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
					fprintf(fout, "      \"mfence              \\n\" \n");
					fprintf(fout, "      \"rdtsc               \\n\" \n");
					fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq (%%%rcx),  %%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 64(%%%rcx), %%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 448(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 256(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 384(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 320(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 192(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 128(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
		       	if (if_last){
			       	fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
		        }
	           	else {
	            	fprintf(fout, "      : \n");
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : );\n");
	       	   	}
	       	}
	       	if (if_last||local_u_last_step){
	       	   	for(int pp=var_num;pp<var_num+8;pp++){
	       		fprintf(fout, "      asm __volatile__ (\n");
			   	fprintf(fout, "      \"mfence              \\n\" \n");
			    fprintf(fout, "      \"rdtsc               \\n\" \n");
			    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq (%%%rcx),  %%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 64(%%%rcx), %%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 448(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 256(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 384(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 320(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 192(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 128(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"rdtsc               \\n\" \n");
	            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
	            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n",pp);
	            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	            }
	       	}
		}
		   

		fprintf(fout, "     } else if (j==1) {\n"); // a_alias
		var_num=8;
	  	if (if_write){
			for(int pp=var_num;pp<var_num+8;pp++){
			    fprintf(fout, "      asm __volatile__ (\n");
			    if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			    }
	           	fprintf(fout, "      \"mfence              \\n\" \n");
	            fprintf(fout, "      \"movq %%%rcx, (%%%rcx)       \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            if (if_last){
	           		fprintf(fout, "      \"rdtsc               \\n\" \n");
	            	fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            	fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
	            	fprintf(fout, "      : \n");
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : );\n");
	       	   	} 
	       	}
	       	if (if_last||local_u_last_step){
	       		for(int pp=var_num;pp<var_num+8;pp++){
	       			fprintf(fout, "      asm __volatile__ (\n");
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
	       			fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, (%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	        	}
	        }
		} 
		else {
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq (%%%rcx),  %%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 64(%%%rcx), %%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 448(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 256(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 384(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 320(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 192(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\" \n");
		        fprintf(fout, "      \"movq 128(%%%rcx),%%%rax     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\"\n");
		        if (if_last){
			       	fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	            }
	            else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	    }
	       	}
	       	for(int pp=var_num;pp<var_num+8;pp++){
	       	   	if (if_last||local_u_last_step){
		       		fprintf(fout, "      asm __volatile__ (\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq (%%%rcx),  %%%rax     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq 64(%%%rcx), %%%rax     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq 448(%%%rcx),%%%rax     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq 256(%%%rcx),%%%rax     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq 384(%%%rcx),%%%rax     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq 320(%%%rcx),%%%rax     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq 192(%%%rcx),%%%rax     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq 128(%%%rcx),%%%rax     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
			}
		}
		  
		fprintf(fout, "     } else if (j==2) {\n"); // NIB
		var_num=0;
	  	    if (if_write){
				for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
	           	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 576(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 768(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 512(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 896(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 704(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 832(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 960(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 640(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	} 
	       	}
	       	if (if_last||local_u_last_step){
	       		for(int pp=var_num;pp<var_num+8;pp++){
		       		fprintf(fout, "      asm __volatile__ (\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 576(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 768(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 512(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 896(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 704(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 832(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 960(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 640(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
		           	fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	        	}
	        }
		} 
		else {
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 576(%%%rcx),  %%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 768(%%%rcx), %%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 512(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 896(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 704(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 832(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 960(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 640(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
		       	if (if_last){
			       	fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	}
	       	}
	       	for(int pp=var_num;pp<var_num+8;pp++){
	       	   	if (if_last||local_u_last_step){
		       		fprintf(fout, "      asm __volatile__ (\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq 576(%%%rcx),  %%%rax     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq 768(%%%rcx), %%%rax     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq 512(%%%rcx),%%%rax     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq 896(%%%rcx),%%%rax     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq 704(%%%rcx),%%%rax     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq 832(%%%rcx),%%%rax     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq 960(%%%rcx),%%%rax     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq 640(%%%rcx),%%%rax     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
			}
		}		   
		fprintf(fout, "     }\n"); 
		fprintf(fout, " else if(j==3){\n");
		fprintf(fout, "   	int dummy_print=1;\n");
		fprintf(fout, "   }\n");
	} 
	else {
		int var_num=RANDOM_NUM;
		switch(var_type)
			{
			    case A:
			        var_num = 0;
			        break;
			    case D:
			        var_num = 16;
			        break;
			    case A_ALIAS:
			        var_num = 8;
			        break;
			    default:
			        printf("Error! var_type is not correct for access;1");
			}
		if (if_write){
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
	            fprintf(fout, "      \"mfence              \\n\" \n");
	            fprintf(fout, "      \"movq %%%rcx, (%%%rcx)       \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	            }
	            else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	} 
	       	}
		} 
		else {
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq (%%%rcx),  %%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 64(%%%rcx), %%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 448(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 256(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 384(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 320(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 192(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq 128(%%%rcx),%%%rax     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
		       	if (if_last){
		       		fprintf(fout, "      \"mfence              \\n\"\n");
	           		fprintf(fout, "      \"rdtsc               \\n\" \n");
	            	fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            	fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
	            	fprintf(fout, "      : \n");
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : );\n");
	       	   	}
	       	}
		}
	}
}


void print_flush(int if_write, bool if_unknown, FILE *fout, int miss_hit, int l1_assoc, int if_last, int var_type,bool local_u_last_step){

	if (if_unknown){
	  	fprintf(fout, "     if(j==0) {\n"); // a
	  	int var_num=0;
	  	if (if_write){
			for(int pp=var_num;pp<var_num+8;pp++){
			    fprintf(fout, "      asm __volatile__ (\n");
			    if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			    }
	            fprintf(fout, "      \"mfence              \\n\" \n");
	            fprintf(fout, "      \"movq %%%rcx, (%%%rcx)       \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
	            fprintf(fout, "      \"mfence              \\n\"\n");
	            if (if_last){
	           		fprintf(fout, "      \"rdtsc               \\n\" \n");
	            	fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            	fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	            }
	           	else {
	            	fprintf(fout, "      : \n");
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : );\n");
	       	   	} 
	       	}
	       	if (if_last||local_u_last_step){
	       		for(int pp=var_num;pp<var_num+8;pp++){
	       			fprintf(fout, "      asm __volatile__ (\n");
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, (%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\" \n");
			        fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
			        fprintf(fout, "      \"mfence              \\n\"\n");
	           		fprintf(fout, "      \"rdtsc               \\n\" \n");
	            	fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            	fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	        	}
			}
		} 
		else {
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
			   	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush (%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 64(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 448(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 256(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 384(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 320(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 192(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 128(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
		       	if (if_last){
	           		fprintf(fout, "      \"rdtsc               \\n\" \n");
	            	fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            	fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
	            	fprintf(fout, "      : \n");
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : );\n");
	       	   	}
	       	}
	       	if (if_last||local_u_last_step){
	       		for(int pp=var_num;pp<var_num+8;pp++){
	       			fprintf(fout, "      asm __volatile__ (\n");
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush (%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 64(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 448(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 256(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 384(%%%rcx)     \\n\"\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				   	fprintf(fout, "      \"clflush 320(%%%rcx)     \\n\"\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				   	fprintf(fout, "      \"clflush 192(%%%rcx)     \\n\"\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				   	fprintf(fout, "      \"clflush 128(%%%rcx)     \\n\"\n");
				   	fprintf(fout, "      \"mfence              \\n\"\n");
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	        	}	
			}
		}

		fprintf(fout, "     } else if (j==1) {\n"); // a_alias
		var_num=8;
	  	if (if_write){
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
	           	fprintf(fout, "      \"mfence              \\n\" \n");
	           	fprintf(fout, "      \"movq %%%rcx, (%%%rcx)       \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
	            	fprintf(fout, "      : \n");
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : );\n");
	       	   	} 
	       	}
	       	if (if_last||local_u_last_step){
	       		for(int pp=var_num;pp<var_num+8;pp++){
	       			fprintf(fout, "      asm __volatile__ (\n");
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, (%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\"\n");
	           		fprintf(fout, "      \"rdtsc               \\n\" \n");
	            	fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            	fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	        	}
			}
		} 
		else {
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
			   	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush (%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 64(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 448(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 256(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 384(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 320(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 192(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 128(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
		       	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	}
	       	}
	       	if (if_last||local_u_last_step){
	       		for(int pp=var_num;pp<var_num+8;pp++){
	       			fprintf(fout, "      asm __volatile__ (\n");
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush (%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 64(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 448(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 256(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 384(%%%rcx)     \\n\"\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				   	fprintf(fout, "      \"clflush 320(%%%rcx)     \\n\"\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				   	fprintf(fout, "      \"clflush 192(%%%rcx)     \\n\"\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				   	fprintf(fout, "      \"clflush 128(%%%rcx)     \\n\"\n");
				   	fprintf(fout, "      \"mfence              \\n\"\n");
	           		fprintf(fout, "      \"rdtsc               \\n\" \n");
	            	fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            	fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
	            	fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
	            	fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	        	}
			}
		}
		  

		fprintf(fout, "     } else if (j==2) {\n"); // NIB
		var_num=0;
	  	if (if_write){
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
	           	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 576(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 768(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 512(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 896(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 704(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 832(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 960(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 640(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
	           	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	} 
	       	}
	       	if (if_last||local_u_last_step){
	       		for(int pp=var_num;pp<var_num+8;pp++){
	       			fprintf(fout, "      asm __volatile__ (\n");
			   		fprintf(fout, "      \"mfence              \\n\" \n");
			    	fprintf(fout, "      \"rdtsc               \\n\" \n");
			    	fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 576(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 768(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 512(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 896(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 704(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 832(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 960(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"movq %%%rcx, 640(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\"\n");
	           		fprintf(fout, "      \"rdtsc               \\n\" \n");
	            	fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
	            	fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
	            	fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
	            	fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	        	}
			}
		} 
		else {
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
			   	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 576(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 768(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 512(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 896(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 704(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 832(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 960(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 640(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
		       	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	}
	       	}
	       	if (if_last||local_u_last_step){
	       		for(int pp=var_num;pp<var_num+8;pp++){
	       			fprintf(fout, "      asm __volatile__ (\n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
				   	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 576(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 768(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 512(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 896(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 704(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 832(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 960(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\" \n");
			       	fprintf(fout, "      \"clflush 640(%%%rcx)     \\n\"\n");
			       	fprintf(fout, "      \"mfence              \\n\"\n"); 
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t_r%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	        	}
			}
		}
		   
		fprintf(fout, "     }\n"); 
		fprintf(fout, " else if(j==3){\n");
		fprintf(fout, "   	int dummy_print=1;\n");
		fprintf(fout, "   }\n");
	  
	} 
	else if(var_type==INV){
		if (if_write){
			for(int pp=0;pp<0+24;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
	           	fprintf(fout, "      \"mfence              \\n\" \n");
	           	fprintf(fout, "      \"movq %%%rcx, (%%%rcx)       \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-0);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	} 
	       	} 
	       	for(int pp=0;pp<8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
	           	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 576(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 768(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 512(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 896(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 704(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 832(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 960(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"movq %%%rcx, 640(%%%rcx)     \\n\"\n");
		        fprintf(fout, "      \"mfence              \\n\"\n");
	           	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp+24);
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	} 
	       	}       		
		} 
		else {
			for(int pp=0;pp<0+24;pp++){
				fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
			   	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush (%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 64(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 448(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 256(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 384(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 320(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 192(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 128(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
		       	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-0);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	}
	       	}

	       	for(int pp=0;pp<8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
	           	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 576(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 768(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 512(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 896(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 704(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 832(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 960(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 640(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n"); 
	           	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp+24);
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+untar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	} 
	       	}	       
		}
	} 
	else {
		int var_num=RANDOM_NUM;
		switch(var_type)
		{
			case A_INV:
			    var_num = 0;
		       	break;
			case D_INV:
		        var_num = 16;
		        break;
			case A_ALIAS_INV:
		        var_num = 8;
		        break;
			default:
			    printf("Error! var_type is not correct for flush;1");
		}
		if (if_write){
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
	           	fprintf(fout, "      \"mfence              \\n\" \n");
	           	fprintf(fout, "      \"movq %%%rcx, (%%%rcx)       \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 64(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 448(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 256(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 384(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 320(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 192(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	fprintf(fout, "      \"movq %%%rcx, 128(%%%rcx)     \\n\"\n");
	           	fprintf(fout, "      \"mfence              \\n\"\n");
	           	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	} 
	       	}
		} 
		else {
			for(int pp=var_num;pp<var_num+8;pp++){
			   	fprintf(fout, "      asm __volatile__ (\n");
			   	if(if_last){
				   	fprintf(fout, "      \"mfence              \\n\" \n");
				    fprintf(fout, "      \"rdtsc               \\n\" \n");
				    fprintf(fout, "      \"movl %%%%eax, %%%%esi   \\n\" \n");
			   	}
			   	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush (%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 64(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 448(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 256(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 384(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 320(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 192(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\" \n");
		       	fprintf(fout, "      \"clflush 128(%%%rcx)     \\n\"\n");
		       	fprintf(fout, "      \"mfence              \\n\"\n");
		       	if (if_last){
		           	fprintf(fout, "      \"rdtsc               \\n\" \n");
		            fprintf(fout, "      \"subl %%%%esi, %%%%eax         \\n\" \n");
		            fprintf(fout, "      :\"=a\" (t%d) \n", pp-var_num);
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : \"%%esi\", \"%%edx\");\n");
	           	}
	           	else {
		            fprintf(fout, "      : \n");
		            fprintf(fout, "      : \"c\" (start[%d]+tar_block)\n", pp);
		            fprintf(fout, "      : );\n");
	       	   	}
	       	}
		}
	}
}

void print_step(int if_write, int var_type, int miss_hit, FILE *fout, int l1_assoc, int if_last, int step_num, bool local_u_last_step){
	bool if_unknown=false;
	if (var_type==U || var_type==U_INV) if_unknown=true;
	if (var_type==A || var_type==A_ALIAS || var_type==D || var_type==U){
		print_access(if_write, if_unknown, fout, miss_hit, l1_assoc, if_last, var_type, step_num, local_u_last_step);
	} else if (var_type==INV || var_type==A_INV || var_type==D_INV || var_type==A_ALIAS_INV || var_type==U_INV){
		print_flush(if_write, if_unknown, fout, miss_hit, l1_assoc, if_last, var_type, local_u_last_step);
	} else if (var_type==PRE) {
		fprintf(fout, "	rand_chosen = rand()%%(%d);\n", TOTAL_ADDRESS_VALUES+1);
		fprintf(fout, "  if (m==10 && i==10) printf(\"rand_chosen=%%d\\n\",  rand_chosen);\n");
		fprintf(fout, "	if (rand_chosen==0){\n");
		print_access(0, 0, fout, 0, l1_assoc, if_last, A, step_num, false);
		fprintf(fout, "	} else if (rand_chosen==1){\n");
		print_access(0, 0, fout, 0, l1_assoc, if_last, A_ALIAS, step_num, false);
		fprintf(fout, "	} else if (rand_chosen==2){\n");
		print_access(0, 0, fout, 0, l1_assoc, if_last, D, step_num, false);
		fprintf(fout, "	} else if (rand_chosen==3){\n");
		print_flush(0, 0, fout, 0, l1_assoc, if_last, INV, false);
		fprintf(fout, "	} else if (rand_chosen==4){\n");
		print_flush(0, 0, fout, 0, l1_assoc, if_last, A_INV, false);
		fprintf(fout, "	} else if (rand_chosen==5){\n");
		print_flush(0, 0, fout, 0, l1_assoc, if_last, D_INV, false);
		fprintf(fout, "	} else if (rand_chosen==6){\n");
		print_flush(0, 0, fout, 0, l1_assoc, if_last, A_ALIAS_INV, false);
		fprintf(fout, "	} else {\n");
		fprintf(fout, "	}\n");
		fprintf(fout, "//  initiate maintain_arr\n");
	} else if (var_type==STAR) {
		fprintf(fout, "	rand_chosen = rand()%%(%d);\n", TOTAL_ADDRESS_VALUES*2+1);
		fprintf(fout, "  if (m==10 && i==10) printf(\"rand_chosen=%%d\\n\",  rand_chosen);\n");
		fprintf(fout, "	if (rand_chosen==0){\n");
		print_access(0, 0, fout, 0, l1_assoc, if_last, A, step_num, false);
		fprintf(fout, "	} else if (rand_chosen==1){\n");
		print_access(1, 0, fout, 0, l1_assoc, if_last, A, step_num, false);
		fprintf(fout, "	} else if (rand_chosen==2){\n");
		print_access(0, 0, fout, 0, l1_assoc, if_last, A_ALIAS, step_num, false);
		fprintf(fout, "	} else if (rand_chosen==3){\n");
		print_access(1, 0, fout, 0, l1_assoc, if_last, A_ALIAS, step_num, false);
		fprintf(fout, "	} else if (rand_chosen==4){\n");
		print_access(0, 0, fout, 0, l1_assoc, if_last, D, step_num, false);
		fprintf(fout, "	} else if (rand_chosen==5){\n");
		print_access(1, 0, fout, 0, l1_assoc, if_last, D, step_num, false);
		fprintf(fout, "	} else if (rand_chosen==6){\n");
		print_flush(0, 0, fout, 0, l1_assoc, if_last, INV, false);
		fprintf(fout, "	} else if (rand_chosen==7){\n");
		print_flush(1, 0, fout, 0, l1_assoc, if_last, INV, false);
		fprintf(fout, "	} else if (rand_chosen==8){\n");
		print_flush(0, 0, fout, 0, l1_assoc, if_last, A_INV, false);
		fprintf(fout, "	} else if (rand_chosen==9){\n");
		print_flush(1, 0, fout, 0, l1_assoc, if_last, A_INV, false);
		fprintf(fout, "	} else if (rand_chosen==10){\n");
		print_flush(0, 0, fout, 0, l1_assoc, if_last, D_INV, false);
		fprintf(fout, "	} else if (rand_chosen==11){\n");
		print_flush(1, 0, fout, 0, l1_assoc, if_last, D_INV, false);
		fprintf(fout, "	} else if (rand_chosen==12){\n");
		print_flush(0, 0, fout, 0, l1_assoc, if_last, A_ALIAS_INV, false);
		fprintf(fout, "	} else if (rand_chosen==13){\n");
		print_flush(1, 0, fout, 0, l1_assoc, if_last, A_ALIAS_INV, false);
		fprintf(fout, "	} else {\n");
		fprintf(fout, "	}\n");
		fprintf(fout, "//  initiate maintain_arr\n");
	} else {
		printf("Error: Wrong input in print_step\n");
	}
}


void print_eviction_set(FILE *fout, int l1_assoc, int probe_size, int l1_cache_line, int l1_cache_set, int vul_in){
	fprintf(fout, "	int rand_chosen=%d;\n", RANDOM_NUM);

	fprintf(fout, "//  initiate maintain_arr\n");
	fprintf(fout, "  maintain_arr = mmap(0, %d*sizeof(pid_t), PROT_READ|PROT_WRITE,\n", MUTEX_BOUND);
	fprintf(fout, "              MAP_SHARED | MAP_ANONYMOUS, -1, 0);\n");
	fprintf(fout, "  if (!maintain_arr) {\n");
	fprintf(fout, "    perror(\"mmap failed for maintain_arr\");\n");
	fprintf(fout, "    exit(1);\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  for(int i=0; i < %d; i++){\n", MUTEX_BOUND);
	fprintf(fout, "    maintain_arr[i]=%d-i;\n", MUTEX_BOUND);
	fprintf(fout, "  }\n");
	  
	fprintf(fout, "  for(int i =0 ; i< %d*%d*%d; i++) /*probe_arr*/ chain_arr[i]=/*(char)*/ MAX_ARRAY_SIZE-i; //copy on write\n", l1_assoc, l1_cache_line, l1_cache_set);

	fprintf(fout, "  char* start[%d*%d*%d];\n", l1_assoc, l1_cache_line, l1_cache_set);
	fprintf(fout, "  \n");

	fprintf(fout, "  int tar_block;\n");
  	fprintf(fout, "  int untar_block;\n");

	fprintf(fout, "  for(int i=0;i< %d*%d/%d;i++){\n", l1_cache_set, l1_assoc, 4);
    fprintf(fout, "  	start[i]=&chain_arr[ i*way_size	];\n");
  	fprintf(fout, "  }\n");

	fprintf(fout, "  // load into L1 L2\n");
						
	fprintf(fout, "	  for(int i=0;i<%d*8;i++){  \n", l1_assoc);
	fprintf(fout, "      asm __volatile__ (\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq (%%%rcx),  %%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 64(%%%rcx), %%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 448(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 256(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 384(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 320(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 192(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 128(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\"\n");
	fprintf(fout, "      : \n");
	fprintf(fout, "      : \"c\" (start[i])\n");
	fprintf(fout, "      : );\n");
	fprintf(fout, "	  }  \n");

	fprintf(fout, "	  for(int i=0;i<%d*8;i++){  \n", l1_assoc);
	fprintf(fout, "      asm __volatile__ (\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 576(%%%rcx),  %%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 768(%%%rcx), %%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 512(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 896(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 704(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 832(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 960(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\" \n");
	fprintf(fout, "      \"movq 640(%%%rcx),%%%rax     \\n\"\n");
	fprintf(fout, "      \"mfence              \\n\"\n");
	fprintf(fout, "      : \n");
	fprintf(fout, "      : \"c\" (start[i]+untar_block)\n");
	fprintf(fout, "      : );\n");
	fprintf(fout, "	  }  \n");

	fprintf(fout, "\n");
	fprintf(fout, "  int tmp;\n");
	fprintf(fout, "  for (int i = 0; i < 1000000; ++i){\n");
	fprintf(fout, "      tmp+=i;\n");
	fprintf(fout, "    } \n");

}

void print_pre(FILE *fout, int l1_cache_size, int l1_assoc, int l1_cache_line, int l1_cache_set, int num_test, int vic_machine, int probe_size, int max_array_size, int each_run, int miss_hit, int max_cycle, bool u_last_step){
	fputs("#define _GNU_SOURCE\n", fout);
	fputs("#include <stdio.h>\n", fout);
	fputs("#include <stdlib.h>\n", fout);
	fputs("#include <sys/mman.h>\n", fout);
	fputs("#include <string.h>\n", fout);
	fputs("#include <unistd.h>\n", fout);
	fputs("#include <sys/types.h>\n", fout);
	fputs("#include <emmintrin.h>\n", fout);
	fputs("#include <x86intrin.h>\n", fout);
	fputs("#include <stdint.h>\n", fout);
	fputs("#include <fcntl.h>\n", fout);
	fputs("#include <sched.h>\n", fout);
	fputs("#include <sys/mman.h>\n\n", fout);
	fputs("#include <math.h>\n\n", fout);
	fputs("#include <time.h>\n\n", fout);
	fputs("", fout);

	fprintf(fout, "#define L1_CACHE_SIZE %d\n", l1_cache_size);
	fprintf(fout, "#define L1_ASSOC %d\n", l1_assoc);
	fprintf(fout, "#define L1_CACHE_LINE %d\n", l1_cache_line);
	fprintf(fout, "#define L1_CACHE_SET  L1_CACHE_SIZE/L1_ASSOC/L1_CACHE_LINE\n");
	fprintf(fout, "#define VIC_MACHINE %d\n", vic_machine);
	fprintf(fout, "#define PROBE_SIZE %d\n", probe_size);
	fprintf(fout, "#define MAX_ARRAY_SIZE %d\n", max_array_size);
	fprintf(fout, "#define NUM_TEST %d\n\n", num_test);
	fprintf(fout, "#define NUM_TEST_HALF %d\n\n", num_test/2);
	fprintf(fout, "#define EACH_RUN %d\n\n", each_run);
	fprintf(fout, "#define MAX_CYCLE %d\n\n", max_cycle);



	fprintf(fout, "#define NOONE_RUN 0\n");
	fprintf(fout, "#define STEP0_RUN 1\n");
	fprintf(fout, "#define STEP1_RUN 2\n");
	fprintf(fout, "#define STEP2_RUN 3\n");
	fprintf(fout, "#define STEP3_RUN 4\n");

	fprintf(fout, "const int line_size=%d;\n", l1_cache_line);
	fprintf(fout, "const int way_size=%d*%d/%d;\n", l1_cache_line,l1_cache_set, 8);
	fprintf(fout, "int histogram[%d][%d]={0};\n", probe_size, max_cycle);
	fprintf(fout, "int status;\n");
	fprintf(fout, "int u_last_step = %d;\n", u_last_step);
	

	fprintf(fout, "uint64_t sum[PROBE_SIZE] = {0};\n");
	fprintf(fout, "uint64_t counter[PROBE_SIZE] = {0};\n");
	fprintf(fout, "uint64_t result[2] = {0};\n");

	fprintf(fout, "double arr1[%d*%d]={0};\n", each_run, num_test/2);
	fprintf(fout, "double arr2[%d*%d]={0};\n", each_run, num_test/2);
	fprintf(fout, "double arr3[%d*%d]={0};\n", each_run, num_test/2);
	fprintf(fout, "double arr4[%d*%d]={0};\n", each_run, num_test/2);
	fprintf(fout, "double arr5[%d*%d]={0};\n", each_run, num_test/2);
	fprintf(fout, "double arr6[%d*%d]={0};\n", each_run, num_test/2);
	
	fprintf(fout, "char **chain_arr;\n");
	fprintf(fout, "volatile pid_t *maintain_arr;\n");
	fprintf(fout, "\n");


	// t test
	fprintf(fout, "double Pvalue (const double *restrict ARRAY1, const size_t ARRAY1_SIZE, const double *restrict ARRAY2, const size_t ARRAY2_SIZE) {//calculate a p-value based on an array\n");
	fprintf(fout, "  if (ARRAY1_SIZE <= 1) {\n");
	fprintf(fout, "    return 1.0;\n");
	fprintf(fout, "  } else if (ARRAY2_SIZE <= 1) {\n");
	fprintf(fout, "    return 1.0;\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  double fmean1 = 0.0, fmean2 = 0.0;\n");
	fprintf(fout, "  for (size_t x = 0; x < ARRAY1_SIZE; x++) {//get sum of values in ARRAY1\n");
	fprintf(fout, "    if (isfinite(ARRAY1[x]) == 0) {//check to make sure this is a real numbere\n");
	fprintf(fout, "      puts(\"Got a non-finite number in 1st array, can't calculate P-value.\");\n");
	fprintf(fout, "      exit(EXIT_FAILURE);\n");
	fprintf(fout, "    }\n");
	fprintf(fout, "    fmean1 += ARRAY1[x];\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  fmean1 /= ARRAY1_SIZE;\n");
	fprintf(fout, "  for (size_t x = 0; x < ARRAY2_SIZE; x++) {//get sum of values in ARRAY2\n");
	fprintf(fout, "    if (isfinite(ARRAY2[x]) == 0) {//check to make sure this is a real number\n");
	fprintf(fout, "      puts(\"Got a non-finite number in 2nd array, can't calculate P-value.\");\n");
	fprintf(fout, "      exit(EXIT_FAILURE);\n");
	fprintf(fout, "    }\n");
	fprintf(fout, "    fmean2 += ARRAY2[x];\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  fmean2 /= ARRAY2_SIZE;\n");
	fprintf(fout, "//  printf(\"mean1 = %lf mean2 = %lf\\n\", fmean1, fmean2);\n");
	fprintf(fout, "  if (fmean1 == fmean2) {\n");
	fprintf(fout, "    printf(\"the means are equal\\n\");\n");
	fprintf(fout, "    return 1.0;//if the means are equal, the p-value is 1, leave the function\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  double unbiased_sample_variance1 = 0.0, unbiased_sample_variance2 = 0.0;\n");
	fprintf(fout, "  for (size_t x = 0; x < ARRAY1_SIZE; x++) {//1st part of added unbiased_sample_variance\n");
	fprintf(fout, "    unbiased_sample_variance1 += (ARRAY1[x]-fmean1)*(ARRAY1[x]-fmean1);\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  for (size_t x = 0; x < ARRAY2_SIZE; x++) {\n");
	fprintf(fout, "    unbiased_sample_variance2 += (ARRAY2[x]-fmean2)*(ARRAY2[x]-fmean2);\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "//  printf(\"unbiased_sample_variance1 = %%lf\\tunbiased_sample_variance2 = %%lf\\n\",unbiased_sample_variance1,unbiased_sample_variance2);//DEBUGGING\n");
	fprintf(fout, "  unbiased_sample_variance1 = unbiased_sample_variance1/(ARRAY1_SIZE-1);\n");
	fprintf(fout, "  unbiased_sample_variance2 = unbiased_sample_variance2/(ARRAY2_SIZE-1);\n");
	fprintf(fout, "  const double WELCH_T_STATISTIC = (fmean1-fmean2)/sqrt(unbiased_sample_variance1/ARRAY1_SIZE+unbiased_sample_variance2/ARRAY2_SIZE);\n");
	fprintf(fout, "  const double DEGREES_OF_FREEDOM = pow((unbiased_sample_variance1/ARRAY1_SIZE+unbiased_sample_variance2/ARRAY2_SIZE),2.0)//numerator\n");
	fprintf(fout, "   /\n");
	fprintf(fout, "  (\n");
	fprintf(fout, "    (unbiased_sample_variance1*unbiased_sample_variance1)/(ARRAY1_SIZE*ARRAY1_SIZE*(ARRAY1_SIZE-1))+\n");
	fprintf(fout, "    (unbiased_sample_variance2*unbiased_sample_variance2)/(ARRAY2_SIZE*ARRAY2_SIZE*(ARRAY2_SIZE-1))\n");
	fprintf(fout, "  );\n");
	fprintf(fout, "//  printf(\"Welch = %%lf DOF = %%lf\\n\", WELCH_T_STATISTIC, DEGREES_OF_FREEDOM);\n");
	fprintf(fout, "    const double a = DEGREES_OF_FREEDOM/2;\n");
	fprintf(fout, "  double value = DEGREES_OF_FREEDOM/(WELCH_T_STATISTIC*WELCH_T_STATISTIC+DEGREES_OF_FREEDOM);\n");
	fprintf(fout, "  if ((isinf(value) != 0) || (isnan(value) != 0)) {\n");
	fprintf(fout, "    printf(\"free degree1\\n\");\n");
	fprintf(fout, "    return 1.0;\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  if ((isinf(value) != 0) || (isnan(value) != 0)) {\n");
	fprintf(fout, "    printf(\"free degree2\\n\");\n");
	fprintf(fout, "    return 1.0;\n");
	fprintf(fout, "  }\n");
	fprintf(fout, " \n");
	fprintf(fout, "  const double beta = lgammal(a)+0.57236494292470009-lgammal(a+0.5);\n");
	fprintf(fout, "  const double acu = 0.1E-14;\n");
	fprintf(fout, "  double ai;\n");
	fprintf(fout, "  double cx;\n");
	fprintf(fout, "  int indx;\n");
	fprintf(fout, "  int ns;\n");
	fprintf(fout, "  double pp;\n");
	fprintf(fout, "  double psq;\n");
	fprintf(fout, "  double qq;\n");
	fprintf(fout, "  double rx;\n");
	fprintf(fout, "  double temp;\n");
	fprintf(fout, "  double term;\n");
	fprintf(fout, "  double xx;\n");
	 
	fprintf(fout, "//  ifault = 0;\n");
	fprintf(fout, "//Check the input arguments.\n");
	fprintf(fout, "  if ( (a <= 0.0)) {// || (0.5 <= 0.0 )){\n");
	fprintf(fout, "//    *ifault = 1;\n");
	fprintf(fout, "//    return value;\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  if ( value < 0.0 || 1.0 < value )\n");
	fprintf(fout, "  {\n");
	fprintf(fout, "//    *ifault = 2;\n");
	fprintf(fout, "    return value;\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "/*\n");
	fprintf(fout, "  Special cases.\n");
	fprintf(fout, "*/\n");
	fprintf(fout, "  if ( value == 0.0 || value == 1.0 )   {\n");
	fprintf(fout, "    return value;\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  psq = a + 0.5;\n");
	fprintf(fout, "  cx = 1.0 - value;\n");
	fprintf(fout, " \n");
	fprintf(fout, "  if ( a < psq * value )\n");
	fprintf(fout, "  {\n");
	fprintf(fout, "    xx = cx;\n");
	fprintf(fout, "    cx = value;\n");
	fprintf(fout, "    pp = 0.5;\n");
	fprintf(fout, "    qq = a;\n");
	fprintf(fout, "    indx = 1;\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  else\n");
	fprintf(fout, "  {\n");
	fprintf(fout, "    xx = value;\n");
	fprintf(fout, "    pp = a;\n");
	fprintf(fout, "    qq = 0.5;\n");
	fprintf(fout, "    indx = 0;\n");
	fprintf(fout, "  }\n");
	fprintf(fout, " \n");
	fprintf(fout, "  term = 1.0;\n");
	fprintf(fout, "  ai = 1.0;\n");
	fprintf(fout, "  value = 1.0;\n");
	fprintf(fout, "  ns = ( int ) ( qq + cx * psq );\n");
	fprintf(fout, "/*\n");
	fprintf(fout, "  Use the Soper reduction formula.\n");
	fprintf(fout, "*/\n");
	fprintf(fout, "  rx = xx / cx;\n");
	fprintf(fout, "  temp = qq - ai;\n");
	fprintf(fout, "  if ( ns == 0 )\n");
	fprintf(fout, "  {\n");
	fprintf(fout, "    rx = xx;\n");
	fprintf(fout, "  }\n");
	fprintf(fout, " \n");
	fprintf(fout, "  for ( ; ; )\n");
	fprintf(fout, "  {\n");
	fprintf(fout, "    term = term * temp * rx / ( pp + ai );\n");
	fprintf(fout, "    value = value + term;;\n");
	fprintf(fout, "    temp = fabs ( term );\n");
	fprintf(fout, " \n");
	fprintf(fout, "    if ( temp <= acu && temp <= acu * value )\n");
	fprintf(fout, "    {\n");
	fprintf(fout, "      value = value * exp ( pp * log ( xx ) \n");
	fprintf(fout, "      + ( qq - 1.0 ) * log ( cx ) - beta ) / pp;\n");
	fprintf(fout, " \n");
	fprintf(fout, "      if ( indx )\n");
	fprintf(fout, "      {\n");
	fprintf(fout, "        value = 1.0 - value;\n");
	fprintf(fout, "      }\n");
	fprintf(fout, "      break;\n");
	fprintf(fout, "    }\n");
	fprintf(fout, "    ai = ai + 1.0;\n");
	fprintf(fout, "    ns = ns - 1;\n");
	fprintf(fout, " \n");
	fprintf(fout, "    if ( 0 <= ns )\n");
	fprintf(fout, "    {\n");
	fprintf(fout, "      temp = qq - ai;\n");
	fprintf(fout, "      if ( ns == 0 )\n");
	fprintf(fout, "      {\n");
	fprintf(fout, "        rx = xx;\n");
	fprintf(fout, "      }\n");
	fprintf(fout, "    }\n");
	fprintf(fout, "    else\n");
	fprintf(fout, "    {\n");
	fprintf(fout, "      temp = psq;\n");
	fprintf(fout, "      psq = psq + 1.0;\n");
	fprintf(fout, "    }\n");
	fprintf(fout, "  }\n");
	fprintf(fout, "  return value;\n");
	fprintf(fout, "}\n");

}

int find_index(int aa[], int num_elements, int value)
{
	int i;
	for (i=0; i<num_elements; i++)
	{
		if (aa[i] == value)
		{
			return(value);  /* it was found */
		}
	}
	return(-1);  /* if it was not found */
}


int main(int argc, char const *argv[])
{
	char* attack_machine = argv[1];
	int max_cycle = atoi(argv[2]);
	int l1_cache_size = atoi(argv[3]);
	int l1_assoc = atoi(argv[4]);
	int l1_cache_line = atoi(argv[5]);
	int l1_cache_set = l1_cache_size/l1_assoc/l1_cache_line;
	int num_test = atoi(argv[6]);
	int vic_machine = atoi(argv[7]);
	int probe_size = atoi(argv[8]);
	int max_array_size = l1_cache_set*l1_cache_line*l1_assoc*8;
	int each_run = atoi(argv[9]);
	int pad_num = 25;
	char buf[40];

	int cpu[5] = {0};
	if (vic_machine==WKST10){
		cpu[0]=3;
		cpu[1]=7;
		cpu[2]=2;
		cpu[3]=6;
		cpu[4]=5;
	} else if (vic_machine==SRV2ONCHIP){
		cpu[0]=8;
		cpu[1]=20;
		cpu[2]=10;
		cpu[3]=22;
		cpu[4]=6;
	} else if (vic_machine==SRV2INTERCHIP){
		cpu[0]=8;
		cpu[1]=20;
		cpu[2]=11;
		cpu[3]=23;
		cpu[4]=6;
	} else if (vic_machine==SRV1){
		cpu[0]=6;
		cpu[1]=14;
		cpu[2]=7;
		cpu[3]=15;
		cpu[4]=5;
	} else if (vic_machine==WKST5){
		cpu[0]=3;
		cpu[1]=3;
		cpu[2]=2;
		cpu[3]=2;
		cpu[4]=1;
	} else if (vic_machine==AMDCORE4){
       	cpu[0]=3;
        cpu[1]=1;
	    cpu[2]=2;
        cpu[3]=0;
        cpu[4]=0;
    } else if (vic_machine==BROADWELL){
	    cpu[0]=3;
        cpu[1]=3;
	    cpu[2]=2;
        cpu[3]=2;
        cpu[4]=1;
    } else if (vic_machine==SKYLAKE){
        cpu[0]=3;
        cpu[1]=7;
        cpu[2]=2;
        cpu[3]=6;
        cpu[4]=5;
    } else if (vic_machine==AMDLOCAL){
		cpu[0]=5;
		cpu[1]=5;
		cpu[2]=7;
		cpu[3]=7;
		cpu[4]=3;
	} else {
		printf("Error in choosing machines\n");
		exit(1);
	}
	srand(1234);
	char* type_match[14] = {"ATT","VIC","A","A_ALIAS","D","U","INV","A_INV","D_INV","U_INV","A_ALIAS_INV","ERROR1", "ERROR2", "STAR"};
	int grids[4913]={0};
	int eff_grids[88]={0};
	// 1 internal 2 with attacker but no two process on the same core
	// 3 time-slicing 4 hyper-threading

	int pattern_counter = 0;
	int case_counter = -1;
	if (strcmp(attack_machine, "x86")==0) {
		for (int vul_num_order = 0; vul_num_order < VUL_NUM; ++vul_num_order)
		{
			int eff_flag = false;
			pattern_counter = 0;
		for (int vul1 = 0; vul1 < TOTAL_STATES; ++vul1)
		{
		for (int vul2 = 0; vul2 < TOTAL_STATES; ++vul2)
		{
		for (int vul3 = 0; vul3 < TOTAL_STATES; ++vul3)
		{
			
		total_arr[pattern_counter][0] = pattern_counter;
		total_arr[pattern_counter][1] = possi_access[vul1][0];
		total_arr[pattern_counter][2] = possi_access[vul1][1];
		total_arr[pattern_counter][3] = possi_access[vul2][0];
		total_arr[pattern_counter][4] = possi_access[vul2][1];
		total_arr[pattern_counter][5] = possi_access[vul3][0];
		total_arr[pattern_counter][6] = possi_access[vul3][1];
		total_arr[pattern_counter][7] = 0;
			
		if (eff_flag==false && total_arr[pattern_counter][1]==three_step_arr[vul_num_order][1] && total_arr[pattern_counter][2]==three_step_arr[vul_num_order][2] && total_arr[pattern_counter][3]==three_step_arr[vul_num_order][3] && total_arr[pattern_counter][4]==three_step_arr[vul_num_order][4] && total_arr[pattern_counter][5]==three_step_arr[vul_num_order][5] && total_arr[pattern_counter][6]==three_step_arr[vul_num_order][6])
		{
			printf("%d\t\t%d\t\t%s_%s\t\t%s_%s\t\t%s_%s\n", pattern_counter, total_arr[pattern_counter][0], type_match[total_arr[pattern_counter][1]], type_match[total_arr[pattern_counter][2]], type_match[total_arr[pattern_counter][3]], type_match[total_arr[pattern_counter][4]], type_match[total_arr[pattern_counter][5]], type_match[total_arr[pattern_counter][6]]);
				eff_flag = true;
		} else {
			pattern_counter = pattern_counter + 1;
			continue;
		}

		int diff_core_only = 0;
		int skip_case = 0;
		

		// assign steps
		diff_core_only = 2;

		for (int if_plain = 0; if_plain < 2; ++ if_plain)
		{
			case_counter=-1;
		for (int access_1 = 0; access_1 < 2; ++access_1)
		{
			for (int access_2 = 0; access_2 < 2; ++access_2)
			{
				for (int access_3 = 0; access_3 < 2; ++access_3)
				{
					for (int affinity_rel = 0; affinity_rel < diff_core_only; ++affinity_rel)
					{
						skip_case=0;
						case_counter=case_counter+1;

						// assign steps
						int attacker_l_list[3] = {0};
						int victim_l_list[3] = {0};
						int attacker_r_list[3] = {0};
						int victim_r_list[3] = {0};

						int L_list[10] = {A, A_ALIAS, D, U, STAR, INV, A_INV, D_INV, U_INV, A_ALIAS_INV};
						int R_list[5] = {INV, A_INV, D_INV, U_INV, A_ALIAS_INV};
						int lr_num[4]={0};
						int L_1 = 0;
						int L_2 = 1;
					 	int R_1 = 2;
						int R_2 = 3;
						int REST = 4;
						int star_choice = RANDOM_NUM;
						for (int list_num = 0; list_num < 3; ++list_num)
						{
							// inv and flush
							int inner_type=0;
							if (list_num==0) inner_type=access_1;
							else if (list_num==1) inner_type=access_2;
							else if (list_num==2) inner_type=access_3;
							if ((total_arr[pattern_counter][2*list_num+2]==INV||total_arr[pattern_counter][2*list_num+2]==A_INV
								||total_arr[pattern_counter][2*list_num+2]==D_INV||total_arr[pattern_counter][2*list_num+2]==U_INV
								||total_arr[pattern_counter][2*list_num+2]==A_ALIAS_INV)&&inner_type==0/*flush*/){
								if (find_index(L_list, 10, total_arr[pattern_counter][2*list_num+2])!=-1)  {
									if (total_arr[pattern_counter][2*list_num+1]==ATTACKER) {
										lr_num[0]++;
										attacker_l_list[list_num]=list_num+1;
									}
									else if (total_arr[pattern_counter][2*list_num+1]==VICTIM) {
										lr_num[1]++;
										victim_l_list[list_num]=list_num+1;
									}
								}
							} else if ((total_arr[pattern_counter][2*list_num+2]==INV||total_arr[pattern_counter][2*list_num+2]==A_INV
								||total_arr[pattern_counter][2*list_num+2]==D_INV||total_arr[pattern_counter][2*list_num+2]==U_INV
								||total_arr[pattern_counter][2*list_num+2]==A_ALIAS_INV)&&inner_type==1/*remote write*/){
								if (find_index(R_list, 5, total_arr[pattern_counter][2*list_num+2])!=-1)  {
									if (total_arr[pattern_counter][2*list_num+1]==ATTACKER) {
										lr_num[2]++;
										attacker_r_list[list_num]=list_num+1;
									}
									else if (total_arr[pattern_counter][2*list_num+1]==VICTIM) {
										lr_num[3]++;
										victim_r_list[list_num]=list_num+1;
									}
								}
							}
							else if (find_index(L_list, 5, total_arr[pattern_counter][2*list_num+2])!=-1)  {
								if (total_arr[pattern_counter][2*list_num+2]==STAR){
									star_choice = rand()%2;
									if (star_choice==ATTACKER){
										lr_num[0]++;
										attacker_l_list[list_num]=list_num+1;
									} else if (star_choice==VICTIM){
										lr_num[1]++;
										victim_l_list[list_num]=list_num+1;
									} else {
										printf("ERROR: no star matched");
										exit(1);
									}
								} else if (total_arr[pattern_counter][2*list_num+1]==ATTACKER) {
									lr_num[0]++;
									attacker_l_list[list_num]=list_num+1;
								}
								else if (total_arr[pattern_counter][2*list_num+1]==VICTIM) {
									lr_num[1]++;
									victim_l_list[list_num]=list_num+1;
								}
							}
							else if (find_index(R_list, 5, total_arr[pattern_counter][2*list_num+2])!=-1)  {
								if (total_arr[pattern_counter][2*list_num+1]==ATTACKER) {
									lr_num[2]++;
									attacker_r_list[list_num]=list_num+1;
								}
								else if (total_arr[pattern_counter][2*list_num+1]==VICTIM) {
									lr_num[3]++;
									victim_r_list[list_num]=list_num+1;
								}
							}
							else {
								printf("ERROR: variable cannot be matched");
								exit(1);
							}
						}



						char* file_name[50];
						char* plain[50];
						if (if_plain){
							sprintf(plain, "_plain");
						} else {
							sprintf(plain, "");
						}
						sprintf(file_name, "gen_output/%02d_AccOne%d_AccTwo%d_AccThr%d_Aff%d%s_%04d", vul_num_order, access_1, access_2, access_3, affinity_rel, plain, pattern_counter);

						FILE *fout = fopen(strcat(file_name,".c"), "w");
						if(fout == NULL){
				      		printf("\nCould not open file fout\n");
				      		return -1;
				    	}

				    	bool u_last_step=total_arr[pattern_counter][6]==U_INV || total_arr[pattern_counter][6]==U;
				    	

				    	
					    print_pre(fout, l1_cache_size, l1_assoc, l1_cache_line, l1_cache_set, num_test, vic_machine, probe_size, max_array_size, each_run, total_arr[pattern_counter][7], max_cycle, u_last_step);

						fprintf(fout, "int main(int argc, char **argv) {\n");

						fprintf(fout, "  srand(time(NULL));\n");

						fprintf(fout, "  /* get the number of cpu's */\n");
						fprintf(fout, "  cpu_set_t mycpuset;\n");
						fprintf(fout, "  int numcpu = sysconf( _SC_NPROCESSORS_ONLN );\n");
						fprintf(fout, "  int mycpu, cpu;\n");
						fprintf(fout, "  // get our CPU \n");

						fprintf(fout, "  CPU_SET(%d, &mycpuset);\n", cpu[REST]);
						fprintf(fout, "  if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1) {\n");
						fprintf(fout, "        perror(\"sched_setaffinity\");\n");
						fprintf(fout, "    }\n");
						fprintf(fout, "  //print_affinity();\n");
						fprintf(fout, "  //printf(\"sched_getcpu_in_main = %%d\\n\", sched_getcpu());\n");

						fprintf(fout, "     \n");
						fprintf(fout, "  size_t t=0;\n");
						fprintf(fout, "  size_t t0=0;\n");
						fprintf(fout, "  size_t t1=0;\n");
						fprintf(fout, "  size_t t2=0;\n");
						fprintf(fout, "  size_t t3=0;\n");
						fprintf(fout, "  size_t t4=0;\n");
						fprintf(fout, "  size_t t5=0;\n");
						fprintf(fout, "  size_t t6=0;\n");
						fprintf(fout, "  size_t t7=0;\n");
						fprintf(fout, "  size_t t8=0;\n");
						fprintf(fout, "  size_t t9=0;\n");
						fprintf(fout, "  size_t t10=0;\n");
						fprintf(fout, "  size_t t11=0;\n");
						fprintf(fout, "  size_t t12=0;\n");
						fprintf(fout, "  size_t t13=0;\n");
						fprintf(fout, "  size_t t14=0;\n");
						fprintf(fout, "  size_t t15=0;\n");
						fprintf(fout, "  size_t t16=0;\n");
						fprintf(fout, "  size_t t17=0;\n");
						fprintf(fout, "  size_t t18=0;\n");
						fprintf(fout, "  size_t t19=0;\n");
						fprintf(fout, "  size_t t20=0;\n");
						fprintf(fout, "  size_t t21=0;\n");
						fprintf(fout, "  size_t t22=0;\n");
						fprintf(fout, "  size_t t23=0;\n");
						fprintf(fout, "  size_t t24=0;\n");
						fprintf(fout, "  size_t t25=0;\n");
						fprintf(fout, "  size_t t26=0;\n");
						fprintf(fout, "  size_t t27=0;\n");
						fprintf(fout, "  size_t t28=0;\n");
						fprintf(fout, "  size_t t29=0;\n");
						fprintf(fout, "  size_t t30=0;\n");
						fprintf(fout, "  size_t t31=0;\n");
						fprintf(fout, "  size_t t32=0;\n");
						fprintf(fout, "  size_t t_r=0;\n");
						fprintf(fout, "  size_t t_r0=0;\n");
						fprintf(fout, "  size_t t_r1=0;\n");
						fprintf(fout, "  size_t t_r2=0;\n");
						fprintf(fout, "  size_t t_r3=0;\n");
						fprintf(fout, "  size_t t_r4=0;\n");
						fprintf(fout, "  size_t t_r5=0;\n");
						fprintf(fout, "  size_t t_r6=0;\n");
						fprintf(fout, "  size_t t_r7=0;\n");
						fprintf(fout, "  size_t t_r8=0;\n");
						fprintf(fout, "  for (int i = 0; i < %d*%d; ++i)\n", each_run, num_test/2);
						fprintf(fout, "  {\n");
						fprintf(fout, "  	arr1[i] = 0;\n");
						fprintf(fout, "  	arr2[i] = 0;\n");
						fprintf(fout, "  	arr3[i] = 0;\n");
						fprintf(fout, "  	arr4[i] = 0;\n");
						fprintf(fout, "  	arr5[i] = 0;\n");
						fprintf(fout, "  	arr6[i] = 0;\n");
						fprintf(fout, "  }\n");
						fprintf(fout, "  FILE *fp = fopen(strcat(argv[1],\".output\"), \"a\");\n");
						fprintf(fout, "  FILE *fp_res = fopen(strcat(argv[1],\".res\"), \"a\");\n");
						fprintf(fout, "\n");
						fprintf(fout, "  int c;\n");
						fprintf(fout, "  pid_t pid_n, pid_l_1, pid_l_2, pid_r_1, pid_r_2;\n");
						fprintf(fout, "  uint64_t a, b, d, e;\n");
						fprintf(fout, "\n");
						fprintf(fout, "  // Map space for shared array\n");
						fprintf(fout, "  chain_arr = mmap(0, %d*%d*%d*sizeof(char**), PROT_READ|PROT_WRITE,\n", l1_cache_line, l1_assoc, l1_cache_set);
	              		fprintf(fout, "  MAP_SHARED | MAP_ANONYMOUS, -1, 0);\n");
	  					fprintf(fout, "  if (!chain_arr) {\n");
	    				fprintf(fout, "  perror(\"mmap failed for chain_arr\");\n");
	    				fprintf(fout, "  exit(1);\n");
	  					fprintf(fout, "  }\n");
	  					fprintf(fout, "  memset((void *)chain_arr, 0, %d*%d*%d*sizeof(char**));\n", l1_cache_line, l1_assoc, l1_cache_set);
						fprintf(fout, "\n");
						

						print_eviction_set(fout, l1_assoc, probe_size, l1_cache_line, l1_cache_set, pattern_counter);


							void print_step0(int vul_type, bool miss_hit, bool local_u_last_step){
								fprintf(fout, "  if(maintain_arr[%d]==NOONE_RUN){\n", MUTEX_BOUND/2);
		        				fprintf(fout, "  maintain_arr[%d]=STEP0_RUN;\n", MUTEX_BOUND/2);
		      					fprintf(fout, "  }\n");
		      					fprintf(fout, "  while(maintain_arr[%d]!=STEP0_RUN)sched_yield();\n", MUTEX_BOUND/2);

								fprintf(fout, "  		tar_block = rand()%4;\n");
						  	    fprintf(fout, "  		untar_block = rand()%4;\n");
								


								fprintf(fout, "        //if (j==1 && i==299 && m==30) printf(\"sched_getcpu_in_step_1 = %%d\\n\", sched_getcpu()); \n");
								fprintf(fout, " \n");


								fprintf(fout, "		maintain_arr[%d]=STEP1_RUN;\n", MUTEX_BOUND/2);
							}


							void print_step1(int vul_type, bool miss_hit, bool local_u_last_step){
		      					fprintf(fout, "  while(maintain_arr[%d]!=STEP1_RUN)sched_yield();\n", MUTEX_BOUND/2);

								print_step(access_1, vul_type, miss_hit, fout, l1_assoc, false, 1, u_last_step);

								fprintf(fout, "        //if (j==1 && i==299 && m==30) printf(\"sched_getcpu_in_step_1 = %%d\\n\", sched_getcpu()); \n");
								fprintf(fout, " \n");


								fprintf(fout, "		maintain_arr[%d]=STEP2_RUN;\n", MUTEX_BOUND/2);
							}

							void print_step2(int vul_type, bool miss_hit, bool local_u_last_step){
								fprintf(fout, "           while(maintain_arr[%d]!=STEP2_RUN)sched_yield(); \n", MUTEX_BOUND/2);
								
								if (if_plain){
									//
								} else {
									print_step(access_2, vul_type, miss_hit, fout, l1_assoc, false, 2, u_last_step);
								}

								fprintf(fout, " \n");
								fprintf(fout, "          //if (j==1 && i==299 && m==30) printf(\"sched_getcpu_in_step_2 = %%d\\n\", sched_getcpu()); \n");

								fprintf(fout, "          maintain_arr[%d] = STEP3_RUN;\n", MUTEX_BOUND/2);
							}

							void print_step3(int vul_type, bool miss_hit, bool local_u_last_step){
								fprintf(fout, "			while(maintain_arr[%d]!=STEP3_RUN)sched_yield();\n", MUTEX_BOUND/2);

								fprintf(fout, "         printf(\"maintain_arr[6000]=%%d, maintain_arr[6001]=%%d\\n\", maintain_arr[6000], maintain_arr[6001]);	\n");							
									
								print_step(access_3, vul_type, miss_hit, fout, l1_assoc, true, 3, u_last_step);

								
								fprintf(fout, " \n");
								fprintf(fout, "        if ((t+t0+t1+t2+t3+t4+t5+t6+t7+t8+t9+t10+t11+t12+t13+t14+t15+t16+t17+t18+t19+t20+t21+t22+t23+t24+t25+t26+t27+t28+t29+t30+t31+t32)< 30000 && i>NUM_TEST_HALF*0.3){ \n");
								fprintf(fout, "            sum[j]+=(t+t0+t1+t2+t3+t4+t5+t6+t7+t8+t9+t10+t11+t12+t13+t14+t15+t16+t17+t18+t19+t20+t21+t22+t23+t24+t25+t26+t27+t28+t29+t30+t31+t32);//delta; \n");
								fprintf(fout, "            counter[j]+=1; \n");
								fprintf(fout, "          } \n");
								fprintf(fout, "        if((t+t0+t1+t2+t3+t4+t5+t6+t7+t8+t9+t10+t11+t12+t13+t14+t15+t16+t17+t18+t19+t20+t21+t22+t23+t24+t25+t26+t27+t28+t29+t30+t31+t32)<MAX_CYCLE){\n");
							    fprintf(fout, "            histogram[j][(t+t0+t1+t2+t3+t4+t5+t6+t7+t8+t9+t10+t11+t12+t13+t14+t15+t16+t17+t18+t19+t20+t21+t22+t23+t24+t25+t26+t27+t28+t29+t30+t31+t32)]++;\n");
							    fprintf(fout, "          }\n");
							    fprintf(fout, "          else {\n");
							    fprintf(fout, "            histogram[j][MAX_CYCLE-1]++; \n");
							    fprintf(fout, "        }\n");

							    fprintf(fout, "        if(j==0){\n");
						        fprintf(fout, "          arr1[m*NUM_TEST_HALF+i]+=(double)(t+t0+t1+t2+t3+t4+t5+t6+t7+t8+t9+t10+t11+t12+t13+t14+t15+t16+t17+t18+t19+t20+t21+t22+t23+t24+t25+t26+t27+t28+t29+t30+t31+t32);\n");
						        fprintf(fout, "        } else if(j==1){\n");
						        fprintf(fout, "          arr2[m*NUM_TEST_HALF+i]+=(double)(t+t0+t1+t2+t3+t4+t5+t6+t7+t8+t9+t10+t11+t12+t13+t14+t15+t16+t17+t18+t19+t20+t21+t22+t23+t24+t25+t26+t27+t28+t29+t30+t31+t32);\n");
						        fprintf(fout, "        } else if(j==2){\n");
						        fprintf(fout, "          arr3[m*NUM_TEST_HALF+i]+=(double)(t+t0+t1+t2+t3+t4+t5+t6+t7+t8+t9+t10+t11+t12+t13+t14+t15+t16+t17+t18+t19+t20+t21+t22+t23+t24+t25+t26+t27+t28+t29+t30+t31+t32);\n");
						        fprintf(fout, "        }\n");


				        		fprintf(fout, "				if(round_rand==0)\n");
								fprintf(fout, "		        {\n");
								fprintf(fout, "		        	if(j==0){\n");
								fprintf(fout, "			          arr4[m*NUM_TEST_HALF+i]+=(double)(t_r+t_r0+t_r1+t_r2+t_r3+t_r4+t_r5+t_r6+t_r7+t_r8);\n");
								fprintf(fout, "			        } else if(j==1){\n");
								fprintf(fout, "			          arr5[m*NUM_TEST_HALF+i]+=(double)(t_r+t_r0+t_r1+t_r2+t_r3+t_r4+t_r5+t_r6+t_r7+t_r8);\n");
								fprintf(fout, "			        } else if(j==2){\n");
								fprintf(fout, "			          arr6[m*NUM_TEST_HALF+i]+=(double)(t_r+t_r0+t_r1+t_r2+t_r3+t_r4+t_r5+t_r6+t_r7+t_r8);\n");
								fprintf(fout, "			        }\n");
								fprintf(fout, "		        } else {\n");
								fprintf(fout, "		        	if(j==0){\n");
								fprintf(fout, "			          arr4[m*NUM_TEST_HALF+i]+=(double)(t_r+t_r0+t_r1+t_r2+t_r3+t_r4+t_r5+t_r6+t_r7+t_r8-arr1[m*NUM_TEST_HALF+i]);\n");
								fprintf(fout, "			        } else if(j==1){\n");
								fprintf(fout, "			          arr5[m*NUM_TEST_HALF+i]+=(double)(t_r+t_r0+t_r1+t_r2+t_r3+t_r4+t_r5+t_r6+t_r7+t_r8-arr2[m*NUM_TEST_HALF+i]);\n");
								fprintf(fout, "			        } else if(j==2){\n");
								fprintf(fout, "			          arr6[m*NUM_TEST_HALF+i]+=(double)(t_r+t_r0+t_r1+t_r2+t_r3+t_r4+t_r5+t_r6+t_r7+t_r8-arr3[m*NUM_TEST_HALF+i]);\n");
								fprintf(fout, "			        }\n");
								fprintf(fout, "		        }\n");


								fprintf(fout, "        //if (j==1 && i==299 && m==30) printf(\"sched_getcpu_in_step_3 = %%d\\n\", sched_getcpu()); \n");

								fprintf(fout, "        maintain_arr[%d]=STEP0_RUN;\n", MUTEX_BOUND/2);
							}

							void print_timing(if_pre){
								if (if_pre){
									fprintf(fout, "  		} \n");
								}
								fprintf(fout, "  	} \n");
								fprintf(fout, "  } \n");

							
									
								if (if_plain==0){
									
									fprintf(fout, "        if (m==10) {\n"); 
									fprintf(fout, "  			for(int i=0; i< PROBE_SIZE; i++){ \n");
									fprintf(fout, "    				fprintf(fp, \"%%lf\\t\", (double)(sum[i])/counter[i]);\n"); 
									fprintf(fout, "  			} \n");
									fprintf(fout, "  		} \n");
									fprintf(fout, " \n");
								}

								
								fprintf(fout, "  } \n");
								fprintf(fout, " printf(\"%%lu %%lu\\n\", result[0], result[1]);\n");
								if (if_plain==0){
									fprintf(fout, " fprintf(fp, \"%%lu %%lu \\n\", result[0], result[1]);\n"); 
								}
								
								fprintf(fout, " if (2*result[0]>result[1])\n");
								fprintf(fout, "   {\n");

								fprintf(fout, "   }\n");

								fprintf(fout, "     printf(\"cycles\\ta\\ta_alias\\tNIB\\n\");\n");
								fprintf(fout, "     for(int i=0;i<MAX_CYCLE;i++){\n");
								fprintf(fout, "           printf(\"%%d\\t%%d\\t%%d\\t%%d\\n\", i, histogram[0][i], histogram[1][i], histogram[2][i]);   \n");
								fprintf(fout, "     }\n");


								// start calculate the p value of mapped and not mapped data
							    fprintf(fout, "     \n");
							    fprintf(fout, "      double pvalue_a_a_alias = Pvalue(arr1,NUM_TEST_HALF*EACH_RUN,arr2,NUM_TEST_HALF*EACH_RUN);\n");
							    fprintf(fout, "      double pvalue_a_NIB = Pvalue(arr1,NUM_TEST_HALF*EACH_RUN,arr3,NUM_TEST_HALF*EACH_RUN);\n");
							    fprintf(fout, "      double pvalue_a_alias_NIB = Pvalue(arr2,NUM_TEST_HALF*EACH_RUN,arr3,NUM_TEST_HALF*EACH_RUN);\n");
							    fprintf(fout, "     \n");

							    fprintf(fout, "      double pvalue_a_a_alias_2 = Pvalue(arr4,NUM_TEST_HALF*EACH_RUN,arr5,NUM_TEST_HALF*EACH_RUN);\n");
							    fprintf(fout, "      double pvalue_a_NIB_2 = Pvalue(arr4,NUM_TEST_HALF*EACH_RUN,arr6,NUM_TEST_HALF*EACH_RUN);\n");
							    fprintf(fout, "      double pvalue_a_alias_NIB_2 = Pvalue(arr5,NUM_TEST_HALF*EACH_RUN,arr6,NUM_TEST_HALF*EACH_RUN);\n");
							    fprintf(fout, "     \n");

							    char* type_match[14] = {"ATT","VIC","A","A_ALIAS","D","U","INV","A_INV","D_INV","U_INV","A_ALIAS_INV","ERROR1", "ERROR2", "STAR"};

							    

							    fprintf(fout, "      fprintf(fp, \"%d\\t%s_%s\\t%s_%s\\t%s_%s\\n\");\n", total_arr[pattern_counter][0], type_match[total_arr[pattern_counter][1]], type_match[total_arr[pattern_counter][2]], type_match[total_arr[pattern_counter][3]], type_match[total_arr[pattern_counter][4]], type_match[total_arr[pattern_counter][5]], type_match[total_arr[pattern_counter][6]]);
								fprintf(fout, "      fprintf(fp, \"%%f\\t%%f\\t%%f\\t%%f\\t%%f\\t%%f\\n\", pvalue_a_a_alias, pvalue_a_NIB, pvalue_a_alias_NIB, pvalue_a_a_alias_2, pvalue_a_NIB_2, pvalue_a_alias_NIB_2);\n");
								   
							    fprintf(fout, "     \n");
							    fprintf(fout, "    \n"); 

								fprintf(fout, "    if (((pvalue_a_a_alias<0.0005 && pvalue_a_NIB<0.0005)||(pvalue_a_a_alias<0.0005 && pvalue_a_alias_NIB<0.0005)||(pvalue_a_alias_NIB<0.0005 && pvalue_a_NIB<0.0005))&&((!u_last_step) ||((pvalue_a_a_alias_2<0.0005 && pvalue_a_NIB_2<0.0005)||(pvalue_a_a_alias_2<0.0005 && pvalue_a_alias_NIB_2<0.0005)||(pvalue_a_alias_NIB_2<0.0005 && pvalue_a_NIB_2<0.0005)))) \n");
								fprintf(fout, "   {\n");
								if (if_plain==0){
									fprintf(fout, "     fprintf(fp, \"\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t%s Succeed \\n\");\n", file_name);
									if(skip_case==1){
										//
									}
									else if(affinity_rel==0 && (vic_machine==WKST5 || vic_machine==BROADWELL || vic_machine==AMDLOCAL)){
										//
									}
									else { 
                                            fprintf(fout, "     fprintf(fp_res, \"1\");\n");
									}	
							    	
								} else {
									fprintf(fout, "     fprintf(fp, \"%s Succeeds \\n\\n\");\n", file_name);
									if(skip_case==1){
										fprintf(fout, "     fprintf(fp_res, \"222\\n\");\n");
									}
									else if(affinity_rel==0 && (vic_machine==WKST5 || vic_machine==BROADWELL || vic_machine==AMDLOCAL)){
										fprintf(fout, "     fprintf(fp_res, \"111\\n\");\n");
									}
									else { 
                                            fprintf(fout, "     fprintf(fp_res, \"1\\n\");\n");	
									}
									
								}
								fprintf(fout, "   } else {\n");
								if (if_plain==0){
									fprintf(fout, "     fprintf(fp, \"\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t\\t%s Fails \\n\");\n", file_name);
									if(skip_case==1){
										//
									}
									else if(affinity_rel==0 && (vic_machine==WKST5 || vic_machine==BROADWELL || vic_machine==AMDLOCAL)){
										//
									}
									else { 
                                            fprintf(fout, "     fprintf(fp_res, \"0\");\n");
									}
								} else {
									fprintf(fout, "     //\n", file_name);
									if(skip_case==1){
										fprintf(fout, "     fprintf(fp_res, \"222\\n\");\n");
									}
									else if(affinity_rel==0 && (vic_machine==WKST5 || vic_machine==BROADWELL || vic_machine==AMDLOCAL)){
										fprintf(fout, "     fprintf(fp_res, \"111\\n\");\n");
									}
									else { 
                                            fprintf(fout, "     fprintf(fp_res, \"0\\n\");\n");	
									}
								}
								fprintf(fout, "   }\n");

								if(if_plain==1 && access_1==1 && access_2==1 && access_3==1 && ((diff_core_only==2 && affinity_rel==1)||(diff_core_only==1 && affinity_rel==0))){
									fprintf(fout, "     fprintf(fp_res, \"\\n\");\n");
								}


								fprintf(fout, "  fclose(fp);  \n");
								fprintf(fout, "  fclose(fp_res);  \n");
							}
						
							fprintf(fout, " maintain_arr[%d] = NOONE_RUN; \n", MUTEX_BOUND/2);

							fprintf(fout, " // local attacker\n");
							fprintf(fout, " if ((pid_l_1 = fork()) < 0) {\n");
                            fprintf(fout, "     printf(\"Failed to fork process 1\\n\");\n");
                            fprintf(fout, "     exit(1);\n");
                            fprintf(fout, " }\n");
                            fprintf(fout, " else if (pid_l_1 == 0) {\n");
                            fprintf(fout, "          CPU_ZERO(&mycpuset); \n");
                            if(diff_core_only==2 && lr_num[0]!=0 && lr_num[1]!=0)
								fprintf(fout, "          CPU_SET(%d, &mycpuset); \n", cpu[L_1+affinity_rel]);
							else fprintf(fout, "          CPU_SET(%d, &mycpuset); \n", cpu[L_1]);
							fprintf(fout, "          //set processor affinity \n");
							fprintf(fout, "  	if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1) {\n");
							fprintf(fout, "        perror(\"sched_setaffinity\");\n");
							fprintf(fout, "    }\n");
							fprintf(fout, " \n");

							fprintf(fout, " for (int m = 0; m < EACH_RUN; m++) \n");
							fprintf(fout, " { \n");
							fprintf(fout, "  for (int e = 0; e < PROBE_SIZE; ++e)\n");
							fprintf(fout, "   {\n");
							fprintf(fout, "     sum[e] = 0;\n");
							fprintf(fout, "     counter[e] = 0;\n");
							fprintf(fout, "   }\n");
							fprintf(fout, "    //probe = probe + 0x10*j; \n");
							fprintf(fout, "    for (int i = 0; i < NUM_TEST_HALF; ++i) \n");
							fprintf(fout, "    { \n");
							fprintf(fout, "    for (int round_rand = 0; round_rand < 2; ++round_rand) \n");
							fprintf(fout, "    	{ \n");
							fprintf(fout, "  	for (int j = 0; j < PROBE_SIZE; ++j) \n");
							fprintf(fout, "  		{ \n");
							fprintf(fout, "           \n");
								

							print_step0(PRE, 0, false);
							for (int list_num = 0; list_num < 3; ++list_num)
							{
								if (attacker_l_list[list_num]==list_num+1){
									if (list_num+1==1) print_step1(total_arr[pattern_counter][2], total_arr[pattern_counter][7], u_last_step);
									else if (list_num+1==2) print_step2(total_arr[pattern_counter][4], total_arr[pattern_counter][7], u_last_step);
									else if (list_num+1==3) {
										print_step3(total_arr[pattern_counter][6], total_arr[pattern_counter][7], u_last_step);
										print_timing(true);
									}
								}
								else if (list_num==2){
									fprintf(fout, "    		}}	 \n");
									fprintf(fout, "       }} \n");
								}
							}

                            fprintf(fout, "     exit(0);\n");
                            fprintf(fout, " }\n");
                            fprintf(fout, " // local victim\n");
                            fprintf(fout, " if ((pid_l_2 = fork()) < 0) {\n");
                            fprintf(fout, "     printf(\"Failed to fork process 2\\n\");\n");
                            fprintf(fout, "     exit(1);\n");
                            fprintf(fout, " }\n");
                            fprintf(fout, " else if (pid_l_2 == 0) {\n");
                            fprintf(fout, "          CPU_ZERO(&mycpuset); \n");
							fprintf(fout, "          CPU_SET(%d, &mycpuset); \n", cpu[L_2]);
							fprintf(fout, "          //set processor affinity \n");
							fprintf(fout, "  	if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1) {\n");
							fprintf(fout, "        perror(\"sched_setaffinity\");\n");
							fprintf(fout, "    }\n");
							fprintf(fout, " \n");

							fprintf(fout, " for (int m = 0; m < EACH_RUN; m++) \n");
							fprintf(fout, " { \n");
							fprintf(fout, "  for (int e = 0; e < PROBE_SIZE; ++e)\n");
							fprintf(fout, "   {\n");
							fprintf(fout, "     sum[e] = 0;\n");
							fprintf(fout, "     counter[e] = 0;\n");
							fprintf(fout, "   }\n");
							fprintf(fout, "    //probe = probe + 0x10*j; \n");
							fprintf(fout, "    for (int i = 0; i < NUM_TEST_HALF; ++i) \n");
							fprintf(fout, "    { \n");
								
							fprintf(fout, "    for (int round_rand = 0; round_rand < 2; ++round_rand) \n");
							fprintf(fout, "    	{ \n");
							fprintf(fout, "  	for (int j = 0; j < PROBE_SIZE; ++j) \n");
							fprintf(fout, "  		{ \n");
							fprintf(fout, "           \n");

							for (int list_num = 0; list_num < 3; ++list_num)
							{
								if (victim_l_list[list_num]==list_num+1){
									if (list_num+1==1) print_step1(total_arr[pattern_counter][2], total_arr[pattern_counter][7], u_last_step);
									else if (list_num+1==2) print_step2(total_arr[pattern_counter][4], total_arr[pattern_counter][7], u_last_step);
									else if (list_num+1==3) {
										print_step3(total_arr[pattern_counter][6], total_arr[pattern_counter][7], u_last_step);
										print_timing(true);
									}
								} else if(list_num==2){
									fprintf(fout, "    }}	 \n");
									fprintf(fout, "  }}	 \n");
								}
							}

                            fprintf(fout, "     exit(0);\n");
                            fprintf(fout, " }\n");
                            fprintf(fout, " // remote attacker\n");
                            fprintf(fout, " if ((pid_r_1 = fork()) < 0) {\n");
                            fprintf(fout, "     printf(\"Failed to fork process 3\\n\");\n");
                            fprintf(fout, "     exit(1);\n");
                            fprintf(fout, " }\n");
                            fprintf(fout, " else if (pid_r_1 == 0) {\n");
                            fprintf(fout, "          CPU_ZERO(&mycpuset); \n");
                            if(diff_core_only==2 && lr_num[2]!=0 && lr_num[3]!=0)
								fprintf(fout, "          CPU_SET(%d, &mycpuset); \n", cpu[R_1+affinity_rel]);
							else fprintf(fout, "          CPU_SET(%d, &mycpuset); \n", cpu[R_1]);
								fprintf(fout, "          //set processor affinity \n");
							fprintf(fout, "  	if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1) {\n");
							fprintf(fout, "        perror(\"sched_setaffinity\");\n");
							fprintf(fout, "    }\n");
							fprintf(fout, " \n");

							fprintf(fout, " for (int m = 0; m < EACH_RUN; m++) \n");
							fprintf(fout, " { \n");
							fprintf(fout, "  for (int e = 0; e < PROBE_SIZE; ++e)\n");
							fprintf(fout, "   {\n");
							fprintf(fout, "     sum[e] = 0;\n");
							fprintf(fout, "     counter[e] = 0;\n");
							fprintf(fout, "   }\n");
							fprintf(fout, "    //probe = probe + 0x10*j; \n");
							fprintf(fout, "    for (int i = 0; i < NUM_TEST_HALF; ++i) \n");
							fprintf(fout, "    { \n");
								
							fprintf(fout, "    for (int round_rand = 0; round_rand < 2; ++round_rand) \n");
							fprintf(fout, "    	{ \n");
							fprintf(fout, "  	for (int j = 0; j < PROBE_SIZE; ++j) \n");
							fprintf(fout, "  		{ \n");
							fprintf(fout, "           \n");


							for (int list_num = 0; list_num < 3; ++list_num)
							{
								if (attacker_r_list[list_num]==list_num+1){
									if (list_num+1==1) print_step1(total_arr[pattern_counter][2], total_arr[pattern_counter][7], u_last_step);
									else if (list_num+1==2) print_step2(total_arr[pattern_counter][4], total_arr[pattern_counter][7], u_last_step);
									else if (list_num+1==3) {
										print_step3(total_arr[pattern_counter][6], total_arr[pattern_counter][7], u_last_step);
										print_timing(true);
									}
								}
								else if (list_num==2){
									fprintf(fout, "    		}}	 \n");
									fprintf(fout, "       }} \n");
								}
							}

                            fprintf(fout, "     exit(0);\n");
                            fprintf(fout, " }\n");
                            fprintf(fout, " // remote victim\n");
                            fprintf(fout, " if ((pid_r_2 = fork()) < 0) {\n");
                            fprintf(fout, "     printf(\"Failed to fork process 4\\n\");\n");
                            fprintf(fout, "     exit(1);\n");
                            fprintf(fout, " }\n");
                            fprintf(fout, " else if (pid_r_2 == 0) {\n");
                            fprintf(fout, "          CPU_ZERO(&mycpuset); \n");
							fprintf(fout, "          CPU_SET(%d, &mycpuset); \n", cpu[R_2]);
							fprintf(fout, "          //set processor affinity \n");
							fprintf(fout, "  	if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mycpuset) == -1) {\n");
							fprintf(fout, "        perror(\"sched_setaffinity\");\n");
							fprintf(fout, "    }\n");
							fprintf(fout, " \n");

							fprintf(fout, " for (int m = 0; m < EACH_RUN; m++) \n");
							fprintf(fout, " { \n");
							fprintf(fout, "  for (int e = 0; e < PROBE_SIZE; ++e)\n");
							fprintf(fout, "   {\n");
							fprintf(fout, "     sum[e] = 0;\n");
							fprintf(fout, "     counter[e] = 0;\n");
							fprintf(fout, "   }\n");
							fprintf(fout, "    //probe = probe + 0x10*j; \n");
							fprintf(fout, "    for (int i = 0; i < NUM_TEST_HALF; ++i) \n");
							fprintf(fout, "    { \n");
								
							fprintf(fout, "    for (int round_rand = 0; round_rand < 2; ++round_rand) \n");
							fprintf(fout, "    	{ \n");
							fprintf(fout, "  	for (int j = 0; j < PROBE_SIZE; ++j) \n");
							fprintf(fout, "  		{ \n");
							fprintf(fout, "           \n");


							for (int list_num = 0; list_num < 3; ++list_num)
							{
								if (victim_r_list[list_num]==list_num+1){
									if (list_num+1==1) print_step1(total_arr[pattern_counter][2], total_arr[pattern_counter][7], u_last_step);
									else if (list_num+1==2) print_step2(total_arr[pattern_counter][4], total_arr[pattern_counter][7], u_last_step);
									else if (list_num+1==3) {
										print_step3(total_arr[pattern_counter][6], total_arr[pattern_counter][7], u_last_step);
										print_timing(true);
									}
								} else if(list_num==2){
									fprintf(fout, "    }}	 \n");
									fprintf(fout, "  }}	 \n");
								}
							}

                            fprintf(fout, "     exit(0);\n");
                            fprintf(fout, " }\n");

							fprintf(fout, "} \n");

							fclose(fout);

					}
				}
			}
		}
		}
	pattern_counter = pattern_counter + 1;
	}
	}
	} 
	}
	}
	

	return 0;
}
