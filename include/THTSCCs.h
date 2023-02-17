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
 * @file THTauxiliarySCC.h
 * 
 * @brief Header file for a hash table with open addressing and linear probing. This file contains 
 * the declarations of the functions and structures used to create and manipulate a hash table with 
 * open addressing and linear probing. The hash table is composed by an array of pointers to TInfoHTSCCs 
 * structures, an array of integers indicating which buckets are used, and two integers representing the 
 * number of buckets and the number of used buckets.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef THTSCCS_H
#define THTSCCS_H

#include "TInfoHTSCCs.h"

/**
 * @brief Maximum load factor of the hash table
 * The maximum load factor is the maximum ratio between the number of used buckets and the number 
 * of total buckets in the hash table. When this ratio is reached, the hash table is resized to a 
 * larger size.
 */
#define MAX_LOAD 0.75

/**
 * @brief Factor to grow the hash table when it reaches the maximum load
 * When the hash table reaches the maximum load, it is resized to a larger size by multiplying its 
 * current number of buckets by this factor.
 */
#define GROW_FACTOR 2

/**
 * @brief Structure to represent a hash table with open addressing and linear probing
 * This structure represents a hash table with open addressing and linear probing, composed by:
 * • bucket: an array of TInfoHTSCCs pointers;
 * • used: an array of integers, where 0 means that the bucket is empty and 1 means that the bucket 
 *   is used;
 * • n_bucket: the number of buckets;
 * • n_used: the number of used buckets.
 */
typedef struct SHTSCCs {
	TInfoHTSCCs* bucket; 
	int* used;
	int n_bucket;
	int n_used;
} THTSCCs;

/**
 * @brief Creates a new hash table with open addressing and linear probing
 * This function creates and returns a pointer to a new hash table with open addressing and linear 
 * probing, initialized with a number of buckets equal to the parameter n.
 */
THTSCCs* HTSCCsCreate(int);

/**
 * @brief Deallocates the memory used by a hash table with open addressing and linear probing
 * This function deallocates the memory used by the hash table pointed to by ht.
 */
void HTSCCsDestroy(THTSCCs*);

/**
 * @brief Deletes a key-value pair from the hash table
 * This function deletes the key-value pair with the specified key from the hash table pointed 
 * to by ht.
 */
void HTSCCsInsert(THTSCCs*, TKeyHTSCCs, TValueHTSCCs);

/**
 * @brief Deletes a key-value pair from the hash table
 * This function deletes the key-value pair with the specified key from the hash table pointed to by ht.
 */
void HTSCCsDelete(THTSCCs*, TKeyHTSCCs);

/**
 * @brief Searches for a value in the hash table based on its key
 * This function searches for the value corresponding to the specified key in the hash table pointed 
 * to by ht. If the key is found, a pointer to the corresponding value is returned. If the key is not 
 * found, the function returns NULL.
 */
TValueHTSCCs* HTSCCsSearch(THTSCCs*, TKeyHTSCCs);

/**
 * @brief Prints the contents of the hash table
 * This function prints the contents of the hash table pointed to by ht to the standard output.
 */
void HTSCCsPrint(THTSCCs*);

/**
 * @brief Resizes the hash table to a larger size
 * This function resizes the hash table pointed to by ht to a larger size by multiplying its current number 
 * of buckets by the GROW_FACTOR macro.
 */
void HTSCCsResize(THTSCCs*, int);

#endif 
 
