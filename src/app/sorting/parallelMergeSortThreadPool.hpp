#ifndef PARALLELMERGESORTTHREADPOOL_H
#define PARALLELMERGESORTTHREADPOOL_H

#include <vector>
#include <atomic>

class ParallelMergeSortThreadPool {
    private:
        std::vector<int> *nums;
        
    public:
        ParallelMergeSortThreadPool(std::vector<int> *nums);
        ~ParallelMergeSortThreadPool();
        void sort();
        void recursiveSort(int left, int right);
};

#endif