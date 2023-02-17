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
 * @file Utils.c
 * 
 * @brief This source file contains utility functions for other functions and libraries.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <omp.h>
#include <stdlib.h>

#include "../include/Utils.h"

/**
 * This function calculates an id for a given vertex, rank, number of items, and offset.
 * The function first calculates the id by adding the vertex to the value of rank multiplied 
 * by half of the number of items, and then increments the value of offset. This ensures that 
 * the id is unique for each vertex and each rank.
 *
 * @param rank The rank of the item
 * @param numItem The number of items
 * @param vertex The vertex of the item
 * @param offset Pointer to the current offset value
 *
 * @return The calculated ID
 */
int calculateId(int rank, int numItem, int vertex, int* offset) {
    return vertex + (rank * numItem/2) + ++(*offset);
}

/**
 * This function checks if a given vertex is in a cut of a given graph. 
 * It uses binary search to check if the vertex is in the cut of the graph.
 * It starts by initializing a variable first with the value of the start index of the cut and a 
 * variable last with the value of the stop index of the cut minus 1. It then enters a while loop 
 * that continues until the value of first is less than or equal to the value of last.
 * In the while loop, it calculates the middle index of the cut by taking the average of the start 
 * and stop index. It then checks if the vertex at this middle index of the graph is equal to the 
 * vertex we are searching for. If it is, it returns true, indicating that the vertex is in the cut.
 * If the vertex at the middle index of the graph is less than the vertex we are searching for, it 
 * means that the vertex we are searching for is in the right half of the cut, so it updates the value 
 * of the first variable to be the middle index plus 1.
 * If the vertex at the middle index of the graph is greater than the vertex we are searching for, it 
 * means that the vertex we are searching for is in the left half of the cut, so it updates the value 
 * of the last variable to be the middle index minus 1.
 * If the while loop ends and the vertex is not found in the cut, the function returns false, indicating 
 * that the vertex is not in the cut.
 * 
 * @param graph Pointer to the graph data structure.
 * @param vertex The vertex to check for.
 * @param start The starting index of the cut. 
 * @param stop The ending index of the cut.
 * @return True if the vertex is within the cut, false otherwise.
 */

bool isInCut(TGraph* graph, int vertex, int start, int stop) {

    int first = start, last = stop - 1, chosen;
    
    while(first <= last){
        chosen = (first+last)/2;
        if((*graph)->nodes[chosen].vertex == vertex)
            return true;
        if((*graph)->nodes[chosen].vertex < vertex)
            first = chosen + 1;
        else
            last = chosen - 1;
    }
    
    return false;
} 

/**
 * This line of code is a ternary operator, which is a shorthand way of writing an if statement.
 * In this case, the condition is a < b, and the values are a and b. If the condition is true, the 
 * operator returns a. If the condition is false, the operator returns b.
 * 
 * @param a The first integer.
 * @param b The second integer.
 * @return The minimum of `a` and `b`.
 */ 
int min(int a, int b){

    return (a < b) ? a : b;

}

/**
 * 
 * The function uses OpenMP to parallelize the creation of the new graph. It iterates through all the 
 * nodes in the graph, and for each node it:
 * 
 *  1. Assigns the vertex to the corresponding node in the new graph.
 *  2. If the vertex is greater than or equal to the number of vertices in the original graph, it searches 
 *     the strongly connected components hash table to find the strongly connected component that the vertex 
 *     belongs to. Then it retrieves the neighbors of that component from the original graph, reverse graph or 
 *     the old graph depending on the iteration number.
 *  3. For each neighbor, it checks the auxiliary graph hash table to see if the neighbor is part of a strongly 
 *     connected component. If it is, it follows the chain of pointers in the hash table to find the representative 
 *     vertex of the component, and adds an edge between the current node and that representative vertex in the new 
 *     graph. If the neighbor is not part of a strongly connected component, it adds an edge between the current 
 *     node and that neighbor in the new graph.
 *  4. If the vertex is less than the number of vertices in the original graph, it retrieves the neighbors of the 
 *     vertex from the original graph, reverse graph or the old graph depending on the iteration number and for each 
 *     neighbor, it checks the auxiliary graph hash table to see if the neighbor is part of a strongly connected 
 *     component. If it is, it follows the chain of pointers in the hash table to find the representative vertex of 
 *     the component, and adds an edge between the current node and that representative vertex in the new graph. If 
 *     the neighbor is not part of a strongly connected component, it adds an edge between the current node and that 
 *     neighbor in the new graph.
 *
 * @param graph Pointer to the original graph.
 * @param oldGraph Pointer to the previous iteration graph.
 * @param newGraph Pointer to the new graph to be created.
 * @param rvdGraph Pointer to the received graph.
 * @param sccsHT Pointer to the hash table for strongly connected components.
 * @param auxiliaryGraphHT Pointer to the hash table for auxiliary graph.
 * @param nodes Array containing the ids of the new graph's vertices
 * @param sccCount Number of strongly connected components.
 * @param numIteration The number of iteration.
 */
