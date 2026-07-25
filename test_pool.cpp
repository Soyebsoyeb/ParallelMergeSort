#include <iostream>
#include <thread>
#include <chrono>
#include "src/app/sorting/threadPool.hpp"

int main() {
    std::cout << "Testing ThreadPool..." << std::endl;
    
    ThreadPool pool(4);
    
    for (int i = 0; i < 5; i++) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " running on thread " 
                      << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "ThreadPool test complete!" << std::endl;
    return 0;
}
