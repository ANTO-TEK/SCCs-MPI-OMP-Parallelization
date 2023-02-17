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
 * @file TestUtil.h
 * 
 * @brief This file contains utility functions for testing the strongly connected components (SCC) algorithm.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef TESTUTIL_H
#define TESTUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/GraphGenerator.h"
#include "../include/TArray.h"

#define FILE_IN_GRAPH "../test/graphTest.txt"
#define FILE_OUT_SEQ "../test/SCCSeqOut.txt"
#define FILE_OUT_PAR "../test/SCCParOut.txt"

/**
 * @brief A struct for storing the results of the sequential and parallel implementations of 
 * the strongly connected components (SCC) algorithm.
 * The Results struct is used to store the results of the sequential and parallel implementations 
 * of the SCC algorithm, so that they can be compared and tested. The sequentialResult and 
 * parallelResult fields are arrays of TArray objects representing the SCCs found by the respective 
 * implementations, and the sequentialSCCsNum and parallelSCCsNum fields store the number of SCCs 
 * found by the respective implementations.
*/
typedef struct {

    TArray* sequentialResult,
            *parallelResult;
    int sequentialSCCsNum,
        parallelSCCsNum;

} Results;

/**
 * @brief Frees the memory allocated for a Results object.
*/
void destroyResults(Results*);

/**
 * @brief A comparator function for use with the qsort function.
*/
int comparator(const void*, const void*);

/**
 * @brief Reads the results of the SCC algorithm from a file and stores them in an array 
 * of TArray objects.
*/
TArray* readResult(char*, int*);

/**
 * @brief Compares the results of the sequential and parallel implementations of the SCC 
 * algorithm.
*/
void compareResults(Results*);

#endif