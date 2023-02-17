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
 * @file main.c
 * 
 * @brief Main file of the project
 * 
 * @version 0.1
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <sys/resource.h>
#include <time.h>

#include "../include/TGraph.h"
#include "../include/Tarjan.h"
#include "../include/Kosaraju.h"
#include "../include/THTSCCs.h"
#include "../include/THTAuxiliaryGraph.h"
#include "../include/DebugPrintUtils.h"
#include "../include/Utils.h"

int main(int argc, char **argv) {

/**
 * Setting the Stack Memory Limit 
 * 
 * This code snippet sets the stack memory limit for the current process to an infinite limit. 
 * It does this by first creating a struct rlimit , which contains two fields - rlim_cur and 
 * rlim_max - that hold the soft and hard limits, respectively. The soft and hard limits are 
 * then set to RLIM_INFINITY using the rlim object. Finally, the setrlimit function is called 
 * with the RLIMIT_STACK constant and the rlim object, which sets the stack memory limit to an 
 * infinite limit. If an error occurs, the perror function is called with the string "setrlimit" 
 * and return 1 is called.
*/
    struct rlimit rlim;

    rlim.rlim_cur = RLIM_INFINITY;
    rlim.rlim_max = RLIM_INFINITY;

    if (setrlimit(RLIMIT_STACK, &rlim) < 0) {
        perror("setrlimit");
        return 1;
    }

/**
 * The program checks for the presence of the PARALLEL macro, which indicates that the program is 
 * being compiled for parallel execution. If PARALLEL is defined, the program checks that the correct 
 * number of command-line arguments were passed (2 arguments, the number of OpenMP threads and the name 
 * of the input file). If the arguments are not correct, the program prints an error message and exits.
 */
#ifdef PARALLEL

/**
 * This block of code checks if the program was compiled with OpenMP support and checks for the correct 
 * number of command line arguments. If the number of arguments is not equal to 3, it prints a usage 
 * message and exits with a failure code.
*/
    if(argc != 3){
        fprintf(stderr, "Usage:\n\t%s [ompNumThreads] [fileName]\n", argv[0]);
		exit(EXIT_FAILURE);
    }

/**
 * This line sets the number of threads that will be used in the calculations. The value is taken from 
 * the command line arguments.
*/
    omp_set_num_threads(atoi(argv[1]));

    int cutGraphSize, 
        remainder, 
        start, 
        stop,
        sccCount = 0,
        offset = 0,
        MPIProcesses;
    
    double itime = 0,
           sccs_time = 0,
           comm_time = 0;

    int rank, 
        size;

/**
 * The MPI library is then initialized, and the rank and size of the current MPI process are determined.
 */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == size - 1) 
        MPIProcesses = size;

/**
 * The program then creates a graph from the file name given as the second command line argument.
 */
    TGraph graph = popolateGraph(argv[2]);

/**
 * This line stores the number of vertices in the graph.
 */
    int vertices = graph->vertices;

/**
 * This line stores the number of vertices in the graph.
 */
    int ultimo = graph->nodes[graph->vertices - 1].vertex;

/**
 * This code works by first dividing the graph into size parts. Each rank is assigned a portion of 
 * the graph, which is then passed to the appropriate algorithm. The two algorithms used are Tarjan's 
 * Algorithm and Kosaraju's Algorithm. The code then determines the start and stop values for each rank. 
 * If the rank is less than the remainder, it is assigned an extra vertex, otherwise it is assigned the 
 * same number of vertices as every other rank.
 */
    cutGraphSize = graph->vertices / size;
    remainder = graph->vertices % size;

    if(rank < remainder) {
        start = rank * (cutGraphSize + 1); 
        stop = start + cutGraphSize + 1;
    } else {
        start = rank * cutGraphSize + remainder;  
        stop = start + cutGraphSize;
    }

/**
 * The appropriate algorithm is then called on the assigned portion of the graph, and the results are stored 
 * in an array called sccs. Finally, the time taken to find the SCCs is stored.
*/
    TArray* sccs;

    if(rank == size - 1) itime = MPI_Wtime();

    #ifdef TARJAN
        sccs = scc(graph, start, stop, &sccCount);
    #endif

    #ifdef KOSARAJU
        sccs = kosaraju(graph, start, stop, &sccCount);
    #endif

    if(rank == size - 1) sccs_time += (MPI_Wtime() - itime);

