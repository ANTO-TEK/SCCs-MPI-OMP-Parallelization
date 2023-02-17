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
 * @file Tarjan.c
 *
 * @brief The source file provides the implementation of the Tarjan's algorithm
 *        to find Strongly Connected Components in a Graph.
 *
 * @version 0.1
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "../include/Tarjan.h"

/**
 * An helper function for finding the strongly connected components (SCCs) in a graph.
 *
 * The function starts by incrementing the time variable and setting the discovery time and low value
 * of the current vertex u in the hash table. The vertex u is then added to the stack and the flag
 * indicating that it is a member of the stack is set to true.
 * Next, the function iterates over the neighbors of u, represented as an array adj, and for each neighbor
 * v that is within the range specified by start and stop, it does the following:
 *
 *  - If the neighbor v has not been visited yet, the function recursively calls itself on v, and updates
 *    the low value of u to be the minimum between its current low value and the low value of v.
 *
 *  - If the neighbor v has already been visited and is a member of the stack, the function updates the low
 *    value of u to be the minimum between its current low value and the discovery time of v.
 *
 * After the loop, the function checks if the low value of u is equal to its discovery time, indicating that
 * u is the root of a SCC. If this is the case, the function creates a new dynamic array in the array containing
 * the scc found and adds all stack vertices that belong to this scc to it. The function * also updates the
 * stackMember flag of each vertex in the scc to false and removes them from the stack. Finally, the function
 * increments the sccCount variable in order to update the total number of sccs.
 *
 * @param graph Pointer to the graph structure.
 * @param u The current node being visited.
 * @param auxiliaryHT Pointer to a hash table that stores auxiliary information for each node in the graph.
 * @param st Pointer to a stack used to track nodes during the SCC search.
 * @param time Pointer to a variable that tracks the current time.
 * @param start The index of the first node in the subgraph being searched.
 * @param stop The index of the last node in the subgraph being searched.
 * @param sccCount Pointer to a variable that tracks the number of SCCs found.
 * @param sccs Pointer to an array of nodes representing the SCCs found.
 */
void sccUtil(TGraph graph, int u, THTAuxiliarySCC *auxiliaryHT, TStack *st, int *time, int start, int stop, int *sccCount, TArray *sccs)
{

    TValueHTAuxiliary *val = HTAuxiliarySCCSearch(auxiliaryHT, u);
    val->disc = val->low = ++(*time);
    stackPush(st, u);
    val->stackMember = true;

    TArray *adj = getNeighbor(&graph, u);

    for (int i = 0; i < adj->length; i++)
    {
        if (isInCut(&graph, adj->items[i], start, stop))
        {
            TValueHTAuxiliary *neighbor = HTAuxiliarySCCSearch(auxiliaryHT, adj->items[i]);
            if (neighbor->disc == -1)
            {
                sccUtil(graph, adj->items[i], auxiliaryHT, st, time, start, stop, sccCount, sccs);
                val->low = min(val->low, neighbor->low);
            }
            else if (neighbor->stackMember == true)
                val->low = min(val->low, neighbor->disc);
        }
    }

    int w = 0;

    if (val->low == val->disc)
    {
        sccs[*sccCount] = arrayCreate(0, false);
        while (stackTop(st) != u)
        {
            w = stackTop(st);
            arrayAdd(&sccs[*sccCount], w);
            TValueHTAuxiliary *value = HTAuxiliarySCCSearch(auxiliaryHT, w);
            value->stackMember = false;
            stackPop(st);
        }
        w = stackTop(st);

        arrayAdd(&sccs[*sccCount], w);
        TValueHTAuxiliary *value = HTAuxiliarySCCSearch(auxiliaryHT, w);
        value->stackMember = false;
        stackPop(st);
        (*sccCount)++;
    }
}

/**
 * This function finds the strongly connected components (SCCs) in a given range of vertices of a
 * graph.
 *
 * The function starts by initializing the variables time and auxiliaryHT, which is a hash table that
 * stores auxiliary information about each vertex. It also creates an empty stack st and an
 * array sccs to store the scc found. The sccCount variable is initialized to 0.
 * Next, the function iterates over all the vertices in the graph and inserts them in the hash table
 * with default values for their discovery time, low value, and stack membership flag.
 * Then, the function iterates over the vertices in the specified range [start, stop), and for each
 * vertex u that has not been visited yet, it calls the SSCUtil function to find the SCCs rooted at u.
 * After the loop, the function frees the resources used by the hash table and the stack, and returns the
 * array of SCC vertices.
 *
 * @param graph a graph data structure represented as an adjacency list.
 * @param start the index of the first vertex in the graph that should be considered in the traversal.
 * @param stop the index of the last vertex in the graph that should be considered in the traversal.
 * @param sccCount a pointer to a variable that stores the number of SCCs found.
 * @return an array of nodes representing the SCCs found in the graph.
 */
TArray *scc(TGraph graph, int start, int stop, int *sccCount)
{

    int time = 0;
    int dim = stop - start;

    THTAuxiliarySCC *auxiliaryHT = HTAuxiliarySCCCreate(graph->vertices);
    TStack st = stackCreate();
    TArray *sccs = (TArray *)malloc(graph->vertices * 10 * sizeof(TArray));

    *sccCount = 0;

    for (int i = 0; i < graph->vertices; i++)
        HTAuxiliarySCCInsert(auxiliaryHT, graph->nodes[i].vertex, (TValueHTAuxiliary){NOT_INITIALIZED, NOT_INITIALIZED, false});

    for (int i = start; i < stop; i++)
    {

        TValueHTAuxiliary *value = HTAuxiliarySCCSearch(auxiliaryHT, graph->nodes[i].vertex);

        if (value->disc == -1)
        {

            sccUtil(graph, graph->nodes[i].vertex, auxiliaryHT, &st, &time, start, stop, sccCount, sccs);
        }
    }

    HTAuxiliarySCCDestroy(auxiliaryHT);
    stackDestroy(&st);

    return sccs;
}