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
 * @file GraphGenerator.c
 * 
 * @brief This is a C file containing a method which allows the generation of a graph. 
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/GraphGenerator.h"

/**
 * The generateGraph function generates a random directed graph and writes it to a file. The function 
 * first opens the file specified by filename in write mode. If it is unable to open the file, it prints
 * an error message and returns. It then writes the value of vertices to the file, followed by a newline 
 * character. The function then creates an array of integers called nodes, which is used to store the nodes
 * of the graph. It initializes this array with the values 0 to vertices - 1. The function then seeds the
 * random number generator with the current time. It then iterates through the vertices nodes of the graph.
 * For each node, it writes the node's index to the file, followed by the value -1. It then generates a 
 * random number of out-going edges for the node, between lowerBound and upperBound, inclusive. For each 
 * out-going edge, it selects a random node from the nodes array, making sure that the selected node is not
 * the same as the current node. It then writes the selected node's index to the file, followed by a space 
 * character. Finally, it writes the value -1 to the file, followed by a newline character. After all nodes
 * have been written to the file, the function frees the memory occupied by the nodes array and closes the 
 * file.
 * 
 * @param vertices an integer representing the number of vertices in the graph
 * @param lowerBound an integer representing the minimum number of out-going edges for each vertex
 * @param upperBound an integer representing the maximum number of out-going edges for each vertex
 * @param filename a string representing the name of the file where the graph will be written
 * 
 */
void generateGraph(int vertices, int lowerBound, int upperBound, const char* filename) {
  FILE* file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "Error opening file '%s'\n", filename);
    return;
  }

  fprintf(file, "%d\n", vertices);

  int* nodes = (int*) malloc(vertices * sizeof(int));
  for (int i = 0; i < vertices; i++) {
    nodes[i] = i;
  }

  srand(time(NULL));
  for (int i = 0; i < vertices; ++i) {
    fprintf(file, "%d -1", i);

    int adjSize = lowerBound + rand() % (upperBound - lowerBound + 1);
    int k = vertices - 1;
    for (int j = 0; j < adjSize; ++j) {
      int p = rand() % (k + 1);
      if (nodes[p] != i) {
        fprintf(file, " %d", nodes[p]);
      }

      int temp = nodes[k];
      nodes[k] = nodes[p];
      nodes[p] = temp;
      --k;
    }
    fprintf(file, " -1\n");
  }

  free(nodes);
  fclose(file);
}

/** 
 * If the TEST macro is not defined, the function is run as a standalone program. It takes in four
 * command-line arguments: the number of vertices in the graph, the minimum number of out-going edges
 * for each vertex, the maximum number of out-going edges for each vertex, and the name of the file 
 * where the graph will be written. If the number of command-line arguments is not four, it prints a 
 * usage message and exits with an error code. Otherwise, it converts the command-line arguments to 
 * the appropriate data types and calls the generateGraph function with these arguments. It then exits 
 * with a success code.
 */
#ifndef TEST

  int main(int argc, char** argv) {

    if(argc != 5){
      fprintf(stderr,"Usage:\n\t%s [vertices] [lowerBound] [upperBound] [fileName]\n", argv[0]);
      exit(EXIT_FAILURE);
    }

    int vertices = atoi(argv[1]);
    int lowerBound = atoi(argv[2]);
    int upperBound = atoi(argv[3]);
    char* fileName = argv[4];

    generateGraph(vertices, lowerBound, upperBound, fileName);

    exit(EXIT_SUCCESS);

  }

#endif