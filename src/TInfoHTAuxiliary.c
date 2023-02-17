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
 * @file TInfoHTAuxiliary.c
 * 
 * @brief This file contains the implementation of the auxiliary info type for the THTAuxiliarySCC 
 * hash table.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>

#include "../include/TInfoHTAuxiliary.h"

/**
* Prints the auxiliary info.
*
* @param info The auxiliary info to be printed
*/
void infoPrintHTAuxiliary(TInfoHTAuxiliary info) {
	printf("(%d: %d, %d, %d) ", info.key, info.value.disc, info.value.low, info.value.stackMember);
}

/**
* Compares two auxiliary infos.
*
* @param a The first auxiliary info
* @param b The second auxiliary info
* @return True if the auxiliary infos are equal, false otherwise
*/
bool infoEqualHTAuxiliary(TInfoHTAuxiliary a, TInfoHTAuxiliary b) {
	return a.key == b.key;
}

/**
* Compares two auxiliary infos in order to determine which one is less.
*
* @param a The first auxiliary info
* @param b The second auxiliary info
* @return True if the first auxiliary info is less than the second one, false otherwise
*/
bool infoLessHTAuxiliary(TInfoHTAuxiliary a, TInfoHTAuxiliary b) {
	return a.key < b.key;
}

/**
* Compares two auxiliary infos in order to determine which one is greater.
*
* @param a The first auxiliary info
* @param b The second auxiliary info
* @return True if the first auxiliary info is greater than the second one, false otherwise
*/
bool infoGreaterHTAuxiliary(TInfoHTAuxiliary a, TInfoHTAuxiliary b) {
	return a.key > b.key;
}

/**
* Returns the hash value of the auxiliary info.

* @param info The auxiliary info
* @return The hash value of the auxiliary info
*/
unsigned hashHTAuxiliary(TKeyHTAuxiliary key) {
	return (unsigned)key;
}