/**
 * The next part of the code contains several variables: 
 *  - sizeBuffer and rcvSizeBuffer: both of type int, used to store the size of the buffers. 
 *  - buffer and rcvBuffer: both of type int*, used to store buffers. 
 *  - color: of type int, used to store the color assigned to the process. 
 *  - COMM: of type MPI_Comm, used to store the MPI_Comm_WORLD communicator. 
 *  - NEW_COMM: of type MPI_Comm, used to store a new communicator. 
 *  - sccsHT: of type THTSCCs, used to store a hash table of strongly connected components. 
 *  - auxiliaryGraphHT: of type THTAuxiliaryGraph, used to store a hash table in order to compute 
 *    a new graph. 
 *  - oldGraph: of type TGraph, used to store the old graph. 
 *  - rvdGraph: of type TGraph, used to store the received graph.
*/
    int sizeBuffer,
        rcvSizeBuffer;
    int *buffer,
        *rcvBuffer;

    int color;

    MPI_Comm COMM = MPI_COMM_WORLD;
    MPI_Comm NEW_COMM;

    THTSCCs* sccsHT = HTSCCsCreate(graph->vertices / 2);
    THTAuxiliaryGraph* auxiliaryGraphHT = HTAuxiliaryGraphCreate(graph->vertices / 2);

    TGraph oldGraph = NULL, rvdGraph = NULL;

