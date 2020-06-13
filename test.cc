#include <iostream>
#include <sstream>
#include "threadpool.h"


void func() {
    std::cout << "Right here\n";
}

int main(int argc, char** argv) {

    ThreadPool pool(10);

    for (int i = 0; i < 20; i++) {
        auto f = [i]() {
            std::stringstream sstream;
            sstream << "Hello from thread " << std::this_thread::get_id() << " " << i << "\n";
            std::cerr << sstream.str(); 
        };
        pool.Schedule(f);
    }
    return 0;
}