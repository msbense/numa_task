#pragma once
#include <list>
#include <functional>
#include "logging.h"
#include "numa.h"
#include "threadpool.h"

enum class NUMABindPolicy {
    kNonStrict,
    kStrict
};

//TODO memory binding policies
class NUMATaskScheduler {
    
    public:
        NUMATaskScheduler() : num_numa_nodes_(NUMANumNodes()) {
            std::function<void(int)> bind_to_node = [](int node) { 
                NUMAThreadBind(node); 
            };
            
            for (int i = 0; i < num_numa_nodes_; i++) {
                // ThreadPool tp(NUMANumCoresPerNode(), std::bind(bind_to_node, i));
                threadpool_list_.emplace_back(NUMANumCoresPerNode(), std::bind(bind_to_node, i));
            } 
        }

        //false if task cannot be scheduled
        bool Schedule(std::function<void()> f, int node, NUMABindPolicy bind_pref) {
            if (node < 0 || threadpool_list_.size() < node) {
                return false;
            }
            std::list<ThreadPool>::iterator i = threadpool_list_.begin();
            if (node > 0) std::advance(i, node); 
            if (bind_pref == NUMABindPolicy::kStrict) {
                (*i).Schedule(f);
            }
            else if (bind_pref == NUMABindPolicy::kNonStrict) {
                if ((*i).NumInactiveThreads > 0) {
                    
                }
            }   
            return true;
        }

    private:
        std::list<ThreadPool> threadpool_list_;
        int num_numa_nodes_;
};