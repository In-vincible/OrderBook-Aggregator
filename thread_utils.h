#ifndef THREAD_UTILS_H
#define __THREAD_UTILS_H

#include <vector>
#include <future>
#include <functional>
#include <iostream>
#include <chrono>
#include <any>  // Added this include

class ThreadGroup {
private:
    std::vector<std::future<std::any>> futures;

public:
    // Add a task that returns anything
    template<typename Func>
    void add(Func task) {
        futures.push_back(std::async(std::launch::async, 
            [task]() -> std::any { 
                return task(); 
            }
        ));
    }

    // Collect results of a specific type
    template<typename T>
    std::vector<T> collect_results() {
        std::vector<T> results;
        for (auto& fut : futures) {
            results.push_back(std::any_cast<T>(fut.get()));
        }
        futures.clear();
        return results;
    }

    // Wait for all tasks to complete
    void join_all() {
        for (auto& fut : futures) {
            fut.wait();
        }
        futures.clear();
    }
};

#endif // THREAD_UTILS_H