void createNewGraph(TGraph* graph, TGraph* oldGraph, TGraph* newGraph, TGraph* rvdGraph, THTSCCs* sccsHT, THTAuxiliaryGraph* auxiliaryGraphHT, int* nodes, int sccCount, int numIteration){

    int i, j, k;

    #pragma omp parallel for shared(sccCount, newGraph, oldGraph, graph, sccsHT, auxiliaryGraphHT) private(i, j, k)
    for(i = 0; i < sccCount; i++) {
        int vertex = nodes[i];
        (*newGraph)->nodes[i].vertex = vertex;
        if(vertex >= (*graph)->vertices) {
            TValueHTSCCs* components = HTSCCsSearch(sccsHT, vertex);
            for(j = 0; j < components->length; j++) {
                TArray* adj = getAdjacencyList(numIteration, graph, oldGraph, rvdGraph, components->items[j]);
                for(k = 0; k < adj->length; k++) {
                    int toSearch = adj->items[k];
                    TValueHTAuxiliaryGraph* value = HTAuxiliaryGraphSearch(auxiliaryGraphHT, toSearch);
                    if(value == NULL) {
                        graphAddEdge(newGraph, i, toSearch);
                        continue;
                    }
                    while(value != NULL){
                        toSearch = *value;
                        value = HTAuxiliaryGraphSearch(auxiliaryGraphHT, toSearch);
                    } 
                    if(toSearch != vertex)
                        graphAddEdge(newGraph, i, toSearch);
                }
            }

        } else {
            TArray* adj = getAdjacencyList(numIteration, graph, oldGraph, rvdGraph, vertex);
            for(int j = 0; j < adj->length; j++) {
                int toSearch = adj->items[j];
                TValueHTAuxiliaryGraph* value = HTAuxiliaryGraphSearch(auxiliaryGraphHT, toSearch);
                if(value == NULL) {
                    graphAddEdge(newGraph, i, toSearch);
                    continue;
                }
                while(value != NULL){
                    toSearch = *value;
                    value = HTAuxiliaryGraphSearch(auxiliaryGraphHT, toSearch);
                } 
                if(toSearch != vertex)
                    graphAddEdge(newGraph, i, toSearch);
            }
        }
    }
}

/**
 * This function returns the adjacency list of a given vertex in a given graph.
 * 
 * @param numIteration The number of iteration.
 * @param graph Pointer to the original graph.
 * @param oldGraph Pointer to the previous iteration graph.
 * @param rvdGraph Pointer to the received graph.
 * @param vertex The vertex.  
 * @return The adjacency list of the vertex.
 */
TArray* getAdjacencyList(int numIteration, TGraph* graph, TGraph* oldGraph, TGraph* rvdGraph, int vertex){
    TArray* adj = NULL;
    if(numIteration > 1){
        adj = getNeighbor(rvdGraph, vertex);
        if(adj == NULL)
            adj = getNeighbor(oldGraph, vertex);
    }
    if(adj == NULL)
        adj = getNeighbor(graph, vertex);
    return adj;
}

/**
 * This function serializes an array of strongly connected components (sccs) into a one-dimensional 
 * array of integers (buf).
 * The function starts by calculating the total size of the output array by iterating through the strongly 
 * connected components and adding up the number of vertices in each component. It then allocates memory for 
 * the output array and sets the first element to be the number of strongly connected components. The function 
 * then iterates through the array of strongly connected components and for each component:
 *  
 *  - it writes the number of vertices in the component to the output array
 *  - it writes the vertices of the component to the output array
 * 
 * Finally, it returns the output array. The output array is composed of an integer indicating the number of 
 * strongly connected components followed by the number of vertices in each component and then the vertices.
 * 
 * @param sccs Array of strongly connected components
 * @param sccCount Number of strongly connected components
 * @param size Pointer to the size of the output array
 * @return buf Pointer to the output array
*/
int *serializeSCCs(TArray *sccs, int sccCount, int *size) {

    int dim = 1; 

    #pragma omp parallel for reduction(+:dim)
    for(int i = 0; i < sccCount; i++) 
        dim += 1 + sccs[i].length;   

    *size = dim;

    int *buf = (int *)malloc(dim * sizeof(int));

    buf[0] = sccCount;

    int i = 1,
        j = 0;

    while(i < dim) {
        buf[i++] = sccs[j].length;

        for(int k = 0; k < sccs[j].length; k++) {
            buf[i++] = sccs[j].items[k];
        }

        j++;
    }

    return buf;

}

