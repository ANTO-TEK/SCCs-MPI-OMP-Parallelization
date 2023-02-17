/* 
 * Course: High Performance Computing 2022/2023
 * 
 * Lecturer: Francesco Moscato	fmoscato@unisa.it
 *
 * Group:
 * Amato Emilio        0622701903      e.amato16@studenti.unisa.it             
 * Bove Antonio        0622701898      a.bove57@studenti.unisa.it 
 * De Gregorio Marco   0622701865      m.degregorio19@studenti.unisa.it
 * 
 * Source Code for sequential version:
 * https://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/
 *
 * Copyright (C) 2023 - All Rights Reserved
 *
 * This file is part of FinalProjectHPC.
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation, either version 
 * 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with ContestOMP. 
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *                                  REQUIREMENTS OF THE ASSIGNMENT 
 * 
 * Provide a parallel version of the Tarjan's algorithm to find Strongly Connected Components in 
 * a Graph. The implementation MUST use an hybrid message passing / shared memory paradigm. and 
 * has to be implemented by using MPI and openMP. Students MUST provide parallel processes on 
 * different nodes, and each process has to be parallelized by using Open MP (i.e.: MPI will spawn 
 * OPENMP-compiled processes). Students can choose the graph allocation method the prefer. They can 
 * eventually produce the graph directly in distributed memory (without store anything). Good Graph 
 * dimensions are greater than 4GB of data.
 * 
 * @file TInfoInt.c
 * 
 * @brief Header file for functions that operate on values of type TInfoInt. This file contains the 
 * function prototypes for four functions that operate on values of type TInfoInt: infoEqualInt, 
 * infoGreaterInt, infoLessInt, and infoPrintInt.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>

#include "../include/TInfoInt.h"

/**
 * Compares two TInfoInt values for equality.
 * This function compares two TInfoInt values and returns 1 if they are equal,
 * and 0 if they are not.
 *
 * @param info1 The first TInfoInt value to compare.
 * @param info2 The second TInfoInt value to compare.
 * @return 1 if the values are equal, 0 if they are not.
 */
int infoEqualInt(TInfoInt info1, TInfoInt info2) {
	if(info1 == info2) return 1;
	return 0;
}

/**
 * Determines if one TInfoInt value is greater than another.
 * This function compares two TInfoInt values and returns 1 if the first is
 * greater than the second, and 0 if it is not.
 *
 * @param info1 The first TInfoInt value to compare.
 * @param info2 The second TInfoInt value to compare.
 * @return 1 if the first value is greater than the second, 0 if it is not.
 */
int infoGreaterInt(TInfoInt info1, TInfoInt info2) {
	return info1 > info2;
}

/**
 * Determines if one TInfoInt value is less than another.
 * This function compares two TInfoInt values and returns 1 if the first is
 * less than the second, and 0 if it is not.
 *
 * @param info1 The first TInfoInt value to compare.
 * @param info2 The second TInfoInt value to compare.
 * @return 1 if the first value is less than the second, 0 if it is not.
 */
int infoLessInt(TInfoInt info1, TInfoInt info2) {
	return info1 < info2;
}

/**
 * Prints a TInfoInt value to the console.
 * This function prints the given TInfoInt value to the console.
 *
 * @param info The TInfoInt value to print.
 * @return 
 */
void infoPrintInt(TInfoInt info) {
	printf("%d ", info);
}