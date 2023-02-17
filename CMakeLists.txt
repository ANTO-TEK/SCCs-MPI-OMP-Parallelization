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

cmake_minimum_required(VERSION 3.9)
project(ParallelSCCsAssignment
	DESCRIPTION "Provide a parallel version of the Tarjan's and Kosaraju's algorithm to find Strongly Connected Components in a Graph."
	LANGUAGES C
	)

# Generate comile_commands.json for semantic completion in YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# DEBUG flag
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

# Enable testing
enable_testing()

# Add executable
add_executable(tarjanSequentialO0 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(tarjanParallelO0 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(tarjanSequentialO1 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(tarjanParallelO1 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(tarjanSequentialO2 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(tarjanParallelO2 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(tarjanSequentialO3 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(tarjanParallelO3 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)

add_executable(kosarajuSequentialO0 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(kosarajuParallelO0 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(kosarajuSequentialO1 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(kosarajuParallelO1 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(kosarajuSequentialO2 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(kosarajuParallelO2 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(kosarajuSequentialO3 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(kosarajuParallelO3 src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(generateGraph src/GraphGenerator.c)

# Specify include directoritarjanSequentialecutable
target_include_directories(tarjanSequentialO0 PRIVATE include)
target_include_directories(tarjanParallelO0 PRIVATE include)
target_include_directories(tarjanSequentialO1 PRIVATE include)
target_include_directories(tarjanParallelO1 PRIVATE include)
target_include_directories(tarjanSequentialO2 PRIVATE include)
target_include_directories(tarjanParallelO2 PRIVATE include)
target_include_directories(tarjanSequentialO3 PRIVATE include)
target_include_directories(tarjanParallelO3 PRIVATE include)

target_include_directories(kosarajuSequentialO0 PRIVATE include)
target_include_directories(kosarajuParallelO0 PRIVATE include)
target_include_directories(kosarajuSequentialO1 PRIVATE include)
target_include_directories(kosarajuParallelO1 PRIVATE include)
target_include_directories(kosarajuSequentialO2 PRIVATE include)
target_include_directories(kosarajuParallelO2 PRIVATE include)
target_include_directories(kosarajuSequentialO3 PRIVATE include)
target_include_directories(kosarajuParallelO3 PRIVATE include)

# Link with required libraries
find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

if(OpenMP_C_FOUND AND MPI_C_FOUND)
	message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
	target_link_libraries(tarjanSequentialO0 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(tarjanParallelO0 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(tarjanSequentialO1 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(tarjanParallelO1 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(tarjanSequentialO2 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(tarjanParallelO2 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(tarjanSequentialO3 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(tarjanParallelO3 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)

	target_link_libraries(kosarajuSequentialO0 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(kosarajuParallelO0 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(kosarajuSequentialO1 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(kosarajuParallelO1 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(kosarajuSequentialO2 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(kosarajuParallelO2 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(kosarajuSequentialO3 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(kosarajuParallelO3 PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
endif()

# Specify compilation otarjanSequentialtarget_compile_options(tarjanParallelIVATE -O3)
target_compile_options(tarjanSequentialO1 PRIVATE -O1)
target_compile_options(tarjanParallelO1 PRIVATE -O1)
target_compile_options(tarjanSequentialO2 PRIVATE -O2)
target_compile_options(tarjanParallelO2 PRIVATE -O2)
target_compile_options(tarjanSequentialO3 PRIVATE -O3)
target_compile_options(tarjanParallelO3 PRIVATE -O3)

target_compile_options(kosarajuSequentialO1 PRIVATE -O1)
target_compile_options(kosarajuParallelO1 PRIVATE -O1)
target_compile_options(kosarajuSequentialO2 PRIVATE -O2)
target_compile_options(kosarajuParallelO2 PRIVATE -O2)
target_compile_options(kosarajuSequentialO3 PRIVATE -O3)
target_compile_options(kosarajuParallelO3 PRIVATE -O3)

target_compile_options(generateGraph PRIVATE -O3)

target_compile_features(tarjanSequentialO0 PRIVATE c_std_11)
target_compile_features(tarjanParallelO0 PRIVATE c_std_11)
target_compile_features(tarjanSequentialO1 PRIVATE c_std_11)
target_compile_features(tarjanParallelO1 PRIVATE c_std_11)
target_compile_features(tarjanSequentialO2 PRIVATE c_std_11)
target_compile_features(tarjanParallelO2 PRIVATE c_std_11)
target_compile_features(tarjanSequentialO3 PRIVATE c_std_11)
target_compile_features(tarjanParallelO3 PRIVATE c_std_11)

target_compile_features(kosarajuSequentialO0 PRIVATE c_std_11)
target_compile_features(kosarajuParallelO0 PRIVATE c_std_11)
target_compile_features(kosarajuSequentialO1 PRIVATE c_std_11)
target_compile_features(kosarajuParallelO1 PRIVATE c_std_11)
target_compile_features(kosarajuSequentialO2 PRIVATE c_std_11)
target_compile_features(kosarajuParallelO2 PRIVATE c_std_11)
target_compile_features(kosarajuSequentialO3 PRIVATE c_std_11)
target_compile_features(kosarajuParallelO3 PRIVATE c_std_11)

target_compile_definitions(tarjanParallelO0 PRIVATE -DPARALLEL -DTARJAN)
target_compile_definitions(tarjanSequentialO0 PRIVATE -DSEQUENTIAL -DTARJAN)
target_compile_definitions(tarjanParallelO1 PRIVATE -DPARALLEL -DTARJAN)
target_compile_definitions(tarjanSequentialO1 PRIVATE -DSEQUENTIAL -DTARJAN)
target_compile_definitions(tarjanParallelO2 PRIVATE -DPARALLEL -DTARJAN)
target_compile_definitions(tarjanSequentialO2 PRIVATE -DSEQUENTIAL -DTARJAN)
target_compile_definitions(tarjanParallelO3 PRIVATE -DPARALLEL -DTARJAN)
target_compile_definitions(tarjanSequentialO3 PRIVATE -DSEQUENTIAL -DTARJAN)

target_compile_definitions(kosarajuParallelO0 PRIVATE -DPARALLEL -DKOSARAJU)
target_compile_definitions(kosarajuSequentialO0 PRIVATE -DSEQUENTIAL -DKOSARAJU)
target_compile_definitions(kosarajuParallelO1 PRIVATE -DPARALLEL -DKOSARAJU)
target_compile_definitions(kosarajuSequentialO1 PRIVATE -DSEQUENTIAL -DKOSARAJU)
target_compile_definitions(kosarajuParallelO2 PRIVATE -DPARALLEL -DKOSARAJU)
target_compile_definitions(kosarajuSequentialO2 PRIVATE -DSEQUENTIAL -DKOSARAJU)
target_compile_definitions(kosarajuParallelO3 PRIVATE -DPARALLEL -DKOSARAJU)
target_compile_definitions(kosarajuSequentialO3 PRIVATE -DSEQUENTIAL -DKOSARAJU)

# ---------------------------------------- TEST -----------------------------------------
add_executable(tarjanSequentialTest src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(tarjanParallelTest src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)

add_executable(kosarajuSequentialTest src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)
add_executable(kosarajuParallelTest src/main.c src/Utils.c src/Tarjan.c src/Kosaraju.c src/TArray.c src/TGraph.c src/THTAuxiliaryGraph.c src/THTAuxiliarySCC.c src/THTSCCs.c src/TInfoHTAuxiliary.c src/TInfoHTAuxiliaryGraph.c src/TInfoHTSCCs.c src/TInfoInt.c src/TStack.c src/DebugPrintUtils.c)

target_include_directories(tarjanSequentialTest PRIVATE include)
target_include_directories(tarjanParallelTest PRIVATE include)

target_include_directories(kosarajuSequentialTest PRIVATE include)
target_include_directories(kosarajuParallelTest PRIVATE include)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

if(OpenMP_C_FOUND AND MPI_C_FOUND)
	message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
	target_link_libraries(tarjanSequentialTest PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(tarjanParallelTest PUBLIC MPI::MPI_C OpenMP::OpenMP_C)

	target_link_libraries(kosarajuSequentialTest PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
	target_link_libraries(kosarajuParallelTest PUBLIC MPI::MPI_C OpenMP::OpenMP_C)
endif()

target_compile_features(tarjanSequentialTest PRIVATE c_std_11)
target_compile_features(tarjanParallelTest PRIVATE c_std_11)

target_compile_features(kosarajuSequentialTest PRIVATE c_std_11)
target_compile_features(kosarajuParallelTest PRIVATE c_std_11)

target_compile_definitions(tarjanSequentialTest PRIVATE -DSEQUENTIAL -DTEST -DTARJAN)
target_compile_definitions(tarjanParallelTest PRIVATE -DPARALLEL -DTEST -DTARJAN)

target_compile_definitions(kosarajuSequentialTest PRIVATE -DSEQUENTIAL -DTEST -DKOSARAJU)
target_compile_definitions(kosarajuParallelTest PRIVATE -DPARALLEL -DTEST -DKOSARAJU)

add_executable(case_test_1 test/TestCase1.c src/GraphGenerator.c src/TArray.c src/TInfoInt.c src/TestUtil.c)
add_executable(case_test_2 test/TestCase2.c src/GraphGenerator.c src/TArray.c src/TInfoInt.c src/TestUtil.c)
add_executable(case_test_3 test/TestCase3.c src/GraphGenerator.c src/TArray.c src/TInfoInt.c src/TestUtil.c)
add_executable(case_test_4 test/TestCase4.c src/GraphGenerator.c src/TArray.c src/TInfoInt.c src/TestUtil.c)
add_executable(case_test_5 test/TestCase5.c src/GraphGenerator.c src/TArray.c src/TInfoInt.c src/TestUtil.c)
add_executable(case_test_6 test/TestCase6.c src/GraphGenerator.c src/TArray.c src/TInfoInt.c src/TestUtil.c)

target_include_directories(case_test_1 PRIVATE include)
target_include_directories(case_test_2 PRIVATE include)
target_include_directories(case_test_3 PRIVATE include)
target_include_directories(case_test_4 PRIVATE include)
target_include_directories(case_test_5 PRIVATE include)
target_include_directories(case_test_6 PRIVATE include)

target_compile_definitions(case_test_1 PRIVATE -DTEST)
target_compile_definitions(case_test_2 PRIVATE -DTEST)
target_compile_definitions(case_test_3 PRIVATE -DTEST)
target_compile_definitions(case_test_4 PRIVATE -DTEST)
target_compile_definitions(case_test_5 PRIVATE -DTEST)
target_compile_definitions(case_test_6 PRIVATE -DTEST)

add_test(NAME case_test_1 COMMAND case_test_1)
add_test(NAME case_test_2 COMMAND case_test_2)
add_test(NAME case_test_3 COMMAND case_test_3)
add_test(NAME case_test_4 COMMAND case_test_4)
add_test(NAME case_test_5 COMMAND case_test_5)
add_test(NAME case_test_6 COMMAND case_test_6)

# ---------------------------------- GENERATE MEASURES ----------------------------------
add_custom_target(
	generate_measures
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating measures [${CMAKE_CURRENT_SOURCE_DIR}/measures/measures.bash ${CMAKE_CURRENT_BINARY_DIR}]"
    DEPENDS	
		generateGraph
		tarjanSequentialO0
		tarjanParallelO0
		tarjanSequentialO1
		tarjanParallelO1
		tarjanSequentialO2
		tarjanParallelO2
		tarjanSequentialO3
		tarjanParallelO3
)

# --------------------------------- MEASURES EXTRACTION ---------------------------------
find_package(Python3 COMPONENTS Interpreter REQUIRED)

if(NOT PipEnv_FOUND)
    message("pipenv not found, installing...")
    execute_process(
        COMMAND pip install pipenv
        RESULT_VARIABLE result
    )
    if(result)
        message(FATAL_ERROR "pipenv installation failed")
    endif()
endif()

find_program(PipEnv pipenv REQUIRED)

add_custom_target(
install_pipenv
COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} install --ignore-pipfile
COMMENT "Installing pipenv dependencies [${PipEnv} install]"
DEPENDS	${CMAKE_CURRENT_SOURCE_DIR}/Pipfile.lock
)

add_custom_target(
	extract_measures
	COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extractTarjan.py
	COMMAND PIPENV_PIPFILE=${CMAKE_CURRENT_SOURCE_DIR}/Pipfile ${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extractKosaraju.py
	COMMENT "Generating version header [${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extractTarjan.py]"
	COMMENT "Generating version header [${PipEnv} run python ${CMAKE_CURRENT_SOURCE_DIR}/measures/extractKosaraju.py]"
	DEPENDS	
	install_pipenv
	${CMAKE_CURRENT_SOURCE_DIR}/measures/extractTarjan.py
	${CMAKE_CURRENT_SOURCE_DIR}/measures/extractKosaraju.py
)