#include <iostream>
#include <vector>
#include <mutex>
#include <chrono>
#include "sorting/mergeSort.hpp"
#include "sorting/parallelMergeSort.hpp"


int main(int argc , char* argv[]){
    // Define Array with 10 million elements
    const int SIZE = 10000000;

    // Create two Identical vectors for fair comparison
    std::vector<int> nums(SIZE);
    std::vector<int> nums1(SIZE);

    // Fill Both Vectors  with random Numbers
    for(int i=0 ; i<SIZE ; i++){
        nums[i] = rand() % 10000000;
        nums1[i] = nums[i];
    }


    // (1) SEQUENTIAL MERGE SORT BENCHMARK

    // Create MergeSort Object with pointer to first vector
    MergeSort* mergesort = new MergeSort(&nums);

    // Start Timing
    auto start = std::chrono::high_resolution_clock::now();

    // Perform Sequential Sort
    mergesort->sort();

    auto end = std::chrono::high_resolution_clock::now();

    // Calculations and Display Duration
    std::chrono::duration<double> mergeSortDuration = end - start;
    std::cout << "MergeSort time taken: " << mergeSortDuration.count() << " seconds" << std::endl;

    // clean up the sequential sort object
    delete mergesort;



    // (2) Parallel Merge Sort BenchMark

    // Create ParallelMergeSort object with pointer to second vector
    ParallelMergeSort* mergesort1 = new ParallelMergeSort(&nums1);

     // Start timing
    auto start2 = std::chrono::high_resolution_clock::now();
    
    // Perform parallel sort (uses multiple threads)
    mergesort1->sort();
    
    // End timing
    auto end2 = std::chrono::high_resolution_clock::now();
    
    // Calculate and display duration
    std::chrono::duration<double> parallelMergeSortDuration = end2 - start2;
    std::cout << "ParallelMergeSort time taken: " << parallelMergeSortDuration.count() << " seconds" << std::endl;   

    // Clean up parallel sort objects
    delete mergesort1;


    return 0;
}