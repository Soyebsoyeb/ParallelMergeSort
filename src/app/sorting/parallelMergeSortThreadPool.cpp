#include "parallelMergeSortThreadPool.hpp"
#include <algorithm>
#include <future>
#include <iostream>

ParallelMergeSortThreadPool::ParallelMergeSortThreadPool(std::vector<int> *nums){
    this->nums = nums;
}

ParallelMergeSortThreadPool::~ParallelMergeSortThreadPool() {}

void ParallelMergeSortThreadPool::recursiveSort(int left, int right){
    const int THRESHOLD = 5000;
    
    if (right - left < THRESHOLD) {
        std::sort(nums->begin() + left, nums->begin() + right + 1);
        return;
    }
    
    if (left >= right) return;
    
    int mid = left + (right - left) / 2;
    
    // Use std::async for parallel execution (simpler than thread pool)
    auto future = std::async(std::launch::async, [this, left, mid] {
        this->recursiveSort(left, mid);
    });
    
    // Sort right half in current thread
    this->recursiveSort(mid + 1, right);
    
    // Wait for left half
    future.wait();
    
    // Merge
    std::vector<int> result;
    int i = left;
    int j = mid + 1;
    
    while (i <= mid && j <= right) {
        if ((*nums)[i] <= (*nums)[j]) {
            result.push_back((*nums)[i]);
            i++;
        } else {
            result.push_back((*nums)[j]);
            j++;
        }
    }
    
    while (i <= mid) {
        result.push_back((*nums)[i]);
        i++;
    }
    
    while (j <= right) {
        result.push_back((*nums)[j]);
        j++;
    }
    
    for (size_t k = 0; k < result.size(); k++) {
        (*nums)[left + k] = result[k];
    }
}

void ParallelMergeSortThreadPool::sort(){
    if ((*nums).size() == 0) return;
    recursiveSort(0, (*nums).size() - 1);
}
