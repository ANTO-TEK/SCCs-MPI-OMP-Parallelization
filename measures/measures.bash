#!/bin/bash
# 
# Course: High Performance Computing 2022/2023
# 
# Lecturer: Francesco Moscato	fmoscato@unisa.it
#
# Group:
# Amato Emilio        0622701903      e.amato16@studenti.unisa.it             
# Bove Antonio        0622701898      a.bove57@studenti.unisa.it 
# De Gregorio Marco   0622701865      m.degregorio19@studenti.unisa.it
# 
# Source Code for sequential version:
# https://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/
#
# Copyright (C) 2023 - All Rights Reserved
#
# This file is part of FinalProjectHPC.
#
# This program is free software: you can redistribute it and/or modify it under the terms of 
# the GNU General Public License as published by the Free Software Foundation, either version 
# 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with ContestOMP. 
# If not, see <http://www.gnu.org/licenses/>.
#

# This script is used for benchmarking the performance of two algorithms, Tarjan and Kosaraju, 
# for finding strongly connected components in a graph. The script generates graphs with different 
# numbers of vertices and edges, and then runs the algorithms on those graphs using different numbers 
# of MPI processes and OpenMP threads. The script also has some options for optimization. 
# The script continues by iterating through different optimization options, graph dimensions, MPI 
# processes, and OpenMP threads, running the algorithms and recording the results in the CSV files. 
# The script also uses the mkdir command to create directories to store the results if they do not 
# already exist. It's important to note that the script defines the number of measures per test with 
# the variable NUM_MEASURES and the number of MPI processes and OpenMP threads with the variables 
# VECT_MPI_PROCESSES and VECT_OPENMP_THREADS respectively. These are parameters that can be adjusted 
# to fit the needs of the specific benchmarking run. This script also use the trap command to exit 
# the script if it receives an interrupt signal (INT). This is a useful feature for ensuring that the 
# script can be stopped if it is running for too long or if the user wants to stop it for any other 
# reason. The script also uses the printf command to print the progress of the script to the terminal 
# window. 

VERTEX_1=500
LB_1=(0 125 250)
HB_1=(125 250 375)

VERTEX_2=1000
LB_2=(0 0 250 500 999)
HB_2=(0 250 500 750 999)

VERTEX_3=2500
LB_3=(0 625 1350)
HB_3=(625 1350 1875)

VERTEX_4=10000
LB_4=(0 0 2500 5000 9999)
HB_4=(0 2500 5000 7500 9999)

# ARRAY THAT CONTAINS THE NUMBER OF MPI PROCESSES FOR TEST
VECT_MPI_PROCESSES=(0 1 2 4 8) 

# ARRAY THAT CONTAINS THE NUMBER OF OPENMP THREADS FOR TEST
VECT_OPENMP_THREADS=(1 2 4 8 16) 

OPTIMIZATION_OPTS=4

trap "exit" INT

SCRIPTPATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P)

