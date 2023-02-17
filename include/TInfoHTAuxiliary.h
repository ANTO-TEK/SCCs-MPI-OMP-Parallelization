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
 * @file TInfoHTAuxiliary.h
 * 
 * @brief Header file for the auxiliary information of a vertex in the graph in order to perform the 
 * Tarjan's algorithm
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef TINFOHTAUXILIARY_H
#define TINFOHTAUXILIARY_H

#include <stdbool.h>

/**
 * @brief Type representing the key of a vertex in the graph in order to perform the Tarjan's algorithm
 * This type represents the key of a vertex in the graph in order to perform the Tarjan's algorithm, which 
 * is the vertex's id represented by an integer.
 */
typedef int TKeyHTAuxiliary;

/**
 * @brief Structure representing the auxiliary information of a vertex in the graph in order to perform the 
 * Tarjan's algorithm
 * This structure represents the auxiliary information of a vertex in the graph in order to perform the 
 * Tarjan's algorithm. It is composed by the following fields:
 * 	- disc: the discovery time of the vertex
 *  - low: the lowest discovery time of the vertex
 *  - stackMember: a boolean value that indicates if the vertex is in the stack or not
 */
typedef struct SValueHTAuxiliary {
	int disc;
	int low;
	bool stackMember;
} TValueHTAuxiliary;

/**
 * @brief Structure representing the information of a vertex in the graph in order to perform the Tarjan's 
 * algorithm
 * This structure represents the information of a vertex in the graph in order to perform the Tarjan's 
 * algorithm. It is composed by the following fields:
 *  - key: the key of the vertex in the hash table used by Tarjan's algorithm
 *  - value: the auxiliary information of the vertex
 */
typedef struct SInfoHTAuxiliary {
	TKeyHTAuxiliary key;
	TValueHTAuxiliary value;
} TInfoHTAuxiliary;

/**
 * @brief Prints the auxiliary information of a vertex in the graph
 * This function prints the auxiliary information of a vertex in the graph to the standard output.
 */
void infoPrintHTAuxiliary(TInfoHTAuxiliary);

/**
 * @brief Compares the auxiliary information of two vertices in the graph
 * This function compares the auxiliary information of two vertices in the graph. If the information 
 * is equal, the function returns true. Otherwise, it returns false.
 */
bool infoEqualHTAuxiliary(TInfoHTAuxiliary, TInfoHTAuxiliary);

/**
 * @brief Compares the auxiliary information of two vertices in the graph
 * This function compares the auxiliary information of two vertices in the graph. If the first information 
 * is less than the second, the function returns true. Otherwise, it returns false.
 */
bool infoLessHTAuxiliary(TInfoHTAuxiliary, TInfoHTAuxiliary);

/**
 * @brief Compares the auxiliary information of two vertices in the graph
 * This function compares the auxiliary information of two vertices in the graph. If the first information 
 * is greater than the second, the function returns true. Otherwise, it returns false.
 */
bool infoGreaterHTAuxiliary(TInfoHTAuxiliary, TInfoHTAuxiliary);

/**
 * @brief Calculates the hash value of a key for the hash table used by Tarjan's algorithm
 * This function calculates the hash value of a key for the hash table used by Tarjan's algorithm. The key 
 * is the vertex's id, represented by an integer.
 */
unsigned hashHTAuxiliary(TKeyHTAuxiliary);

#endif 