/**
 * At this point each process has its own SCC based on the portion of the graph that has been assigned to it. As a result, 
 * the inter-process communication process begins. In particular, while the number of processes is greater than 1, the 
 * process with even rank sends to the process with the following rank the closely connected components found, the hash 
 * table 'sccsHT' which has the name of a macronode as key and as value the strictly connected components that compose it, 
 * the hash table 'auxiliaryGraphHT' which has as key the name of a vertex and as value the name of the macronode to which 
 * it belongs and the graph of the previous iteration. In particular, during the first iteration, the two tables hash and 
 * oldGraph are empty. The process with odd rank, on the other hand, receives this information from the process with previous 
 * rank and adds it to its own. At this point, based on the information received, the process proceeds with the creation of a 
 * new graph, which is then passed to the 'scc' or 'kosaraju' method to find the strictly connected components. Finally, based 
 * on the value of the 'color' value (0 for processes with rank even and that sent the information and 1 for all others), a 
 * new communicator is created, and processes with color equal to 0 are eliminated. The old communicator is then updated, the 
 * number of iterations is updated and we proceed with the next iteration. 
 */
    int numIteration = 1; 
    while(size > 1) {

        if(rank % 2 == 0) {
            color = 1;
            if(rank + 1 < size) {
                
                if(rank == 0) itime = MPI_Wtime();
                buffer = serializeSCCs(sccs, sccCount, &sizeBuffer);
                MPI_Send(&sizeBuffer, 1, MPI_INT, rank + 1, 0, COMM);
                MPI_Send(buffer, sizeBuffer, MPI_INT, rank + 1, 0, COMM);
                free(buffer);

                buffer = serializeSCCsHT(sccsHT, &sizeBuffer);
                MPI_Send(&sizeBuffer, 1, MPI_INT, rank + 1, 0, COMM);
                if(sizeBuffer != 1)
                    MPI_Send(buffer, sizeBuffer, MPI_INT, rank + 1, 0, COMM);
                free(buffer);

                buffer = serializeAuxiliaryGraphHT(auxiliaryGraphHT, &sizeBuffer);
                MPI_Send(&sizeBuffer, 1, MPI_INT, rank + 1, 0, COMM);
                if(sizeBuffer != 1)
                    MPI_Send(buffer, sizeBuffer, MPI_INT, rank + 1, 0, COMM);
                free(buffer);

                if(numIteration > 1) {
                    buffer = serializeGraph(&oldGraph, &sizeBuffer);
                    MPI_Send(&sizeBuffer, 1, MPI_INT, rank + 1, 0, COMM);
                    MPI_Send(buffer, sizeBuffer, MPI_INT, rank + 1, 0, COMM);
                    free(buffer);
                }
                
                if(rank == 0) comm_time += (MPI_Wtime() - itime);
                color = 0;
            }
        } else {
            
            color = 1;

            MPI_Recv(&rcvSizeBuffer, 1, MPI_INT, rank - 1, 0, COMM, MPI_STATUS_IGNORE);
            rcvBuffer = malloc(rcvSizeBuffer * sizeof(int));
            MPI_Recv(rcvBuffer, rcvSizeBuffer, MPI_INT, rank - 1, 0, COMM, MPI_STATUS_IGNORE);

            if(rank == size - 1) itime = MPI_Wtime();
            deserializeSCCs(sccs, rcvBuffer, rcvSizeBuffer, &sccCount);
            if(rank == size - 1) comm_time += (MPI_Wtime() - itime);
            free(rcvBuffer);

            MPI_Recv(&rcvSizeBuffer, 1, MPI_INT, rank - 1, 0, COMM, MPI_STATUS_IGNORE);
            if(rcvSizeBuffer != 1) {
                rcvBuffer = malloc(rcvSizeBuffer * sizeof(int));
                MPI_Recv(rcvBuffer, rcvSizeBuffer, MPI_INT, rank - 1, 0, COMM, MPI_STATUS_IGNORE);
                if(rank == size - 1) itime = MPI_Wtime();
                deserializeSCCsHT(sccsHT, rcvBuffer);
                if(rank == size - 1) comm_time += (MPI_Wtime() - itime);
                free(rcvBuffer);
            }

            MPI_Recv(&rcvSizeBuffer, 1, MPI_INT, rank - 1, 0, COMM, MPI_STATUS_IGNORE);
            if(rcvSizeBuffer != 1) {
                rcvBuffer = malloc(rcvSizeBuffer * sizeof(int));
                MPI_Recv(rcvBuffer, rcvSizeBuffer, MPI_INT, rank - 1, 0, COMM, MPI_STATUS_IGNORE);
                if(rank == size - 1) itime = MPI_Wtime();
                deserializeAuxiliaryGraphHT(auxiliaryGraphHT, rcvBuffer);
                if(rank == size - 1) comm_time += (MPI_Wtime() - itime);
                free(rcvBuffer);
            }

            if(numIteration > 1) {
                MPI_Recv(&rcvSizeBuffer, 1, MPI_INT, rank - 1, 0, COMM, MPI_STATUS_IGNORE);
                rcvBuffer = malloc(rcvSizeBuffer * sizeof(int));
                MPI_Recv(rcvBuffer, rcvSizeBuffer, MPI_INT, rank - 1, 0, COMM, MPI_STATUS_IGNORE);
                if(rank == size - 1) itime = MPI_Wtime();
                rvdGraph = deserializeGraph(rcvBuffer, rcvSizeBuffer);
                if(rank == size - 1) comm_time += (MPI_Wtime() - itime);
                free(rcvBuffer);
            }

            if(rank == size - 1) itime = MPI_Wtime();
            int* nodes = (int*)malloc(sccCount * sizeof(int)); 
            int i, j;
            for(i = 0; i < sccCount; i++) {
                int len = sccs[i].length;
                if(len > 1) {
                    int id = calculateId((rank+1)*size, vertices, ultimo*numIteration, &offset);
                    nodes[i] = id;
                    TArray tmp = arrayCreate(0, false); 
                    for(j = 0; j < len; j++) {
                        int key = sccs[i].items[j];
                        HTAuxiliaryGraphInsert(auxiliaryGraphHT, key, id);
                        arrayAdd(&tmp, key);
                    }
                    HTSCCsInsert(sccsHT, id, tmp);
                } else {
                    nodes[i] = sccs[i].items[0];
                }
            }
            
            TGraph newGraph = graphCreate(sccCount);

            createNewGraph(&graph, &oldGraph, &newGraph, &rvdGraph, sccsHT, auxiliaryGraphHT, nodes, sccCount, numIteration);

            if(rank == size - 1) comm_time += (MPI_Wtime() - itime);
            
            if(numIteration > 1)
                graphDestroy(oldGraph);

            oldGraph = newGraph;

            free(nodes);

            qsort(oldGraph->nodes, oldGraph->vertices, sizeof(Node), compare);

            for(int i = 0; i < sccCount; i++) 
                arrayDestroy(&sccs[i]);
            free(sccs);

            if(rank == size - 1) itime = MPI_Wtime();

            #ifdef TARJAN
                sccs = scc(oldGraph, 0, oldGraph->vertices, &sccCount);
            #endif

            #ifdef KOSARAJU
                sccs = kosaraju(oldGraph, 0, oldGraph->vertices, &sccCount);
            #endif

            if(rank == size - 1) sccs_time += (MPI_Wtime() - itime);

        }

        if(size == 2 && rank == 0) {
            MPI_Send(&sccs_time, 1, MPI_DOUBLE, 1, 0, COMM);
            MPI_Send(&comm_time, 1, MPI_DOUBLE, 1, 0, COMM);
        } else if(size == 2 && rank == 1){
            double rcvTarjanTime, rcvCommTime;
            MPI_Recv(&rcvTarjanTime, 1, MPI_DOUBLE, 0, 0, COMM, MPI_STATUS_IGNORE);
            MPI_Recv(&rcvCommTime, 1, MPI_DOUBLE, 0, 0, COMM, MPI_STATUS_IGNORE);
            sccs_time += rcvTarjanTime;
            comm_time += rcvCommTime;
        }

        MPI_Barrier(COMM);
        MPI_Comm_split(COMM, color, rank, &NEW_COMM);
        MPI_Comm_rank(NEW_COMM, &rank);
        MPI_Comm_size(NEW_COMM, &size);
        
        if(color == 0) {
            HTSCCsDestroy(sccsHT);
            HTAuxiliaryGraphDestroy(auxiliaryGraphHT);
            graphDestroy(graph);
            if(oldGraph != NULL) graphDestroy(oldGraph);
            if(rvdGraph != NULL) graphDestroy(rvdGraph);
            for(int i = 0; i < sccCount; i++) 
                arrayDestroy(&sccs[i]);
            free(sccs);
            MPI_Finalize();
            return 0;
        }

        if(numIteration > 1)
            MPI_Comm_free(&COMM);
        
        numIteration++;
        
        COMM = NEW_COMM;

    }