function execMeasure {

    OPTIMIZATION_OPTS=$2
    local -n MPI_PROCESSES=$3
    local -n OPENMP_THREADS=$4
    ALGORITHM=$5
    VERTEX=$6
    local -n LB=$7
    local -n HB=$8
    SCRIPTPATH=$9

    NUM_MEASURES=2

    for ((opt = 0; opt < $OPTIMIZATION_OPTS; opt++)); do
        
        for j in "${!LB[@]}"; do

            lowerBound=${LB[${j}]}
            upperBound=${HB[${j}]}

            fileName="../data/graph_${VERTEX}_${lowerBound}_${upperBound}.txt"
            $1/generateGraph ${VERTEX} ${lowerBound} ${upperBound} $fileName

            for index in "${!MPI_PROCESSES[@]}"; do 

                if [[ ${MPI_PROCESSES[index]} -eq 0 ]]; then

                        proc_str=$(printf "%02d" ${MPI_PROCESSES[index]})

                        OUT_FILE=$SCRIPTPATH/$ALGORITHM/measure_O$opt/DIM_${VERTEX}_LB_${lowerBound}_HB_${upperBound}/DIM_${VERTEX}_LB_${lowerBound}_HB_${upperBound}_MPI_${proc_str}_OMP_00.csv

                        mkdir -p $(dirname $OUT_FILE) 2> /dev/null

                        if [ $ALGORITHM == "tarjan" ]; then
                            echo "processes,tarjan_time,comm_time,elapsed" >$OUT_FILE
                        elif [ $ALGORITHM == "kosaraju" ]; then
                            echo "processes,kosaraju_time,comm_time,elapsed" >$OUT_FILE
                        fi
                        
                        echo DIM_${VERTEX}_LB_${lowerBound}_HB_${upperBound}_MPI_${MPI_PROCESSES[index]}_OMP_00

                        for ((k = 0 ; k < $NUM_MEASURES ; k++)); do
                                
                                if [ $ALGORITHM == "tarjan" ]; then
                                    $1/tarjanSequentialO$opt $fileName >> $OUT_FILE
                                elif [ $ALGORITHM == "kosaraju" ]; then
                                    $1/kosarajuSequentialO$opt $fileName >> $OUT_FILE
                                fi

                                printf "\r> %d/%d %3.1d%% " $(expr $k + 1) $NUM_MEASURES $(expr \( \( $k + 1 \) \* 100 \) / $NUM_MEASURES)
                                printf "#%.0s" $(seq -s " " 1 $(expr \( $k \* 40 \) / $NUM_MEASURES))

                        done 
                    printf "\n"

                else 

                    for indexOmp in "${!OPENMP_THREADS[@]}"; do

                        proc_str=$(printf "%02d" ${MPI_PROCESSES[index]})
                        ths_str=$(printf "%02d" ${OPENMP_THREADS[indexOmp]})

                        OUT_FILE=$SCRIPTPATH/$ALGORITHM/measure_O$opt/DIM_${VERTEX}_LB_${lowerBound}_HB_${upperBound}/DIM_${VERTEX}_LB_${lowerBound}_HB_${upperBound}_MPI_${proc_str}_OMP_${ths_str}.csv

                        mkdir -p $(dirname $OUT_FILE) 2> /dev/null

                        if [ $ALGORITHM == "tarjan" ]; then
                            echo "processes,tarjan_time,comm_time,elapsed" >$OUT_FILE
                        elif [ $ALGORITHM == "kosaraju" ]; then
                            echo "processes,kosaraju_time,comm_time,elapsed" >$OUT_FILE
                        fi

                        echo DIM_${VERTEX}_LB_${lowerBound}_HB_${upperBound}_MPI_${MPI_PROCESSES[index]}_OMP_${OPENMP_THREADS[indexOmp]}

                        for ((k = 0 ; k < $NUM_MEASURES ; k++)); do

                            if [ $ALGORITHM == "tarjan" ]; then
                                mpirun.mpich -np ${MPI_PROCESSES[index]} $1/tarjanParallelO$opt ${OPENMP_THREADS[indexOmp]} $fileName  >> $OUT_FILE
                            elif [ $ALGORITHM == "kosaraju" ]; then
                                mpirun.mpich -np ${MPI_PROCESSES[index]} $1/kosarajuParallelO$opt ${OPENMP_THREADS[indexOmp]} $fileName  >> $OUT_FILE
                            fi
                            printf "\r> %d/%d %3.1d%% " $(expr $k + 1) $NUM_MEASURES $(expr \( \( $k + 1 \) \* 100 \) / $NUM_MEASURES)
                            printf "#%.0s" $(seq -s " " 1 $(expr \( $k \* 40 \) / $NUM_MEASURES))

                        done
                    
                    printf "\n"
                    done
                    
                fi

            done

        done

    done


}

ALGORITHM="tarjan"
execMeasure "$1" "$OPTIMIZATION_OPTS" VECT_MPI_PROCESSES VECT_OPENMP_THREADS "$ALGORITHM" "$VERTEX_1" LB_1 HB_1 "$SCRIPTPATH" 
execMeasure "$1" "$OPTIMIZATION_OPTS" VECT_MPI_PROCESSES VECT_OPENMP_THREADS "$ALGORITHM" "$VERTEX_2" LB_2 HB_2 "$SCRIPTPATH" 
execMeasure "$1" "$OPTIMIZATION_OPTS" VECT_MPI_PROCESSES VECT_OPENMP_THREADS "$ALGORITHM" "$VERTEX_3" LB_3 HB_3 "$SCRIPTPATH" 
execMeasure "$1" "$OPTIMIZATION_OPTS" VECT_MPI_PROCESSES VECT_OPENMP_THREADS "$ALGORITHM" "$VERTEX_4" LB_4 HB_4 "$SCRIPTPATH" 

ALGORITHM="kosaraju"
execMeasure "$1" "$OPTIMIZATION_OPTS" VECT_MPI_PROCESSES VECT_OPENMP_THREADS "$ALGORITHM" "$VERTEX_1" LB_1 HB_1 "$SCRIPTPATH" 
execMeasure "$1" "$OPTIMIZATION_OPTS" VECT_MPI_PROCESSES VECT_OPENMP_THREADS "$ALGORITHM" "$VERTEX_2" LB_2 HB_2 "$SCRIPTPATH" 
execMeasure "$1" "$OPTIMIZATION_OPTS" VECT_MPI_PROCESSES VECT_OPENMP_THREADS "$ALGORITHM" "$VERTEX_3" LB_3 HB_3 "$SCRIPTPATH" 
execMeasure "$1" "$OPTIMIZATION_OPTS" VECT_MPI_PROCESSES VECT_OPENMP_THREADS "$ALGORITHM" "$VERTEX_4" LB_4 HB_4 "$SCRIPTPATH" 
