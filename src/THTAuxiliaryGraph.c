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
 * @file THTAuxiliaryGraph.c
 * 
 * @brief This is an implementation of a hash table with linear probing and open addressing. The code 
 * includes functions for creating and destroying the hash table, inserting and deleting key-value pairs, 
 * and searching for keys in the table. 
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/THTAuxiliaryGraph.h"

/**
* Creates a new hash table with n entries in the bucket array.
*
* @param n The number of entries in the bucket array
* @return A pointer to the new hash table
*/
THTAuxiliaryGraph* HTAuxiliaryGraphCreate(int n) {
	THTAuxiliaryGraph* ht = malloc(sizeof(THTAuxiliaryGraph));
	assert(ht != NULL);

	ht->bucket = malloc(n * sizeof(TInfoHTAuxiliaryGraph));
	ht->used = malloc(n * sizeof(int));
	assert(ht->bucket != NULL && ht->used != NULL);

	for (int i = 0; i < n; i++)
		ht->used[i] = 0;
	ht->n_bucket = n;
	ht->n_used = 0;
	return ht;
}

/**
* Destroys the hash table.
*
*@param ht The hash table
*/
void HTAuxiliaryGraphDestroy(THTAuxiliaryGraph* ht) {
	free(ht->bucket);
	free(ht->used);
	free(ht);
}

/**
* Inserts a new key-value pair in the hash table.
*
*@param ht The hash table
*@param key The key to hash
*@param value The value to insert
*/
void HTAuxiliaryGraphInsert(THTAuxiliaryGraph* ht, TKeyHTAuxiliaryGraph key, TValueHTAuxiliaryGraph value) {
	TValueHTAuxiliaryGraph* p = HTAuxiliaryGraphSearch(ht, key);
	if (p != NULL) { // si aggiorna solo il valore
		*p = value;
		return;
	}

	if (ht->n_used + 1 >= ht->n_bucket * MAX_LOAD) {
		HTAuxiliaryGraphResize(ht, ht->n_bucket * GROW_FACTOR + 1);
	}

	unsigned h = hashHTAuxiliaryGraph(key) % ht->n_bucket;
	while (ht->used[h])
		h = (h + 1) % ht->n_bucket;
	ht->bucket[h].key = key;
	ht->bucket[h].value = value;
	ht->used[h] = 1;
	ht->n_used++;
}

/**
* Searches for a key in the hash table.
*
*@param ht The hash table
*@param key The key to search
*@return A pointer to the value associated with the key, or NULL if the key is not found
*/
TValueHTAuxiliaryGraph* HTAuxiliaryGraphSearch(THTAuxiliaryGraph* ht, TKeyHTAuxiliaryGraph key) {
	unsigned h = hashHTAuxiliaryGraph(key) % ht->n_bucket;
	TInfoHTAuxiliaryGraph info = { key };
	while (ht->used[h] && !infoEqualHTAuxiliaryGraph(info, ht->bucket[h]))
		h = (h + 1) % ht->n_bucket;
	if (ht->used[h])
		return &ht->bucket[h].value;
	return NULL;
}

/**
* Deletes a key-value pair from the hash table.
*
* @param ht The hash table
* @param key The key to delete
*/
void HTAuxiliaryGraphDelete(THTAuxiliaryGraph* ht, TKeyHTAuxiliaryGraph key) {
	unsigned h = hashHTAuxiliaryGraph(key) % ht->n_bucket;
	TInfoHTAuxiliaryGraph info = { key };
	while (ht->used[h] && !infoEqualHTAuxiliaryGraph(info, ht->bucket[h]))
		h = (h + 1) % ht->n_bucket;
	if (ht->used[h]) { 
		unsigned hole = h;
		ht->used[hole] = 0; 
		ht->n_used--;
		h = (h + 1) % ht->n_bucket;
		while (ht->used[h]) { 
			unsigned h1 = hashHTAuxiliaryGraph(ht->bucket[h].key) % ht->n_bucket;
			if ((h > hole && (h1 <= hole || h1 > h)) || (h < hole && h1 > h && h1 <= hole)) {
				
				ht->bucket[hole] = ht->bucket[h]; 
				ht->used[hole] = 1;
				hole = h; 
				ht->used[hole] = 0;
			}
			h = (h + 1) % ht->n_bucket;
		}
	}
}

/**
* Resizes the hash table.
*
* @param ht The hash table
* @param n The new size of the bucket array
*/
void HTAuxiliaryGraphResize(THTAuxiliaryGraph* ht, int n) {
	TInfoHTAuxiliaryGraph* bucket = ht->bucket;
	int* used = ht->used;
	int n_bucket = ht->n_bucket;

	ht->bucket = malloc(n * sizeof(TInfoHTAuxiliaryGraph));
	ht->used = malloc(n * sizeof(int));
	assert(ht->bucket != NULL && ht->used != NULL);

	for (int i = 0; i < n; i++)
		ht->used[i] = 0;
	ht->n_bucket = n;
	ht->n_used = 0;

	for (int i = 0; i < n_bucket; i++)
		if (used[i])
			HTAuxiliaryGraphInsert(ht, bucket[i].key, bucket[i].value);

	free(bucket);
	free(used);
}

/**
* Prints the hash table.
*
* @param ht he hash table
*/
void HTAuxiliaryGraphPrint(THTAuxiliaryGraph* ht) {
	for (int i = 0; i < ht->n_bucket; i++)
		if (ht->used[i])
			infoPrintHTAuxiliaryGraph(ht->bucket[i]);
}