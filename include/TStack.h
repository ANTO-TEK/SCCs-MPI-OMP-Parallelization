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
 * @file TStack.h
 * 
 * @brief Header file for a stack data structure. This file contains the declarations of the functions 
 * and data structures required to implement a stack data structure. The stack is implemented using an 
 * array and can hold elements of type TInfoInt.

 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef TSTACK_H
#define TSTACK_H

#include <stdbool.h>

#include "TInfoInt.h"
#include "TArray.h"


/**
 * @brief Type definition for a stack
 */
typedef struct {
    TArray a;
} TStack;

/**
 * @brief Creates a new empty stack
 * This function creates a new empty stack and returns it.
 */
TStack stackCreate(void);

/**
 * @brief Destroys a stack
 * This function frees all memory associated with the given stack.
 */
void stackDestroy(TStack*);

/**
 * @brief Pushes an element onto the stack
 * This function adds the given element to the top of the stack.
 */
void stackPush(TStack*, TInfoInt);

/**
 * @brief Pops an element from the stack
 * This function removes the top element from the stack and returns it. If the
 * stack is empty, the behavior is undefined.
 */
TInfoInt stackPop(TStack*);

/**
 * @brief Returns the top element of the stack
 * This function returns the element at the top of the stack without removing it.
 * If the stack is empty, the behavior is undefined.
 */
TInfoInt stackTop(TStack*);

/**
 * @brief Check if the stack is empty
 * This function returns true if the stack is empty, and false otherwise.
 */
bool stackIsEmpty(TStack*);

#endif 
