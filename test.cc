#include <iostream>
#include <sstream>
#include "logging.h"
#include "threadpool.h"
#include "numa_task_scheduler.h"

int main(int argc, char** argv) {
    // ThreadPool pool(3);
    // for (int i = 0; i < 4; i++) {
    auto f = []() {
        // std::this_thread::sleep_for(std::chrono::seconds(2 * i));
        std::stringstream sstream;
        sstream << "Hello from thread " << std::this_thread::get_id() << "\n";// << i << "\n";
        std::cerr << sstream.str(); 
    };
    // pool.Schedule(f);
// }
    // NUMATaskScheduler nts;
    
    // if (nts.Schedule(f, 0, NUMABindPolicy::kStrict)) {
    //     VLog("Task scheduled");
    // }
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}