#include "parallelMergeSort.hpp"
#include <algorithm>

// Constructor : Initialize with pointer to vector
ParallelMergeSort::ParallelMergeSort(std::vector<int> *nums){
    this->nums = nums;
}

// Destructor
ParallelMergeSort::~ParallelMergeSort() {}


// Recursive parallel Merge Sort
void ParallelMergeSort::recursiveSort(int left , int right){

    // THRESHOLD: When subarray is small enough ,
    //             Use std::sort for efficiency

    const int THRESHOLD = 5000;

    // If subarray is smaller than threshold , sort sequentially
    // This avoids thread overhead for smaller partitions

    if(right - left < THRESHOLD){
        std::sort(nums->begin()+left , nums->begin()+right+1);
        return;
    }

    // BASE CLASS : Single element or Empty SubArray
    if(left >= right) return;

    // DIVIDE : Calculate Middle Index
    int mid = left + (right - left)/2;

    // PARALLEL CONQUER : Create two threads to sort halves simulataneously

    // Thread1 sorts left half : [left , mid]
    std::thread thread_1([this , left ,mid] {
        this->recursiveSort(left , mid);
    });

    // Thread2 sorts right half: [mid+1 , right]
    int mid_1 = mid+1;
    std::thread thread_2([this,mid_1 ,right]{
        this->recursiveSort(mid_1 , right);
    });

    // Wait before joining
    thread_1.join();
    thread_2.join();

    // MERGE: Combine sorted halves (same as sequential version)
    
    std::vector<int> result;

    int i = left;
    int j = mid + 1;

    // Compare and merge elements from both halves
    while (i <= mid && j <= right) {
        if ((*nums)[i] <= (*nums)[j]) {
            result.push_back((*nums)[i]);
            i++;
        } else {
            result.push_back((*nums)[j]);
            j++;
        }
    }

    // Copy remaining elements from left half
    while (i <= mid) {
        result.push_back((*nums)[i]);
        i++;
    }

    // Copy remaining elements from right half
    while (j <= right) {
        result.push_back((*nums)[j]);
        j++;
    }

    // Copy merged result back to original array
    for (int k = 0; k < result.size(); k++) {
        (*nums)[left + k] = result[k];
    }

}

// Public Method to start parallel sorting

void ParallelMergeSort::sort(){
    // Check if vector is empty
    if((*nums).size() == 0){
        exit(1);
    }

    // Create a thread for the entire sorting process
    // This allows the main thread to continue ( though it wait amyway)

    std::thread thread_1([this]{
        this->recursiveSort(0 , (*nums).size()-1);
    });

    // Waiting for soting to complete 
    thread_1.join();
}

