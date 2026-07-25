# Parallel Merge Sort in C++11

[![C++11](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://en.cppreference.com/w/cpp/11)
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)](https://github.com/Soyebsoyeb/ParallelMergeSort)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Threads](https://img.shields.io/badge/threads-pthread-orange.svg)](https://en.wikipedia.org/wiki/POSIX_Threads)
[![Standard](https://img.shields.io/badge/standard-ISO%20C%2B%2B11-blueviolet.svg)](https://isocpp.org/std/the-standard)

A high-performance, multi-threaded merge sort implementation in modern C++11 featuring three distinct execution strategies: sequential, parallel (2-thread recursive), and thread-pool optimized. Benchmarked on 10 million elements with automatic speedup analysis.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Performance](#performance)
- [Build Instructions](#build-instructions)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Technical Details](#technical-details)
- [Benchmarking](#benchmarking)
- [Future Enhancements](#future-enhancements)

---

## Overview

This project demonstrates advanced concurrent sorting algorithms using C++11 threading primitives. It compares three approaches to sorting large datasets:

| Strategy | Description | Best For |
|----------|-------------|----------|
| **Sequential Merge Sort** | Classic divide-and-conquer with single-thread execution | Baseline comparison, small datasets |
| **Parallel Merge Sort** | Recursive spawning of 2 threads per subarray division | Moderate parallelism, educational purposes |
| **Thread Pool Merge Sort** | Reuses threads via `std::async` with threshold optimization | Production workloads, minimal overhead |

---

## Features

- **Three Sorting Implementations** in a unified benchmark framework
- **Threshold-Based Optimization** automatically switches to `std::sort` for subarrays below 5,000 elements
- **Custom Thread Pool** with `std::future` support for task enqueueing and result retrieval
- **High-Precision Timing** using `std::chrono::high_resolution_clock`
- **Zero-Copy Design** sorts in-place via pointer to original vector
- **Cross-Platform** standard C++11 with POSIX threads

---

## Architecture

```
main.cpp
    |
    |---> MergeSort (sequential baseline)
    |
    |---> ParallelMergeSort (recursive 2-thread spawning)
    |        |
    |        +---> THRESHOLD check -> std::sort (small arrays)
    |        +---> std::thread (left half)
    |        +---> std::thread (right half)
    |        +---> join() -> merge()
    |
    |---> ParallelMergeSortThreadPool (async-based parallelism)
             |
             +---> THRESHOLD check -> std::sort (small arrays)
             +---> std::async (left half)
             +---> current thread (right half)
             +---> future.wait() -> merge()
```

### Thread Pool Design

The included `ThreadPool` class (`threadPool.hpp`) provides:
- **Worker Thread Management** with `std::thread::hardware_concurrency()` default sizing
- **Task Queue** with mutex-protected `std::queue`
- **Condition Variable** signaling for efficient thread wake/sleep cycles
- **Future-Based Results** via `std::packaged_task` and `std::future`
- **Graceful Shutdown** with atomic stop flag and join on destruction

---

## Performance

Benchmarks are executed on **10,000,000 random integers** (range: 0 - 9,999,999). All three variants sort identical input data for fair comparison.

### Expected Output

```
MergeSort (Sequential):          [X.XXXX] seconds
ParallelMergeSort (2-thread):    [Y.YYYY] seconds
ParallelMergeSort (ThreadPool):  [Z.ZZZZ] seconds

Performance Summary:
Speedup (2-thread):              [S.S]x
Speedup (ThreadPool):            [T.T]x
ThreadPool improvement:          [P]% faster than 2-thread
```

### Design Optimizations

| Optimization | Impact |
|--------------|--------|
| **Threshold Switching** | Eliminates thread creation overhead on small partitions by delegating to `std::sort` (introsort) |
| **In-Place Pointer Passing** | Avoids vector copying; all sorters operate on original memory via `std::vector<int>*` |
| **Async Over Raw Threads** | ThreadPool variant uses `std::async` with `std::launch::async` policy for better thread reuse and reduced system call overhead |
| **Balanced Recursion** | Midpoint calculation uses `left + (right - left) / 2` to prevent integer overflow |

---

## Build Instructions

### Prerequisites

- GCC 4.8+ or Clang 3.3+ (C++11 support required)
- POSIX thread library (`-pthread`)
- Make utility

### Compile

```bash
# Clone the repository
git clone https://github.com/Soyebsoyeb/ParallelMergeSort.git
cd ParallelMergeSort

# Build the main benchmark
make all

# Or manually:
g++ -std=c++11 -O2 -pthread -Wall \
    src/app/main.cpp \
    src/app/sorting/mergeSort.cpp \
    src/app/sorting/parallelMergeSort.cpp \
    src/app/sorting/parallelMergeSortThreadPool.cpp \
    -o parallel_sort
```

### Build Targets

| Target | Command | Description |
|--------|---------|-------------|
| `all` | `make all` | Compiles the benchmark binary |
| `run` | `make run` | Builds and executes the benchmark |
| `clean` | `make clean` | Removes compiled binary |

### Thread Pool Test

```bash
# Compile and run the thread pool standalone test
g++ -std=c++11 -O2 -pthread -Wall test_pool.cpp -o test_pool
./test_pool
```

---

## Usage

### Run Benchmark

```bash
./parallel_sort
```

The program automatically:
1. Generates 10M random integers
2. Copies them into 3 identical vectors
3. Times each sorting strategy
4. Computes speedup ratios

### Use as Library

Include the headers and instantiate with a pointer to your vector:

```cpp
#include "sorting/parallelMergeSortThreadPool.hpp"

std::vector<int> data = { /* ... */ };
ParallelMergeSortThreadPool sorter(&data);
sorter.sort();
```

---

## Project Structure

```
ParallelMergeSort/
├── Makefile                              # Build configuration
├── README.md                             # Project documentation
├── test_pool.cpp                         # Standalone ThreadPool unit test
├── test_pool                             # Compiled test binary (remote)
├── parallel_sort                         # Compiled benchmark binary (remote)
├── .gitignore
└── src/
    └── app/
        ├── main.cpp                      # Benchmark driver (10M elements)
        └── sorting/
            ├── mergeSort.hpp             # Sequential merge sort interface
            ├── mergeSort.cpp             # Sequential merge sort implementation
            ├── parallelMergeSort.hpp     # 2-thread parallel interface
            ├── parallelMergeSort.cpp     # 2-thread parallel implementation
            ├── parallelMergeSortThreadPool.hpp  # Async/thread-pool interface
            ├── parallelMergeSortThreadPool.cpp  # Async/thread-pool implementation
            └── threadPool.hpp            # Reusable generic thread pool
```

---

## Technical Details

### Algorithm Complexity

| Metric | Sequential | Parallel (2-thread) | Thread Pool |
|--------|------------|---------------------|-------------|
| **Time** | O(n log n) | O(n log n / p) avg | O(n log n / p) avg |
| **Space** | O(n) auxiliary | O(n) auxiliary | O(n) auxiliary |
| **Threads** | 1 | ~2^log(n/THRESHOLD) | Bounded by hardware concurrency |

*Where `p` = number of physical cores, `n` = array size*

### Key Implementation Notes

- **Thread Safety**: The `std::vector<int>* nums` pointer is shared but read/write operations are synchronized via `join()` / `future.wait()` before merge phases. No explicit mutex is required in the sort logic.
- **Memory**: Auxiliary `std::vector<int> result` is allocated per merge call. For production use, a pre-allocated scratch buffer could reduce allocations.
- **Scalability**: The 2-thread variant creates exponentially many threads (2 per recursion level). The ThreadPool variant is recommended for datasets larger than 10M elements or systems with limited thread resources.

---

## Benchmarking

The benchmark framework in `main.cpp` provides:

- **Identical Input Guarantee**: All three sorters receive the same random seed values
- **High-Resolution Timing**: `std::chrono::high_resolution_clock` with `duration<double>` precision
- **Automatic Speedup Calculation**: Relative performance vs. sequential baseline

### Interpreting Results

- **Speedup (2-thread)**: Typically 1.2x - 1.8x on dual-core systems. Diminishing returns occur due to thread creation overhead.
- **Speedup (ThreadPool)**: Typically 1.5x - 2.5x on multi-core systems. Better cache locality and reduced thread spawn overhead.
- **ThreadPool Improvement**: Measures efficiency gain over naive 2-thread spawning. Positive values indicate the thread pool is superior.

---

## Future Enhancements

- [ ] **In-Place Merge**: Reduce auxiliary memory from O(n) to O(1) using block-based merging
- [ ] **SIMD Optimization**: Vectorize the merge loop with SSE/AVX intrinsics for integer comparison
- [ ] **NUMA Awareness**: Pin threads to specific cores for large dataset sorting on multi-socket systems
- [ ] **Benchmark Suite**: Add Google Benchmark for statistical significance across multiple runs
- [ ] **Template Generics**: Convert `int` specialization to `template <typename T>` for broader applicability
- [ ] **Hybrid Algorithm**: Integrate Timsort or Introsort at the threshold boundary for better cache behavior

---

## License

MIT License - see repository for details.

---

**Author**: Soyebsoyeb  
**Repository**: https://github.com/Soyebsoyeb/ParallelMergeSort
