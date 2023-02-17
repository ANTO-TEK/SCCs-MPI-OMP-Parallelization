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
 * @file TArray.c
 * 
 * @brief Source file for the TArray module. The TArray module provides functions for creating, 
 * manipulating, and destroying dynamic arrays of TInfoInt objects.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "../include/TArray.h"

#define C_EXP 5 //  EXPANSION CONSTANT 
#define C_RED 10 // REDUCTION CONSTANT

/**
 * Creates a new dynamic array with a given length and a flag indicating
 * whether the array should be treated as ordered or not.
 *
 * @param length The length of the array
 * @param ordered A flag indicating whether the array should be treated as ordered or not
 * @return A new dynamic array with the given length and ordering
 */
TArray arrayCreate(int length, bool ordered) {
    TArray a;
    int size = length + C_EXP;
    a.items = (TInfoInt *) malloc(size * sizeof (TInfoInt));
    assert(size == 0 || a.items != NULL);
    a.length = length;
    a.size = size;
    a.ordered = ordered;
    return a;
}

/**
 * Destroys a dynamic array, releasing the memory associated with it.
 *
 * @param a A pointer to the dynamic array to be destroyed.
 */
void arrayDestroy(TArray *a) {
    free(a->items);
    a->items = NULL;
    a->length = a->size = 0;
}

/**
 * Resizes a dynamic array to a given length.
 *
 * @param a A pointer to the dynamic array to be resized
 * @param length The new length of the array
 */
void arrayResize(TArray *a, int length) {
    if (length > a->size || length < a->size - C_RED) {
        int size = length + C_EXP;
        a->items = (TInfoInt *) realloc(a->items, size * sizeof (TInfoInt));
        assert(size == 0 || a->items != NULL);
        a->size = size;
    }
    a->length = length;
}

/**
 * Prints the elements of a dynamic array.
 *
 * @param a A pointer to the dynamic array to be printed
 */
void arrayPrint(TArray *a){
	for(int i=0; i<a->length; i++){
		infoPrintInt(a->items[i]);
        printf(" ");
	}
}

/**
 * Adds an element to a dynamic array.
 * If the array is ordered, the element is inserted in the correct position.
 * If the element is already present in the array, it is not added again.
 *
 * @param a A pointer to the dynamic array.
 * @param info The element to be added to the array.
 */
void arrayAdd(TArray *a, TInfoInt info){

    if(arrayContains(a, info)) return;
    
    arrayResize(a, a->length + 1);
    if(!a->ordered) {
        a->items[a->length-1] = info;
        return;
    }
    int p, flag = 0;
    for(int i = 0; i < a->length; i++) {
        if(infoLessInt(info, a->items[i]) && flag == 0){
            p = i;
            flag = 1;
        }
    }

    if(flag == 0)
        p = a->length-1;
    else {
        for(int i = a->length-1; i > p; i--)
            a->items[i] = a->items[i - 1];
    }    
	a->items[p] = info;
}

/**
 * Determines whether a dynamic array contains a given element.
 *
 * @param a A pointer to the dynamic array
 * @param info The element to search for in the array
 * @return true if the element is found in the array, false otherwise
 */
bool arrayContains(TArray *a, TInfoInt info){

    int first = 0, last = a->length - 1, chosen;
    
    while(first <= last){
        chosen = (first+last)/2;
        if(a->items[chosen] == info)
            return true;
        if(a->items[chosen] < info)
            first = chosen + 1;
        else
            last = chosen - 1;
    }
    
    return false;
}