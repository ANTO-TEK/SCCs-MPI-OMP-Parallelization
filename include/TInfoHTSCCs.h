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
 * @file TInfoHTSCCs.h
 * 
 * @brief This file defines the struct and functions used for handling key-value pairs in the 
 * THTSCCs hash table.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef TINFOHTSCCS_H
#define TINFOHTSCCS_H

#include <stdbool.h>

#include "TArray.h"

/**
 * @brief TKeyHTSCCs is the type used for the key of the key-value pair in the THTSCCs hash table.
*/
typedef int TKeyHTSCCs;

/**
 * @brief TValueHTSCCs is the type used for the value of the key-value pair in the THTSCCs hash table.
*/
typedef TArray TValueHTSCCs;

/**
 * @brief TInfoHTSCCs is the struct that contains a key-value pair in the THTSCCs hash table.
*/
typedef struct SInfoHTSCCs {
	TKeyHTSCCs key;
	TValueHTSCCs value;
} TInfoHTSCCs;

/**
 * @brief Prints the key-value pair in TInfoHTSCCs.
*/
void infoPrintHTSCCs(TInfoHTSCCs);

/**
 * @brief Compares two key-value pairs in TInfoHTSCCs for equality.
*/
bool infoEqualHTSCCs(TInfoHTSCCs, TInfoHTSCCs);

/**
 * @brief Compares two key-value pairs in TInfoHTSCCs for less than.
*/
bool infoLessHTSCCs(TInfoHTSCCs, TInfoHTSCCs);

/**
 * @brief Compares two key-value pairs in TInfoHTSCCs for greater than.
*/
bool infoGreaterHTSCCs(TInfoHTSCCs, TInfoHTSCCs);

/**
 * @brief Generates a hash value for the key in TInfoHTSCCs.
*/
unsigned hashHTSCCs(TKeyHTSCCs);

#endif 