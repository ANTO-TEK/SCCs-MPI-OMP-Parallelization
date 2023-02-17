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
 * @file Tarjan.h
 * 
 * @brief Header file for functions to find the strongly connected components (SCCs) in a graph. This 
 * file contains the declarations of the functions required to find the strongly connected components 
 * (SCCs) in a graph. The SCCs are found using the Tarjan's algorithm.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef TARJANPARALLEL_H
#define TARJANPARALLEL_H

#include "TGraph.h"
#include "THTAuxiliarySCC.h"
#include "THTSCCs.h"
#include "THTAuxiliaryGraph.h"
#include "Utils.h"

#define NOT_INITIALIZED -1

/**
 * @brief Finds the strongly connected components (SCCs) in a graph
 * This function finds the strongly connected components (SCCs) in a graph
 * using the Tarjan's algorithm. It returns an array of nodes, where each
 * node represents a SCC and its children represent the nodes in the SCC. 
 */
TArray* scc(TGraph, int, int, int*);

/**
 * @brief Recursive function to find the strongly connected components (SCCs) in a graph
 * This is a helper function for the SCC() function. It performs the
 * recursive DFS traversal of the graph to find the SCCs.
 */
void sccUtil(TGraph, int, THTAuxiliarySCC*, TStack*, int*, int, int, int*, TArray*);

#endif