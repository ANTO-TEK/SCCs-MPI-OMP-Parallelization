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
 * @file Utils.h
 * 
 * @brief This file defines various utility functions used in the TGraph, THTSCCs, and THTAuxiliaryGraph 
 * data structures and in the main.

 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include "TGraph.h"
#include "THTSCCs.h"
#include "THTAuxiliaryGraph.h"

/**
 * @brief This function calculates an id for a given vertex, rank, number of items, and offset.
 */
int calculateId(int, int, int, int*);

/**
 * @brief This function checks if a given vertex is in a cut of a given graph. 
 */
bool isInCut(TGraph*, int, int, int);

/**
 * @brief Returns the minimum of two integers
 */
int min(int, int);

/**
 * @brief Creates a new graph
 */
void createNewGraph(TGraph *, TGraph*, TGraph*, TGraph*, THTSCCs*, THTAuxiliaryGraph*, int*, int, int);

/**
 * @brief This function returns the adjacency list of a given vertex in a given graph.
 */
TArray* getAdjacencyList(int, TGraph*, TGraph*, TGraph*, int);

/**
 * @brief This function serializes an array of strongly connected components (sccs) into a one-dimensional 
 * array of integers (buf).
 */
int* serializeSCCs(TArray*, int, int*);

/**
 * @brief This function deserializes an array of strongly connected components (SCCs) that were previously 
 * serialized.
 */
void deserializeSCCs(TArray*, int*, int, int*);

/**
 * @brief This function serializes an hash table of strongly connected components (sccs) into a one-dimensional 
 * array of integers.
 */
int* serializeSCCsHT(THTSCCs*, int*);

/**
 * @brief Deserialization function for a hash table of strongly connected components (THTSCCs)
 */
void deserializeSCCsHT(THTSCCs*, int*) ;

/**
 * @brief This function serializes a THTAuxiliaryGraph data structure into an array of integers
 */
int* serializeAuxiliaryGraphHT(THTAuxiliaryGraph*, int*);

/**
 * @brief This function is used to deserialize an auxiliary graph hash table from a buffer of integers. 
 */
void deserializeAuxiliaryGraphHT(THTAuxiliaryGraph*, int*);

/**
 * @brief This function serializes a graph into an array of integers
 */
int* serializeGraph(TGraph*, int*);

/**
 * @brief This function deserializes a graph from an array of integers
 */
TGraph deserializeGraph(int*, int);

#endif 