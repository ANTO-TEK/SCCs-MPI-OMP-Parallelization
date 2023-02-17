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
 * @file Kosaraju.c
 * 
 * @brief Source file for the Kosaraju's algorithm to find the strongly connected components 
 * (SCCs) of a directed graph. The algorithm takes in a TGraph (a graph data structure), start 
 * and stop indices, and a pointer to an integer for the number of SCCs found. The function uses 
 * the TGraph, TArray, and TStack data structures and also calls two additional functions "dfs1" 
 * and "dfs2" to perform depth-first search. The transposeGraph function is used to find the 
 * transpose of the original graph. The function also uses a stack to keep track of the order 
 * of vertices as they are finished in the first dfs, and prints out the SCCs as it finds them. 
 * Finally, it returns an array of TArrays containing all the SCCs and frees any dynamically 
 * allocated memory before returning.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#include "../include/Kosaraju.h"

/**
 * The purpose of the transposeGraph() is to create a new TGraph which is the transpose of the original.
 * The transposeGraph()  will go through the original graph, and get all the neighbor indices in the 
 * original graph, and add them as nodes in the transposed graph.
 * The transposeGraph() function begins by creating a new graph. It then adds the vertices from the original 
 * graph to the new graph. It then iterates through the original graph, and for each node, it gets the list 
 * of the neighbor indices. It then checks if the neighbor index is in the cut (the cut being between the 
 * start and the stop indices). If the neighbor index is in the cut, then it is added to the list of nodes 
 * in the transposed graph.
 * 
 * @param graph The original graph
 * @param start The start index of the cut
 * @param stop The stop index of the cut
 * @return TGraph The transposed graph
*/
TGraph transposeGraph(TGraph* graph, int start, int stop) {

    TGraph transpose = graphCreate((*graph)->vertices);

    for(int i = 0; i < (*graph)->vertices; i++) 
        transpose->nodes[i].vertex = (*graph)->nodes[i].vertex;
        
    for(int i = 0; i < (*graph)->vertices; i++) {
        TArray *adj = getNeighbor(graph, (*graph)->nodes[i].vertex);
        for(int j = 0; j < adj->length; j++) {
            int elem = adj->items[j];
            if(isInCut(graph, elem, start, stop)) {
                TArray *adj = getNeighbor(&transpose, elem);
                arrayAdd(adj, (*graph)->nodes[i].vertex);
            }
        }
    }
    
    return transpose;
}


/**
 * This function performs a depth-first search of a graph starting from a given node.
 * The function starts by searching for the given node in the auxiliary hash table, and setting the 
 * stackMember value to true if it is found. It then gets the neighbors of the node from the graph, 
 * and loops through them. If a neighbor is within the given cut range, it is then searched in the 
 * auxiliary hash table. If it is not found, the dfs1() function is called recursively on the neighbor.
 * Finally, the node is pushed to the stack.
 * 
 * @param node The starting node of the search
 * @param graph The graph to be searched
 * @param auxiliaryHT A hash table used for auxiliary data
 * @param stack A stack used for storing visited nodes
 * @param start The start of the cut range
 * @param stop The end of the cut range
*/
void dfs1(int node, TGraph* graph, THTAuxiliarySCC* auxiliaryHT, TStack* stack, int start, int stop) {

    TValueHTAuxiliary *val = HTAuxiliarySCCSearch(auxiliaryHT, node);
    val->stackMember = true;

    TArray *adj = getNeighbor(graph, node);
    for(int i = 0; i < adj->length; i++) {
        if(isInCut(graph, adj->items[i], start, stop)) {
            TValueHTAuxiliary *neighbor = HTAuxiliarySCCSearch(auxiliaryHT, adj->items[i]);
            if(!neighbor->stackMember) 
                dfs1(adj->items[i], graph, auxiliaryHT, stack, start, stop);
        }
    }
    stackPush(stack, node);
}

/**
 * This function performs a depth-first search (DFS) on a transposed graph, starting at a given node, 
 * to find strongly connected components (SCCs) within a specified range.
 * 
 * @param node The starting node for the DFS
 * @param transpose A pointer to the transposed graph on which to perform the DFS
 * @param auxiliaryHT A pointer to the auxiliary hash table used to store information about the nodes 
 * during the DFS
 * @param start The lower bound of the range in which to search for SCCs
 * @param stop The upper bound of the range in which to search for SCCs
 * @param sccCount A pointer to the variable that keeps track of the number of SCCs found so far
 * @param sccs A pointer to the array that stores the SCCs found
 */
