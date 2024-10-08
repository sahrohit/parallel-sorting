# Sorting Algorithms Comparison

This project implements several sorting algorithms in both serial and parallel versions using OpenMP.

## Implementations

1. Quick Sort
   - Serial (qss)
   - Parallel (qsp)
2. Merge Sort
   - Serial (mss)
   - Parallel (msp)
3. Bubble Sort
   - Serial (bbs)
   - Parallel (bbp)
4. Reference implementation using STL sort (reference)

## Building

To build all implementations, simply run:

```
make
```

This will create all seven executables.

To clean up build files:

```
make clean
```

## Running

Each program takes three command-line arguments:

1. Number of random integers to generate
2. Number of threads to use (for parallel versions)
3. Seed value for random number generation

Example usage:

```
./qss 1000000 4 42
```

This runs the serial quicksort implementation on 1,000,000 integers, using 4 threads (ignored in serial versions), with a random seed of 42.

## Notes

- All implementations use the same random number generation for fair comparison
- Timing information is printed to stderr at the end of each run
- The parallel implementations use OpenMP for parallelization