#include "mergeSort.hpp"
#include <cstdlib>


// Constructor store the pointer to the vector

MergeSort::MergeSort(std::vector<int> *nums){
    this->nums = nums;
}

// Empty Destructor (No Dynamic Memory to clean)
MergeSort::~MergeSort() {}


// Recursive MergeSort Implementation
void MergeSort::recursiveSort(int left , int right){

    if(left >= right) return;

    // Divide : Calculate Middle Index ( Avoid Overflow)
    int mid = left + (right - left)/2;

    // Conquer
    recursiveSort(left , mid);
    recursiveSort(mid+1 , right);

    // Merge
    std::vector<int> result;

    int i = left;
    int j = mid+1;

    while(i<=mid && j<=right){
        if((*nums)[i] <= (*nums)[j]){
            result.push_back((*nums)[i]);
            i++;
        }
        else {
            result.push_back((*nums)[j]);
            j++;
        }
    }

    while(i <= mid){
        result.push_back((*nums)[i]);
        i++;
    }

    while(j <= right){
        result.push_back((*nums)[j]);
        j++;
    }

    // Copy Merged Result Back to Original Array
    for(int k=0; k < result.size() ; k++){
        (*nums)[left + k] = result[k]; 
    }
}


// Public Method to start Sorting

void MergeSort::sort() {
    // Check if vector is empty

    if((*nums).size() == 0){
        exit(1);
    }
    recursiveSort(0 , (*nums).size() - 1);
}