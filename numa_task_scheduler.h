#include <vector>
#include <functional>
#include "logging.h"
#include "numa.h"
#include "threadpool.h"


class NUMATaskScheduler {
    
    public:
        NUMATaskScheduler() : num_numa_nodes_(NumNUMANodes()) {
            
        }

        void Schedule(std::function<void()> f, int node) {
            
        }

    private:
        std::vector<ThreadPool> threadpool_list_;
        int num_numa_nodes_;
};