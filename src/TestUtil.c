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
 * @file TestUtil.c
 * 
 * @brief This file contains utility functions for testing the implementation of the strongly 
 * connected components (SCC) algorithm.
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../include/TestUtil.h"

/**
 * This function frees the memory allocated for the TArray objects stored in the sequentialResult 
 * and parallelResult fields of the Results object, as well as the memory allocated for the Results 
 * object itself. It is called to clean up memory when the Results object is no longer needed.
 * 
 * @param res a pointer to a Results object to be destroyed
*/
void destroyResults(Results* res) {

    for(int i = 0; i < res->sequentialSCCsNum; i++) {
        arrayDestroy(&res->sequentialResult[i]);
        arrayDestroy(&res->parallelResult[i]);
    }
    free(res->sequentialResult);
    free(res->parallelResult);
    free(res);
}

/**
 * This function is used as a comparator by the qsort function to sort the SCCs by the value of 
 * their first element. It compares the first element of the TArray objects pointed to by a and b, 
 * and returns 1 if the element in a is greater, -1 if the element in b is greater, or 0 if they 
 * are equal.
 * 
 * @param a a pointer to a TArray object representing an SCC
 * @param b a pointer to a TArray object representing an SCC
 * 
 * @return an integer indicating the ordering of the two SCCs
*/
int comparator(const void* a, const void* b) {
    const TArray* da = a;
    const TArray* db = b;
    return (da->items[0] > db->items[0]) ? 1 : ((da->items[0] < db->items[0]) ? -1 : 0);  
}

/**
 * This function reads the results of the SCC algorithm stored in a file and stores them in an array 
 * of TArray objects. The file is opened in read-only mode, and the number of SCCs is read from the 
 * first line. The vertices of each SCC are then read from the following lines, until a -1 is encountered, 
 * which indicates the end of the SCC. The arrayCreate function is used to create an empty TArray object 
 * for each SCC, and the arrayAdd function is used to add the vertices of each SCC to its corresponding 
 * TArray object. The array of TArray objects is then sorted using the qsort function and the comparator 
 * function as a comparator. Finally, the array of TArray objects is returned.
 * 
 * @param fileName a string containing the name of the file to be read
 * @param SCCs a pointer to an integer that will be used to store the number of SCCs read from the file
 * 
 * @return an array of TArray objects, where each TArray object contains the vertices of a single SCC
 * 
*/
TArray* readResult(char* fileName, int *SCCs) {

    FILE* fd = fopen(fileName, "rt");
    if(fd == NULL) {
        perror("Error in opening file");
        exit(1);
    }

    int numScc,
        val,
        i;

    if(fscanf(fd, "%d\n", &numScc) == 1);
    
    *SCCs = numScc;

    TArray* sccs = (TArray*)malloc(numScc * sizeof(TArray));
    
    for(i = 0; i < numScc; i++){
        sccs[i] = arrayCreate(0, true);
        if(fscanf(fd, "%d ", &val) == 1);
        while(val != -1) {
            arrayAdd(&sccs[i], val);
            if(fscanf(fd, "%d ", &val) == 1);
        }
    }

    fclose(fd);
    
    qsort(sccs, numScc, sizeof(TArray), comparator);

    return sccs;

}

/**
 * This function compares the results of the sequential and parallel implementations 
 * of the SCC algorithm, and checks that they are equal. If they are not equal, the 
 * assert function is called to terminate the program.
 * 
 * @param res a pointer to a Results object that stores the results of the sequential 
 *            and parallel implementations of the SCC algorithm
*/
void compareResults(Results* res) {
    
    assert(res->sequentialSCCsNum == res->parallelSCCsNum);

    for(int i = 0; i < res->sequentialSCCsNum; i++){
        assert(res->sequentialResult[i].length == res->parallelResult[i].length);
        for(int j = 0; j < res->sequentialResult[i].length; j++)
            assert(res->sequentialResult[i].items[j] == res->parallelResult[i].items[j]);
    }
}