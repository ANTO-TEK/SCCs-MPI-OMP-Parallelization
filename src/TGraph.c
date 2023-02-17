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
 * @file TGraph.c
 * 
 * @brief This file contains the implementation of the graph data structure.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>

#include "../include/TGraph.h"

/**   
 * This function destroy a graph node, that is composed by an array of adjacent nodes and an array 
 * of components.
 * 
 * @param node A pointer to the Node to be destroyed.
 */
void graphNodeDestroy(Node *node){

    arrayDestroy(&(node->adj));
}

/**
 * Creates a new graph with the specified number of vertices.
 * 
 * @param vertices The number of vertices in the new graph.
 * @return A pointer to the new TGraph.
 */
TGraph graphCreate(int vertices) {
    TGraph graph = (TGraph) malloc(sizeof(TGraph));
    graph->vertices = vertices;
    graph->nodes = (Node *) malloc(vertices * sizeof(Node));
    for (int i = 0; i < vertices; i++) {
        graph->nodes[i].adj = arrayCreate(0, false);
    }
    return graph;
}

/**
 * This function destroy a TGraph passed as parameter doing the free for all the nodes and the graph struct.
 * 
 * @param graph A pointer to the TGraph to be destroyed.
 */
void graphDestroy(TGraph graph) {
    for (int i = 0; i < graph->vertices; i++) 
        graphNodeDestroy(&(graph->nodes[i]));
    free(graph->nodes);
    free(graph);
}

/**
 * This function is used to compare two nodes in order to sort them in ascending order.
 * 
 * @param a A pointer to the first value to be compared.
 * @param b A pointer to the second value to be compared.
 * @return An integer indicating the relative order of the values pointed to by a and b.
 */
int compare(const void *a, const void *b) {
    

    const Node *x = a;   
    const Node *y = b;
    
    int comp =  x->vertex - y->vertex;

    if (comp < 0)
        return -1;

    if (comp > 0)
        return 1;

    return comp;
}

/**
 * This function reads a file with the input graph and creates an instance of the TGraph 
 * data structure with the specified number of vertices. 
 * The file must have the following format: 
 *
 *  <number of nodes>
 *  <node> -1 <node> <node> ... <node> -1
 *  <node> -1 <node> <node> ... <node> -1
 *  ...
 *  <node> -1 <node> -1
 *
 * Each line corresponds to a node in the graph, with the first number indicating the node 
 * identifier and the subsequent numbers indicating the node's neighbors. The list of neighbors 
 * starts and ends with the value -1.
 *
 * The function reads the first line of the file to get the number of nodes in the graph and then 
 * creates an instance of TGraph with that number of vertices. It then reads the remaining lines 
 * of the file, one by one, to populate the graph's adjacency lists with the neighbors of each node. 
 * Finally, it sorts the nodes by their identifier and returns the created graph.
 *
 * @param filename A pointer to a character array containing the name of the file to be read.
 * @return A pointer to the populated TGraph.
 *
*/
TGraph popolateGraph(char *fileName) {
    
    FILE *fd;
    int nNodes;

    fd = fopen(fileName, "rt");
    if(fd == NULL) {
        perror("Errore in apertura del file");
        exit(1);
    }

    if(fscanf(fd, "%d\n", &nNodes) == 1);

    TGraph graph = graphCreate(nNodes);

    int src, dst;
    int i = 0;

    for(i = 0; i < nNodes; i++) {
        if(fscanf(fd, "%d -1 ", &src) == 1);
        if(fscanf(fd, "%d ", &dst) == 1);
        graph->nodes[i].vertex = src;
        while(dst != -1) {
            graphAddEdge(&graph, i, dst);
            if(fscanf(fd, "%d ", &dst) == 1);
        }
    }

    fclose(fd);
    qsort(graph->nodes, graph->vertices, sizeof(Node), compare);

    return graph;
}

/** 
 * This function add an edge between vertex v1 and v2 in the graph
 *
 * @param graph A pointer to the TGraph.
 * @param u The index of the first vertex.
 * @param v The index of the second vertex.
 */
void graphAddEdge(TGraph* graph, int v1, int v2) {
    arrayAdd(&(*graph)->nodes[v1].adj, v2);
}

/** 
 * This function print the input graph, giving in output the vertex and the adjacent nodes in the form:
 * 
 * <vertex> -> <adjacent node> <adjacent node> ... <adjacent node>
    
 * @param graph A pointer to the TGraph to be printed.
 */
void graphPrint(TGraph graph) {
    
    for (int i = 0; i < graph->vertices; i++) {
        printf("%d -> ", graph->nodes[i].vertex);
        arrayPrint(&(graph->nodes[i].adj));
        printf("\n");
    }
}

/**
 * This function takes a TGraph and a vertex identifier as input and returns the adjacency list of 
 * the vertex with the specified identifier. The function performs a binary search on the graph's 
 * nodes to find the node with the desired identifier and then returns a pointer to its adjacency 
 * list. If the node with the specified identifier is not found, the function returns NULL.
 * 
 * @param graph A pointer to the TGraph.
 * @param vertex The index of the vertex.
 * @return A pointer to a TArray containing the indices of the neighbors of the given vertex.
*/
TArray* getNeighbor(TGraph* graph, int vertex) {
    
    int first = 0, last = (*graph)->vertices - 1, chosen;
    
    while(first <= last){
        chosen = (first+last)/2;
        if((*graph)->nodes[chosen].vertex == vertex)
            return &(*graph)->nodes[chosen].adj;
        if((*graph)->nodes[chosen].vertex < vertex)
            first = chosen + 1;
        else
            last = chosen - 1;
    }
    
    return NULL;
}
