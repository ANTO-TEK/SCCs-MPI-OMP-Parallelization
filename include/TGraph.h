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
 * @file TGraph.h
 * 
 * @brief Declarations for functions and data structures related to graph manipulation. This file 
 * contains declarations for the TGraph and Node data structures, as well as functions for creating 
 * and destroying graphs, adding edges, and printing graphs. It also includes a function for getting 
 * the neighbors of a given vertex in a graph.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "TArray.h"
#include "TStack.h"

/**
 * @brief A structure representing a node in a graph.
 * This structure contains a vertex identifier, a TArray of adjacent vertices, a TArray of connected 
 * components, and a flag indicating whether or not the node is a "macronode".
 */
typedef struct {
    int vertex;
    TArray adj;
} Node;

/**
 *  @brief A structure representing a graph.
 *  This structure contains the number of vertices in the graph and a pointer to an array of Node
 *  structures representing the vertices in the graph.
 */
typedef struct SGraphNode {
    int vertices;
    Node *nodes;
} TGraphNode;

/**
 *  @brief A typedef for a pointer to a TGraphNode.
 *  This typedef is used to create a new type, TGraph, which is a pointer to a TGraphNode
 *  structure. This allows for easier declaration and manipulation of graph variables.
 */
typedef TGraphNode *TGraph;

/**
 *  @brief Frees all memory associated with a Node structure.
 *  This function takes a pointer to a Node structure as input and frees all memory associated with
 *  the Node, including any memory allocated for its TArray fields.
 * 
 *  @param node A pointer to the Node to be destroyed.
 */
void graphNodeDestroy(Node *);

/**
 * @brief Creates a new graph with a specified number of vertices.
 * This function takes an integer as input and creates a new graph with that many vertices. It
 * returns a pointer to the new TGraph.
 * 
 * @param vertices The number of vertices in the new graph.
 * @return A pointer to the new TGraph.
 */
TGraph graphCreate(int);

/**
 * @brief A comparator function for qsort().
 * This function takes two pointers to void as input and compares the values pointed to by the 
 * pointers. It is intended to be used as a comparator function for the qsort() function, and 
 * returns a value less than, equal to, or greater than 0 depending on whether the first value is 
 * less than, equal to, or greater than the second value, respectively.
 * 
 * @param a A pointer to the first value to be compared.
 * @param b A pointer to the second value to be compared.
 * @return An integer indicating the relative order of the values pointed to by a and b.
 */
int compare(const void *, const void *);

/**
 * @brief Populates a graph with data from a given file.
 * This function takes a pointer to a character array (i.e., a string) containing the name of a
 * file and reads the data in the file to populate a graph with vertices and edges. It returns a
 * pointer to the populated TGraph.
 * 
 * @param filename A pointer to a character array containing the name of the file to be read.
 * @return A pointer to the populated TGraph.
 */
TGraph popolateGraph(char *);

/**
 * @brief Frees all memory associated with a TGraph.
 * This function takes a pointer to a TGraph as input and frees all memory associated with the
 * TGraph, including any memory allocated for its Node structures.
 * 
 * @param graph A pointer to the TGraph to be destroyed.
 */
void graphDestroy(TGraph);

/**
 * @brief Adds an edge between two vertices in a TGraph.
 * This function takes a pointer to a TGraph, and two integers representing the indices of the
 * vertices to be connected by an edge. It adds an edge between the vertices in the TGraph.
 * 
 * @param graph A pointer to the TGraph.
 * @param u The index of the first vertex.
 * @param v The index of the second vertex.
 */
void graphAddEdge(TGraph*, int, int);

/**
 * @brief Prints a TGraph to the console.
 * This function takes a pointer to a TGraph as input and prints a representation of the graph to
 * the console.
 * 
 * @param graph A pointer to the TGraph to be printed.
 */
void graphPrint(TGraph);

/**
 * @brief Gets the neighbors of a given vertex in a TGraph.
 * This function takes a pointer to a TGraph and an integer representing the index of a vertex,
 * and returns a pointer to a TArray containing the indices of the neighbors of the given vertex.
 * 
 * @param graph A pointer to the TGraph.
 * @param vertex The index of the vertex.
 * @return A pointer to a TArray containing the indices of the neighbors of the given vertex.
 */
TArray* getNeighbor(TGraph*, int);

#endif 