/**
 * This function deserializes an array of strongly connected components (SCCs) that were previously 
 * serialized.
 * The function uses two variables, i and j, to keep track of the current position in the buffer and 
 * the current SCC being deserialized. The variable i starts at 1, since the first element of the 
 * buffer is the number of SCCs.
 * The function enters a while loop that runs until i is less than bufSize. Inside the loop, it first 
 * reads the length of the current SCC from the buffer and increments i. It then creates a new TArray 
 * with arrayCreate(0, false) and increments the sccCount by 1.
 * It then enters another for loop that runs until k is less than the length of the current SCC. Inside 
 * the for loop, it reads the vertex number from the buffer and adds it to the current SCC using arrayAdd 
 * function. It increments i and k at the end of each iteration.
 * 
 * @param sccs Array of strongly connected components
 * @param buf Buffer containing the serialized SCCs
 * @param bufSize Size of the buffer
 * @param sccCount Pointer to the number of strongly connected components
*/
void deserializeSCCs(TArray *sccs, int *buf, int bufSize, int* sccCount) {

    int i = 1,
        j = 0;

    while(i < bufSize) {

        int length = buf[i++];

        sccs[(*sccCount)++] = arrayCreate(0, false);

        for(int k = 0; k < length; k++) 
            arrayAdd(&sccs[(*sccCount) - 1], buf[i++]);
        
    }
}

/**
 * This function takes in a pointer to a THTSCCs struct, which is a hash table implementation used to 
 * store the strongly connected components (SCCs) of a graph, and a pointer to an integer which will 
 * be used to store the size of the serialized data. The function first initializes the variable dim 
 * to 1, which will be used to keep track of the size of the serialized data. It then checks if the 
 * hash table is empty, if so it sets the size variable to 1, and creates a buffer of size 1 and assigns 
 * 0 to the first and only element, then returns the buffer. If the hash table is not empty, the 
 * function iterates through the bucket array of the hash table, and for each element that is being used, 
 * adds 2 to the dim variable (1 for the key, 1 for the length of the value), and adds the length of the 
 * value of the current bucket to dim. The size variable is then set to the value of dim. The function 
 * then allocates memory for the buffer of size dim * sizeof(int). The first element of the buffer is 
 * set to the number of used elements in the hash table. The function then iterates through the bucket 
 * array again, for each used element it adds the key, the length of the value, and the value itself to 
 * the buffer, and increments the index variable i accordingly. Finally, the buffer is returned.
 * 
 * @param sccsHT Pointer to a THTSCCs struct
 * @param size Pointer to an integer which will be used to store the size of the serialized data
 * @return buf Pointer to the serialized data
*/
int *serializeSCCsHT(THTSCCs* sccsHT, int *size) {

    int dim = 1;
    int* buf;

    if(sccsHT->n_used == 0) {
        *size = dim;
        buf = (int*)malloc(sizeof(int));
        buf[0] = 0;
        return buf;
    }
    
    int exit = 0;

    for(int i = 0; i < sccsHT->n_bucket; i++) {
        if(sccsHT->used[i] == 1){
            dim += 2 + sccsHT->bucket[i].value.length;
            if(exit++ == sccsHT->n_used) break;
        }
    }

    *size = dim;

    buf = (int *)malloc(dim * sizeof(int));

    buf[0] = sccsHT->n_used;

    int i = 1;
    
    exit = 0;
    for(int j = 0; j < sccsHT->n_bucket; j++) {
        if(sccsHT->used[j] == 1) {
            buf[i++] = sccsHT->bucket[j].key;
            buf[i++] = sccsHT->bucket[j].value.length;
            for(int k = 0; k < sccsHT->bucket[j].value.length; k++) {
                buf[i++] = sccsHT->bucket[j].value.items[k];
            }
            if(exit++ == sccsHT->n_used) break;
        }
    }

    return buf;

}

