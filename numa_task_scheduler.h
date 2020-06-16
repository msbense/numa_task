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
//Note (relevant later): hwloc get_area_membind() takes ~36k microseconds for the first call,
//                          each additional call ~1 microsecond (on cloudlab c220g1)
class NUMATaskScheduler {
    
    public:
        NUMATaskScheduler() : 
            num_numa_nodes_(NUMANumNodes()), 
            main_threadpool_(num_numa_nodes_ * NUMANumCoresPerNode()) {
            
            std::function<void(int)> bind_to_node = [](int node) { 
                NUMAThreadBind(node); 
            };
            
            for (int i = 0; i < num_numa_nodes_; i++) {
                threadpool_list_.emplace_back(NUMANumCoresPerNode(), std::bind(bind_to_node, i));
            } 
        }

        //false if task cannot be scheduled
        bool Schedule(std::function<void()> f, int node, NUMABindPolicy bind_pref) {
            if (node < 0 || threadpool_list_.size() < node) {
                return false;
            }
            std::list<ThreadPool>::iterator node_threadpool = threadpool_list_.begin();
            if (node > 0) std::advance(node_threadpool, node); 
            
            std::unique_lock<std::mutex> ulock(mtx_);
            if (bind_pref == NUMABindPolicy::kStrict) {
                (*node_threadpool).Schedule(f);
            }
            else if (bind_pref == NUMABindPolicy::kNonStrict) {
                if ((*node_threadpool).NumInactiveThreads() > 0) {
                    (*node_threadpool).Schedule(f);
                }
                else {
                     main_threadpool_.Schedule(f);
                }
            }   
            return true;
        }

    private:
        int num_numa_nodes_;
        ThreadPool main_threadpool_; //Threads scheduled here can run on any node
        std::list<ThreadPool> threadpool_list_;
        std::mutex mtx_;
};