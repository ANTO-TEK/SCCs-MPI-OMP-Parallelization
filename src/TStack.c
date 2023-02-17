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
 * @file TStack.c
 * 
 * @brief The source file provides several functions for interacting with the stack data 
 * structure. The stackCreate() function creates and returns a new stack. The stackDestroy() 
 * function destroys a stack by deallocating the memory used by its underlying array and 
 * resetting its element count to 0. The stackPush() function pushes an element onto the top 
 * of the stack, and the stackPop() function removes and returns the element at the top of the 
 * stack. The stackTop() function returns the element at the top of the stack without removing 
 * it, and the stackIsEmpty() function returns a boolean indicating whether the stack is empty.
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdbool.h>
#include <stdio.h>

#include "../include/TStack.h"

/**
 * Creates a new stack.
 * 
 * @return The newly created stack
 */
TStack stackCreate(void) {
	TStack s;
	s.a = arrayCreate(0, false);
	return s;
}

/**
 * Destroys the specified stack.
 * 
 * @param stack A pointer to the stack to be destroyed
 */
void stackDestroy(TStack* stack) {
	arrayDestroy(&stack->a);
}

/**
 * Pushes an element onto the top of the stack.
 * 
 * @param stack A pointer to the stack
 * @param x The element to be pushed onto the stack
 */
void stackPush(TStack* stack, TInfoInt x) {
	//printf("\nentro\n");
	arrayResize(&stack->a, stack->a.length+1);
	//printf("\nEsco\n");
	stack->a.items[stack->a.length-1] = x;
}

/**
 * Removes and returns the element at the top of the stack.
 * 
 * @param stack A pointer to the stack
 * @return The element at the top of the stack
 */
TInfoInt stackPop(TStack* stack) {
	TInfoInt x = stack->a.items[stack->a.length-1];
    arrayResize(&stack->a, stack->a.length-1);
    return x;
}

/**
 * Returns the element at the top of the stack without removing it.
 * 
 * @param stack A pointer to the stack
 * @return The element at the top of the stack
 */
TInfoInt stackTop(TStack* stack) {
	return stack->a.items[stack->a.length-1];
}

/**
 * Determines whether the stack is empty.
 * 
 * @param stack A pointer to the stack
 * @return True if the stack is empty, false otherwise
 */
bool stackIsEmpty(TStack* stack) {
	return stack->a.length == 0;
}