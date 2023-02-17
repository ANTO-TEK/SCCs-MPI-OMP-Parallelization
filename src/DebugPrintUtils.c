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
 * @file DebugPrintUtils.c
 * 
 * @brief This is a C file containing several functions that are used for debugging purposes. 
 * These functions are used to print the content of different data structures used in the parallel 
 * implementation of the Tarjan's algorithm for finding strongly connected components in a graph.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "../include/DebugPrintUtils.h"

/**
 * @brief Prints the strongly connected components (SCCs) of a graph to the console.
 *
 * @param sccCount An integer representing the total number of SCCs in the graph.
 * @param sccs An array of TArray structures, where each TArray represents an SCC and contains the 
 * indices of the vertices that belong to that SCC.
 * @param numOriginalVertices An integer representing the total number of vertices in the original 
 * graph.
 * @param sccsHT A hash table (THTSCCs) used to store the SCCs in a more efficient manner.
 */
void printSCCs(int sccCount, TArray* sccs, int numOriginalVertices, THTSCCs* sccsHT){

    printf("\nSCCs totali: %d\n", sccCount);

    for(int i = 0; i < sccCount; i++) {

        printf("SCC %d: ", i + 1);

        for(int j = 0; j < sccs[i].length; j++) {
            printNode(sccsHT, sccs[i].items[j], numOriginalVertices, NULL);
        }
            
        printf("\n");
    }
    
}


/**
 * @brief Writes the strongly connected components (SCCs) of a graph to a specified file.
 *
 * @param sccCount An integer representing the total number of SCCs in the graph.
 * @param sccs An array of TArray structures, where each TArray represents an SCC and contains the 
 * indices of the vertices that belong to that SCC.
 * @param numOriginalVertices An integer representing the total number of vertices in the original graph.
 * @param sccsHT A hash table (THTSCCs) used to store the SCCs in a more efficient manner.
 * @param fileName A string representing the name of the file to which the SCCs should be written.
 */
void printSCCsOnFile(int sccCount, TArray* sccs, int numOriginalVertices, THTSCCs* sccsHT, char* fileName){

    FILE *fd;

    fd = fopen(fileName, "w");
    if(fd == NULL) {
        perror("Error in opening file");
        exit(1);
    }

    if(fprintf(fd, "%d\n", sccCount) == 1);

    for(int i = 0; i < sccCount; i++) {

        for(int j = 0; j < sccs[i].length; j++) {
            printNode(sccsHT, sccs[i].items[j], numOriginalVertices, fd);
        }
            
        if(fprintf(fd, "-1\n") == 1); 
    }

    fclose(fd);
    
}

/**
 * @brief A helper function used by both printSCCs and printSCCsOnFile. It's also worth noting 
 * that the #ifdef TEST and #else blocks are preprocessor directives, which means that the code 
 * inside the block will only be included in the final executable if the symbol TEST has been defined.
 *
 * @param sccsHT A hash table (THTSCCs) used to store the SCCs in a more efficient manner.
 * @param macronode An integer representing the index of a vertex or a macro node in the graph.
 * @param numOriginalVertices An integer representing the total number of vertices in the original 
 * graph.
 * @param fd A pointer to a FILE struct, used to print the node to a file. (This parameter is optional, 
 * if it is not passed the function will print to the console)
 */
void printNode(THTSCCs* sccsHT, int node, int numOriginalVertices, FILE* fd) {
    if (node < numOriginalVertices) {
        #ifdef TEST
            if (fprintf(fd, "%d ", node) == 1);
        #else
            printf("%d ", node);
        #endif
        return;
    }

    TValueHTSCCs *value = HTSCCsSearch(sccsHT, node);
    for(int i = 0; i < value->length; i++) {
        printNode(sccsHT, value->items[i], numOriginalVertices, fd);
    }
}