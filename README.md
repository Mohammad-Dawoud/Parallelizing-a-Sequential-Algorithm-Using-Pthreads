# Parallel Algorithm Implementation: OpenMP vs Pthreads

## Project Overview
This project demonstrates the parallelization of an image filtering algorithm using two different parallel programming approaches: OpenMP and POSIX Threads (Pthreads). The goal is to compare the performance characteristics of these two parallelization methods when applied to image processing tasks.

## Problem Statement
The project implements a parallel version of an image filtering algorithm that processes input images based on specific criteria. The implementation includes:
- A sequential version (baseline)
- An OpenMP parallel version
- A Pthreads parallel version

## Parallelization with OpenMP
The OpenMP implementation uses the following parallelization approach:
- Utilizes OpenMP's high-level parallel for pragmas to distribute loop iterations across available threads
- Implements dynamic scheduling to handle load balancing for different image regions
- Uses thread-local variables to minimize false sharing
- Employs OpenMP's reduction operations for efficient result combination

## Project Structure
```
Parallel_HW2/
├── src/
│   ├── sequential_filter_apply.cpp   # Sequential implementation
│   ├── parallel_filter_openmp.cpp    # OpenMP parallel implementation
│   ├── parallel_filter_pthreads.cpp  # Pthreads implementation
│   └── Makefile                      # Build configuration
└── README.md                         # This file
```

## Building and Running

### Dependencies
- C++11, OpenMP, Pthreads, OpenCV, GNU Make

### Build Commands
```bash
make all              # Build all versions
make clean            # Clean build files
make test             # Run tests with sample images
```

This will generate the following executables:
- `sequential_filter`
- `parallel_filter_openmp`
- `parallel_filter_pthreads`

## Compilation and Execution
### Dependencies
- C++11 or later
- OpenMP (for the OpenMP implementation)
- Pthreads (usually comes with the system)
- OpenCV (for image processing)
- GNU Make

### Compiling with OpenMP
```bash
g++ -o parallel_filter_openmp parallel_filter_openmp.cpp -fopenmp `pkg-config --cflags --libs opencv4` -O3
```

### Running the OpenMP Version
```bash
OMP_NUM_THREADS=<num_threads> ./parallel_filter_openmp <input_image> <output_image> <filter_parameter>
```
Example:
```bash
OMP_NUM_THREADS=8 ./parallel_filter_openmp input.jpg output.jpg 0.5
```

## Performance Analysis

### Test Environment
- **CPU**: Intel Core i7-10750H @ 2.60 GHz (6 cores, 12 threads)
- **RAM**: 16 GB DDR4-2933 MT/s
- **OS**: Ubuntu 24.04.2 LTS

### Test Images
| Size   | Resolution | Pixels   |
|--------|------------|----------|
| Small  | 696×1000   | ~0.7 MP  |
| Medium | 5184×3054  | ~16 MP   |
| Large  | 14575×8441 | ~123 MP  |

### Performance Results

#### Execution Time (seconds)
| Image Size | Sequential | OpenMP (8 threads) | Pthreads (8 threads) |
|------------|------------|--------------------|----------------------|
| Small      | 0.04721s   | 0.004539s          | 0.020637s            |
| Medium     | 1.08756s   | 0.055615s          | 0.213523s            |
| Large      | 10.22s     | 0.594062s          | 2.01654s             |

#### Speedup Over Sequential
| Implementation       | Speedup (x) |
|----------------------|-------------|
| OpenMP (8 threads)   | 10.4x       |
| Pthreads (8 threads) | 2.4x        |

### Observations
1. **Performance Comparison**: The OpenMP implementation shows significantly better performance compared to both sequential and Pthreads implementations.
2. **OpenMP Efficiency**: With 8 threads, OpenMP achieves a ~10.4x speedup over the sequential version, indicating excellent parallel scaling.
3. **Pthreads Performance**: The Pthreads implementation shows a ~2.4x speedup, which is good but notably less than OpenMP's performance.
4. **Implementation Differences**: The performance gap between OpenMP and Pthreads suggests potential optimizations in the OpenMP implementation or overhead in the Pthreads version that could be investigated further.
### Challenges and Solutions
1. **Load Balancing**: 
   - *Challenge*: Uneven work distribution in image processing loops
   - *Solution*: Implemented dynamic scheduling with an appropriate chunk size

2. **False Sharing**:
   - *Challenge*: Performance degradation due to cache line sharing
   - *Solution*: Used thread-local storage and padding to minimize false sharing

3. **Memory Access Patterns**:
   - *Challenge*: Inefficient memory access in nested loops
   - *Solution*: Optimized loop ordering and memory access patterns for better cache utilization

## Author
[Made by Mohammad Dawoud 2025]
