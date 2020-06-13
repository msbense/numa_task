#include <iostream>
#include <sstream>
#include "threadpool.h"


void func() {
    std::cout << "Right here\n";
}

int main(int argc, char** argv) {

    ThreadPool pool(5);

    auto f = [](){
        std::stringstream sstream;
        sstream << "Hello from thread " << std::this_thread::get_id() << " " << "\n";
        std::cout << sstream.str(); 
    };

    for (int i = 0; i < 5; i++) {
        pool.Schedule(f);
    }
    return 0;
}