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
 * @file Kosaraju.h
 * 
 * @brief This is a header file for the implementation of the Kosaraju's algorithm for finding strongly 
 * connected components (SCCs) in a directed graph. 
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef KOSARAJU_H
#define KOSARAJU_H

#include <stdbool.h>

#include "TGraph.h"
#include "TArray.h"
#include "THTAuxiliarySCC.h"
#include "Utils.h"

#define NOT_INITIALIZED -1
#define DELTA 10

/**
 * @brief This function creates the transpose of a given graph within a specified range of vertices.
 */
TGraph transposeGraph(TGraph*, int, int);

/**
 * @brief This function performs a depth-first search (DFS) on a given node in a graph, within a specified 
 * range of vertices, and updates a stack with the visited nodes.
 */
void dfs1(int, TGraph*, THTAuxiliarySCC*, TStack*, int, int);

/**
 * @brief function "dfs2" performs a depth-first search (DFS) on a given graph and its transpose, using an 
 * auxiliary hash table and an array of strongly connected components (SCCs). The DFS starts at a given node 
 * and continues until it reaches a specified stop point. The function also updates a count of the number of 
 * SCCs and an array of the SCCs themselves.
 */
void dfs2(int, TGraph*, THTAuxiliarySCC*, int, int, int *, TArray *);

/**
 * @brief This function performs the Kosaraju's algorithm for finding strongly connected components (SCCs) in a 
 * directed graph. The function returns an array of SCCs.
 */
TArray* kosaraju(TGraph , int , int , int *);

#endif