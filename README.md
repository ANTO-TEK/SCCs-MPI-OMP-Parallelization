# ParallelSCCAssignment

## Dependencies

* CMake 3.22.1+
* MPICH
* OPENMP
* Python 3.10.6
* Pipenv

## How to run

1. Create a build directory and launch cmake

   ```batch
   mkdir build
   cd build
   cmake ..
   ```

2. Generate executables with `make`
3. To generate measures (It can take a long time!) run `make generate_measures` 
4. To extract mean times and speedup curves from them run `make extract_measures`

Results can be found in the `measures`, divided into `Tarjan` and `Kosaraju` directories with all the optimization version (O0, O1, O2, O3).

Finally, if it is necessary to remove all executable files from a binary program and coding directory there is a simple command: `make clean`