void dfs2(int node, TGraph* transpose, THTAuxiliarySCC* auxiliaryHT, int start, int stop, int *sccCount, TArray *sccs) {

    TValueHTAuxiliary *val = HTAuxiliarySCCSearch(auxiliaryHT, node);
    val->stackMember = true;

    arrayAdd(&sccs[*sccCount], node);
    TArray *adj = getNeighbor(transpose, node);
    for(int i = 0; i < adj->length; i++) {
        if(isInCut(transpose, adj->items[i], start, stop)) {
            TValueHTAuxiliary *neighbor = HTAuxiliarySCCSearch(auxiliaryHT, adj->items[i]);
            if(!neighbor->stackMember) 
                dfs2(adj->items[i], transpose, auxiliaryHT, start, stop, sccCount, sccs);
        }
    }
}

/**
 * The code is an implementation of the Kosaraju's algorithm to find the strongly connected components 
 * (SCCs) in a given graph.
 * 
 *  1.The function starts by creating an auxiliary hash table, called "auxiliaryHT", to store information 
 *    about the nodes during the depth-first search (DFS) process.
 *  2.Then, it creates an array, called "sccs", to store the SCCs that are found.
 *  3.It creates a stack, called "stack", to keep track of the nodes during the DFS process.
 *  4.It creates the transpose of the given graph, called "transpose", by calling the "transposeGraph" 
 *    function.
 *  5.It sets the variable "sccCount" to zero, which will keep track of the number of SCCs found.
 *  6.It initializes the auxiliary hash table by inserting the vertices of the graph into it and setting their 
 *    "stackMember" property to "false".
 *  7.It performs the first DFS by iterating over all the vertices within the given range of "start" and "stop" 
 *    and calling the "dfs1" function on each vertex that is not already on the stack.
 *  8.It reinitializes the auxiliary hash table by inserting the vertices of the graph into it and setting their 
 *    "stackMember" property to "false" again.
 *  9.It performs the second DFS by popping elements from the stack, and for each element, calling the "dfs2" 
 *    function if the element is not already on the stack.
 *  10.It increments the "sccCount" variable for each SCC that is found.
 *  11.Finally, it frees the memory allocated for the auxiliary hash table, the stack and the transposed graph, 
 *     and returns the array of SCCs.
 * 
 * @param graph The graph on which to perform the Kosaraju algorithm
 * @param start The lower bound of the range in which to search for SCCs
 * @param stop The upper bound of the range in which to search for SCCs
 * @param sccCount A pointer to the variable that keeps track of the number of SCCs found so far
 * @return An array of SCCs
 */
TArray* kosaraju(TGraph graph, int start, int stop, int *sccCount) {

    THTAuxiliarySCC* auxiliaryHT = HTAuxiliarySCCCreate(graph->vertices);
    TArray *sccs = (TArray *) malloc(graph->vertices * DELTA * sizeof(TArray));
    TStack stack = stackCreate();
    TGraph transpose = transposeGraph(&graph, start, stop);
    
    *sccCount = 0;
    
    for(int i = 0; i < graph->vertices; i++) 
        HTAuxiliarySCCInsert(auxiliaryHT, graph->nodes[i].vertex, (TValueHTAuxiliary){NOT_INITIALIZED, NOT_INITIALIZED, false});

    for(int i = start; i < stop; i++) {
        TValueHTAuxiliary *value = HTAuxiliarySCCSearch(auxiliaryHT, graph->nodes[i].vertex);
        if(!value->stackMember) 
            dfs1(graph->nodes[i].vertex, &graph, auxiliaryHT, &stack, start, stop);
    }

    for(int i = 0; i < graph->vertices; i++) 
        HTAuxiliarySCCInsert(auxiliaryHT, graph->nodes[i].vertex, (TValueHTAuxiliary){NOT_INITIALIZED, NOT_INITIALIZED, false});

    while(!stackIsEmpty(&stack)){
        int node = stackPop(&stack);
        TValueHTAuxiliary *value = HTAuxiliarySCCSearch(auxiliaryHT, node);
        if(!value->stackMember) {
            sccs[(*sccCount)] = arrayCreate(0, false);
            dfs2(node, &transpose, auxiliaryHT, start, stop, sccCount, sccs);
            (*sccCount)++;
        }
    }

    HTAuxiliarySCCDestroy(auxiliaryHT);
    stackDestroy(&stack);
    graphDestroy(transpose);

    return sccs;
}