/**
 * This function deserializes a hash table of strongly connected components (THTSCCs) from a serialized 
 * form represented by an integer array buf. The first element of the array buf is the number of elements 
 * in the hash table. The rest of the array is a serialized representation of the key-value pairs stored 
 * in the hash table, where each key-value pair is represented by a key (int), followed by the number of 
 * elements in the value (int) and then the elements of the value (int).
 * The function starts by initializing the variable n to the first element of the buf array, which is the 
 * number of elements in the hash table. The variable i is set to 1, to keep track of the current position 
 * in the buf array as the elements are being deserialized.
 * The function then uses a for loop to iterate through the elements of the hash table. The for loop uses 
 * the variable j as the index, which starts at 0 and ends at n-1. On each iteration, the function assigns 
 * the next value in the buf array to a variable key, which is the key of the current key-value pair. The 
 * next value in the buf array is assigned to a variable len, which is the number of elements in the value 
 * of the current key-value pair. A temporary array tmp is created with the arrayCreate function and its 
 * length is set to len.
 * Then, the function uses a nested for loop to iterate through the elements of the value. The nested for 
 * loop uses the variable k as the index, which starts at 0 and ends at len-1. On each iteration, the function 
 * adds the next element in the buf array to the temporary array tmp using the arrayAdd function.
 * After the nested loop ends, the function calls the HTSCCsInsert function to insert the key-value pair 
 * (key, tmp) into the hash table.
 * This process is repeated for all elements in the hash table, until the for loop ends. The deserialized hash 
 * table is now stored in the sccsHT variable passed to the function.
 * 
 * @param sccsHT Pointer to a THTSCCs struct
 * @param buf Pointer to an integer array which contains the serialized data
*/
void deserializeSCCsHT(THTSCCs* sccsHT, int *buf) {

    int n = buf[0];

    int i = 1;

    for(int j = 0; j < n; j++) {
        int key = buf[i++];
        int len = buf[i++];
        TArray tmp = arrayCreate(0, false);
        for(int k = 0; k < len; k++) {
            arrayAdd(&tmp, buf[i++]);
        }
        HTSCCsInsert(sccsHT, key, tmp);
    }

}

/**
 * This function serializes a THTAuxiliaryGraph data structure into an array of integers. The serialized 
 * data includes the number of elements stored in the THTAuxiliaryGraph, the key and value of each element 
 * stored in the THTAuxiliaryGraph. 
 * It starts by initializing a variable dim to 1, which will be used to keep track of the size of the serialized 
 * data. If the THTAuxiliaryGraph is empty, the function will return an array of size 1 with the value 0.
 * Then, it loops through each bucket of the THTAuxiliaryGraph, and if the bucket is in use, it increments the size 
 * of the serialized data by 2 (1 for the key and 1 for the value).
 * Next, it allocates memory for the serialized data array and assigns the value of dim to size. It then sets the 
 * first element of the array to the number of used buckets in the THTAuxiliaryGraph.
 * It then loops through each bucket of the THTAuxiliaryGraph again, and if the bucket is in use, it adds the key 
 * and value of the element stored in that bucket to the serialized data array.
 * Finally, it returns the serialized data array.
 * 
 * @param auxGraphHT Pointer to a THTAuxiliaryGraph struct
 * @param size Pointer to an integer variable which will store the size of the serialized data array
 * @return Pointer to an integer array which contains the serialized data
*/
int *serializeAuxiliaryGraphHT(THTAuxiliaryGraph* auxGraphHT, int* size) {

    int dim = 1;
    int* buf;

    if(auxGraphHT->n_used == 0) {
        *size = dim;
        buf = (int*)malloc(sizeof(int));
        buf[0] = 0;
        return buf;
    }
    
    int exit = 0;
    for(int i = 0; i < auxGraphHT->n_bucket; i++) {
        if(auxGraphHT->used[i] == 1){
            dim += 2;
            if(exit++ == auxGraphHT->n_used) break;
        }
    }

    *size = dim;

    buf = (int *)malloc(dim * sizeof(int));

    buf[0] = auxGraphHT->n_used;

    int i = 1;
    
    exit = 0;
    for(int j = 0; j < auxGraphHT->n_bucket; j++) {
        if(auxGraphHT->used[j] == 1) {
            buf[i++] = auxGraphHT->bucket[j].key;
            buf[i++] = auxGraphHT->bucket[j].value;
            if(exit++ == auxGraphHT->n_used) break;
        }
    }

    return buf;

}

