#include <iostream>      // For cout, endl
#include <vector>        // For vector container
#include <mutex>         // For thread synchronization
#include <chrono>        // For high-precision timing
#include "sorting/mergeSort.hpp"                    // Sequential merge sort
#include "sorting/parallelMergeSort.hpp"            // Parallel merge sort (2 threads)
#include "sorting/parallelMergeSortThreadPool.hpp"  // Parallel merge sort (thread pool)


int main(int argc, char* argv[]){
    // Array size: 10 million elements to show performance differences
    const int SIZE = 10000000;

    // Three identical vectors for fair comparison
    std::vector<int> nums(SIZE);   // Sequential version
    std::vector<int> nums1(SIZE);  // 2-thread version
    std::vector<int> nums2(SIZE);  // Thread pool version

    // Fill all vectors with same random numbers
    for(int i=0 ; i<SIZE ; i++){
        nums[i] = rand() % 10000000;   // Random numbers between 0-9,999,999
        nums1[i] = nums[i];            // Copy to second vector
        nums2[i] = nums[i];            // Copy to third vector
    }

    // Test 1: Sequential Merge Sort (single thread)
  
    
    MergeSort* mergesort = new MergeSort(&nums);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    mergesort->sort();
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> mergeSortDuration = end - start;
    std::cout << "MergeSort (Sequential): " << mergeSortDuration.count() << " seconds" << std::endl;
    delete mergesort;


    // Test 2: Parallel Merge Sort (creates 2 threads per recursion)
  

    ParallelMergeSort* mergesort1 = new ParallelMergeSort(&nums1);
    
    auto start2 = std::chrono::high_resolution_clock::now();
    mergesort1->sort();
    
    auto end2 = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> parallelMergeSortDuration = end2 - start2;
    std::cout << "ParallelMergeSort (2-thread): " << parallelMergeSortDuration.count() << " seconds" << std::endl;
    delete mergesort1;


    // Test 3: Parallel Merge Sort with Thread Pool (reuses threads)
    
    ParallelMergeSortThreadPool* mergesort2 = new ParallelMergeSortThreadPool(&nums2);

    auto start3 = std::chrono::high_resolution_clock::now();

    mergesort2->sort();

    auto end3 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> threadPoolDuration = end3 - start3;
    std::cout << "ParallelMergeSort (ThreadPool): " << threadPoolDuration.count() << " seconds" << std::endl;
    delete mergesort2;

    // Performance Summary - Calculate speedups

    std::cout << "\nPerformance Summary:" << std::endl;

    double speedup1 = mergeSortDuration.count() / parallelMergeSortDuration.count();
    double speedup2 = mergeSortDuration.count() / threadPoolDuration.count();

    std::cout << "Speedup (2-thread): " << speedup1 << "x" << std::endl;
    std::cout << "Speedup (ThreadPool): " << speedup2 << "x" << std::endl;
    std::cout << "ThreadPool improvement: " 
              << ((parallelMergeSortDuration.count() - threadPoolDuration.count()) 
                  / parallelMergeSortDuration.count() * 100) 
              << "% faster than 2-thread" << std::endl;

    return 0;
}