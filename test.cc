#include <iostream>
#include <sstream>
#include "threadpool.h"
#include "numa_task_scheduler.h"
#include "numa.h"

int main(int argc, char** argv) {
    // ThreadPool pool(3);
    // for (int i = 0; i < 4; i++) {
    //     auto f = [i]() {
    //         std::this_thread::sleep_for(std::chrono::seconds(2 * i));
    //         std::stringstream sstream;
    //         sstream << "Hello from thread " << std::this_thread::get_id() << " " << i << "\n";
    //         std::cerr << sstream.str(); 
    //     };
    //     pool.Schedule(f);
    // }
    return 0;
}