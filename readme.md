# Parallel Programming Lab
## Merge Sort ALgorithm
### The mini project was implemented using C++ with OpenMP for parallelism.
**The folder conatins the follwoing files:**
- `input_generator.py`: Helps generate input.txt with input array of size n consisting of all double values upto required number of decimal places.
- `static_mergesort.cpp`: Implements threshold using depth of recursion tree and finds its optimal value.
- `dynamic_mergesort.cpp`: Implements threshold as max number of elements in a subarray while splitting the array during mergesort and finds its optimal value.
- `complete_mergesort.cpp`: Implements 3 methods namely:
    - mergeSortSequential()
    - mergeSortParallelStatic()
    - mergeSortParallelDynamic()

**Instructions to run:**
- Install openmp
- Generate input file as:
    `python input_generator.py `
- Compile file as:
    `g++ -fopenmp -O2 filename.cpp -o executable_name`
- Run executable file with input as:
    `./executable_name < input.txt`