/**
 * This function is used to deserialize an auxiliary graph hash table from a buffer of integers. 
 * The function starts by reading the first element of the buffer, which is the number of elements in 
 * the hash table. It then uses a for loop to iterate through the buffer, starting from the second 
 * element. For each iteration, the function reads two integers from the buffer, the first one being 
 * the key and the second one being the value. It then uses the HTAuxiliaryGraphInsert function to insert 
 * the key-value pair into the auxiliary graph hash table.
 * 
 * @param auxGraphHT Pointer to a THTAuxiliaryGraph struct
 * @param buf Pointer to an integer array which contains the serialized data
*/
void deserializeAuxiliaryGraphHT(THTAuxiliaryGraph* auxGraphHT, int* buf){

    int n = buf[0];

    int i = 1;

    for(int j = 0; j < n; j++) {
        int key = buf[i++];
        int value = buf[i++];
        HTAuxiliaryGraphInsert(auxGraphHT, key, value);
    }

}

/**
 * This function is used to serialize a graph data structure. The function takes in a pointer to a TGraph (which 
 * is a directed graph data structure) and a pointer to an integer (size).
 * First, the function initializes a variable "dim" to 1 and uses an OpenMP parallel for loop to iterate through each 
 * vertex in the graph and add 2 (1 for the vertex number, 1 for the number of adjacent vertices) and the number of 
 * adjacent vertices for that vertex to the "dim" variable. This is used to determine the size of the buffer that will 
 * be used to store the serialized data.
 * The function then sets the size variable passed in to the calculated "dim" value. It then creates an integer buffer 
 * of "dim" size and sets the first element of the buffer to be the number of vertices in the graph.
 * The function then uses a for loop to iterate through each vertex in the graph and adds the vertex number, number of 
 * adjacent vertices, and all adjacent vertices for that vertex to the buffer.
 * Finally, the function returns the buffer containing the serialized data of the graph.
 * 
 * @param graph Pointer to a TGraph struct
 * @param size Pointer to an integer variable which will store the size of the serialized data array
 * @return Pointer to an integer array which contains the serialized data
*/
int* serializeGraph(TGraph* graph, int* size) {
    
    int dim = 1;

    #pragma omp parallel for reduction(+:dim)
    for(int i = 0; i < (*graph)->vertices; i++) 
            dim += 2 + (*graph)->nodes[i].adj.length;
    
    *size = dim;

    int* buf = (int *)malloc(dim * sizeof(int));
    
    buf[0] = (*graph)->vertices;

    int i = 1;
    
    for(int j = 0; j < (*graph)->vertices; j++) {
        buf[i++] = (*graph)->nodes[j].vertex;
        int len = (*graph)->nodes[j].adj.length;
        buf[i++] = len;
        for(int k = 0; k < len; k++) {
            buf[i++] = (*graph)->nodes[j].adj.items[k];
        }
    }

    return buf;
    
}

/**
 * This function deserializes a graph from an array of integers (buf) and the size of that array (bufSize). 
 * It starts by extracting the number of vertices of the graph from the first element of the array (buf[0]). 
 * Then, it creates an empty graph with that number of vertices. Next, it uses two pointers, i and j, to iterate 
 * over the array and the nodes of the graph respectively. It starts by extracting the vertex number from the array 
 * (buf[i++]) and assigns it to the corresponding node in the graph (graph->nodes[j].vertex = vertex). Then it 
 * extracts the length of the adjacency list of that node from the array (buf[i++]) and iterates over the elements 
 * of the adjacency list, adding them to the corresponding node in the graph (arrayAdd(&graph->nodes[j].adj, buf[i++]). 
 * Finally, it returns the deserialized graph.
 * 
 * @param buf Pointer to an integer array which contains the serialized data
 * @param bufSize Size of the serialized data array
 * @return Pointer to a TGraph struct
*/
TGraph deserializeGraph(int* buf, int bufSize) {
    int n = buf[0];
    TGraph graph = graphCreate(n);
    int i = 1;
    int j = 0;

    while (i < bufSize) {
        int vertex = buf[i++];
        graph->nodes[j].vertex = vertex;
        int length = buf[i++];

        for (int k = 0; k < length; k++)
            arrayAdd(&graph->nodes[j].adj, buf[i++]);
        j++;
    }

    return graph;
}