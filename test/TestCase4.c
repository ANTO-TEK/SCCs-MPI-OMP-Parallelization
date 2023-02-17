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
 * @file TestCase4.c
 * 
 * @brief This code is a test driver for comparing the results of a parallel and sequential implementation 
 * of the Tarjan and Kosaraju algorithms for finding strongly connected components (SCCs) in a graph.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */


/**
 * This code is a test case for comparing the results of a parallel implementation of Tarjan and Kosaraju 
 * algorithm to their sequential counterparts. The test is done by generating a graph of VERTICES_TEST 
 * vertices with edges between them randomly chosen between LB_TEST and HB_TEST. The first step is to create 
 * a variable named res of type Results* and allocate memory for it using malloc. The next step is to call 
 * generateGraph() function, which takes 4 arguments: the number of vertices, the lower bound of the edges, 
 * the upper bound of the edges, and the file name where the graph will be stored. This function generates 
 * the graph and stores it in a file. Then, the code runs two system commands, the first one is to run the 
 * sequential version of the Tarjan algorithm on the generated graph, the second is to run the parallel version 
 * of the Tarjan algorithm on the generated graph using 2 MPI processes and 2 OpenMP threads. After that, the 
 * code calls the readResult() function which takes two arguments, the first is the file name of the result and 
 * the second is a pointer to an integer that will store the number of strongly connected components. The function 
 * reads the result from the file, and the result and the number of strongly connected components are stored in 
 * the res variable. Then the code calls the compareResults() function which takes the res variable as an argument, 
 * this function compares the results of the sequential and parallel implementations of the algorithm. After that, 
 * the code calls the destroyResults() function which takes the res variable as an argument, this function frees 
 * the memory allocated for the res variable. The code then removes the output files of the sequential and parallel 
 * versions of the Tarjan algorithm using the remove function. Then the code repeats the same process but this time 
 * for the Kosaraju algorithm. Finally, the code removes the input graph file.
*/

#include "../include/TestUtil.h"

#define VERTICES_TEST 1000
#define LB_TEST 0
#define HB_TEST 100

int main(int argc, char **argv) {

    /*------------------------------------ CASE TEST 1 ------------------------------------*/

    Results* res = (Results*)malloc(sizeof(Results));
    
    generateGraph(VERTICES_TEST, LB_TEST, HB_TEST, FILE_IN_GRAPH);

    system("../build/tarjanSequentialTest ../test/graphTest.txt");
    system("mpirun -np 2 ../build/tarjanParallelTest 2 ../test/graphTest.txt");

    res->sequentialResult = readResult(FILE_OUT_SEQ, &res->sequentialSCCsNum);
    res->parallelResult = readResult(FILE_OUT_PAR, &res->parallelSCCsNum);
    
    compareResults(res);

    destroyResults(res);

    remove(FILE_OUT_SEQ);
    remove(FILE_OUT_PAR);

    res = (Results*)malloc(sizeof(Results));

    system("../build/kosarajuSequentialTest ../test/graphTest.txt");
    system("mpirun -np 2 ../build/kosarajuParallelTest 2 ../test/graphTest.txt");
    
    res->sequentialResult = readResult(FILE_OUT_SEQ, &res->sequentialSCCsNum);
    res->parallelResult = readResult(FILE_OUT_PAR, &res->parallelSCCsNum);
    
    compareResults(res);

    destroyResults(res);

    remove(FILE_IN_GRAPH);
    remove(FILE_OUT_SEQ);
    remove(FILE_OUT_PAR);

    /*-------------------------------------------------------------------------------------*/

}