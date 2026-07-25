#ifndef MERGESORT_H

#define MERGESORT_H

#include <vector>

class MergeSort {
    private:
    // Pointed to the vector to sort (for copy avoiding)
        std::vector<int> *nums; 

    public:

        MergeSort(std::vector<int> *nums);
        ~MergeSort();
        void sort();   // Public Method to start sorting
        
        // Recursive Divide and Conquer
        void recursiveSort(int left , int right);
};

#endif