/**
 * This code snippet is mainly responsible for cleaning up the memory used in the program. It handles 
 * deallocating the memory used by the program and shutting down the MPI processes.
 * The ifndef TEST statement is used to print out the number of MPI processes, the time taken to run 
 * the SCCs, the communication time and the total time taken.
 * The ifdef TEST statement is used to print the SCCs to a file for testing purposes.
*/
    #ifndef TEST
        printf("%d,%f,%f,%f\n", MPIProcesses, sccs_time, comm_time, sccs_time + comm_time);
    #endif

    #ifdef TEST
        printSCCsOnFile(sccCount, sccs, graph->vertices, sccsHT, "../test/SCCParOut.txt");
    #endif

    HTSCCsDestroy(sccsHT);
    HTAuxiliaryGraphDestroy(auxiliaryGraphHT);
    graphDestroy(graph);
    if(oldGraph != NULL) graphDestroy(oldGraph);
    if(rvdGraph != NULL) graphDestroy(rvdGraph);
    for(int i = 0; i < sccCount; i++) 
        arrayDestroy(&sccs[i]);
    free(sccs);

    MPI_Finalize();
    return 0;

#endif

/**
 * Sequential Algorithm for Finding Strongly Connected Components
 * 
 * This code implements an algorithm for finding the strongly connected components of a directed 
 * graph using either Tarjan's or Kosaraju's algorithms.
 * The program takes one argument: the filename of the graph to analyze. It begins by populating 
 * the graph based on the provided filename. It then proceeds to detect the strongly connected 
 * components in the graph using either Tarjan's or Kosaraju's algorithms, depending on which is 
 * specified in the #ifdef statement. The program will then record the elapsed time it took to 
 * compute the strongly connected components and output it to the terminal. If a 'TEST' flag is set, 
 * the program will output the strongly connected components to a file. Lastly, the program will free 
 * the memory allocated for the strongly connected components and destroy the graph.
*/
#ifdef SEQUENTIAL

    if(argc != 2){
        fprintf(stderr, "Usage:\n\t%s [fileName]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    TGraph graph = popolateGraph(argv[1]);

    int sccsCount;
    TArray* sccs;

    clock_t begin = clock();

    #ifdef TARJAN
        sccs = scc(graph, 0, graph->vertices, &sccsCount); 
    #endif

    #ifdef KOSARAJU
        sccs = kosaraju(graph, 0, graph->vertices, &sccsCount);
    #endif 

    clock_t end = clock();
    double elapsed = (double)(end - begin) / CLOCKS_PER_SEC;

    #ifndef TEST
        printf("0,%f,0,%f\n", elapsed, elapsed);          
    #endif
    
    #ifdef TEST
        printSCCsOnFile(sccsCount, sccs, graph->vertices, NULL, "../test/SCCSeqOut.txt");
    #endif

    for(int i = 0; i < sccsCount; i++) 
        arrayDestroy(&sccs[i]);
    free(sccs);
    graphDestroy(graph);

    return 0;

#endif

}