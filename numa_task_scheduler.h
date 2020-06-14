#pragma once
#include <vector>
#include <functional>
#include "logging.h"
#include "numa.h"
#include "threadpool.h"


class NUMATaskScheduler {
    
    public:
        NUMATaskScheduler() : num_numa_nodes_(NUMANumNodes()) {
            std::function<void(int)> bind_to_node = [](int node) { NUMAThreadBind(node); };
            for (int i = 0; i < num_numa_nodes_; i++) {
                ThreadPool tp(NUMANumCoresPerNode(), std::bind(bind_to_node, i));
                threadpool_list_.push_back(tp);
            } 
        }

        void Schedule(std::function<void()> f, int node) {
            
        }

    private:
        std::vector<std::reference_wrapper<ThreadPool>> threadpool_list_;
        int num_numa_nodes_;
};