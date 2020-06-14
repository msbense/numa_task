#include <iostream>
#include <string>
#include <sstream>
#include <thread>

void VLog(std::string msg) {
    ELog(msg);
}

void ELog(std::string msg) {
    std::cerr << msg << std::endl;
}

void ThreadLog(std::string msg) {
    std::stringstream sstream;
    sstream << "Thread " << std::this_thread::get_id() << ": " << msg << std::endl;   
    std::cerr << sstream.str();
}

